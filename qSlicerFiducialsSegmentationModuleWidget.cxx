/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

//	MRML includes
#include <vtkMRMLModelDisplayNode.h>
#include <vtkMRMLModelNode.h>
#include <vtkMRMLModelStorageNode.h>
#include <vtkMRMLNode.h>
#include <vtkMRMLScene.h>

#include <vtkMRMLHierarchyNode.h>
#include <vtkMRMLAnnotationHierarchyNode.h>
#include <vtkMRMLAnnotationFiducialNode.h>


//	Qt includes
#include <QDebug>
#include <QList>
#include <QMessageBox>
#include "QPlane.h"
#include <QString>
#include <QColor>  
#include <QPalette>


//	SlicerQt includes
#include <qMRMLThreeDView.h>
#include <qMRMLThreeDWidget.h>
#include <qSlicerApplication.h>
#include <qSlicerIOManager.h>
#include <qSlicerLayoutManager.h>
#include "qSlicerFiducialsSegmentationModuleWidget.h"
#include "ui_qSlicerFiducialsSegmentationModule.h"

//	vtk includes
#include <vtkActor.h>
#include <vtkClipPolyData.h>
#include <vtkImplicitBoolean.h>
#include <vtkMath.h>
#include <vtkPlane.h>
#include <vtkPlaneCollection.h>
#include "vtkPlaneExtend.h"
#include <vtkPlaneSource.h>
#include <vtkPlaneWidget.h>
#include <vtkPolyData.h>
#include <vtkPolyLine.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

#include <vtkCollection.h>
#include <vtkGeneralTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkReverseSense.h>
#include <vtkObject.h>
#include <vtkCollectionIterator.h>

#include <math.h>
#include <float.h>

#include <iostream> // for debugging

#include <vector>
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkPolyDataWriter.h"
#include "vtkPolyData.h"
#include "vtkPluginFilterWatcher.h"
#include "ModuleEntry.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"



///	\ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerFiducialsSegmentationModuleWidgetPrivate: public Ui_qSlicerFiducialsSegmentationModule
{
	Q_DECLARE_PUBLIC(qSlicerFiducialsSegmentationModuleWidget);

public:
	qSlicerFiducialsSegmentationModuleWidgetPrivate(qSlicerFiducialsSegmentationModuleWidget& object);

protected:
	qSlicerFiducialsSegmentationModuleWidget* const q_ptr;
};

//	qSlicerFiducialsSegmentationModuleWidgetPrivate methods

qSlicerFiducialsSegmentationModuleWidgetPrivate::
	qSlicerFiducialsSegmentationModuleWidgetPrivate(qSlicerFiducialsSegmentationModuleWidget& object)
	 : q_ptr(&object)
{
}

//	qSlicerFiducialsSegmentationModuleWidget methods

qSlicerFiducialsSegmentationModuleWidget::qSlicerFiducialsSegmentationModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerFiducialsSegmentationModuleWidgetPrivate(*this) )
{
  //  return the reference to the renderWindow of the Slicer application singleton 
  //  and those to its renderer and renderWindowInteractor
	qSlicerApplication *app = qSlicerApplication::application();
	qSlicerLayoutManager *layoutManager = app->layoutManager();
	qMRMLThreeDWidget *threeDWidget = layoutManager->threeDWidget(0);
	qMRMLThreeDView *threeDView = threeDWidget->threeDView();
	this->renderWindow = threeDView->renderWindow();
	this->renderer = this->renderWindow->GetRenderers()->GetFirstRenderer();
	this->iren = this->renderWindow->GetInteractor();
    this->timeOfSegmentation = 0;

}

qSlicerFiducialsSegmentationModuleWidget::~qSlicerFiducialsSegmentationModuleWidget()
{
}


void qSlicerFiducialsSegmentationModuleWidget::segment()
{
	Q_D(qSlicerFiducialsSegmentationModuleWidget);

	QPalette pal;
	pal.setColor(QPalette::Text, QColor( QString("red") ));
	d->hintText->setPalette(pal);

	//	obtain the source model for clipping
  	qSlicerApplication *app = qSlicerApplication::application();
	vtkMRMLScene *mrmlScene = app->mrmlScene();

	vtkMRMLNode *sourceNode = d->modelNodeComboBox->currentNode();
	if (sourceNode) {
        this->timeOfSegmentation ++;
		this->renderWindow->Render();
	  	vtkSmartPointer<vtkMRMLModelNode> sourceModel =
		vtkSmartPointer<vtkMRMLModelNode>::New();
		sourceModel->Copy(sourceNode);
		
		vtkSmartPointer<vtkPolyData> sourcePD = vtkSmartPointer<vtkPolyData>::New();
		sourcePD->DeepCopy(sourceModel->GetPolyData());

		vtkSmartPointer<vtkPolyData> resultPD = vtkSmartPointer<vtkPolyData>::New();

		MeshContourEvolver::InitParam init = {0, 0, 0, 0, false};

		// get fiducials coordinates
         vtkMRMLAnnotationHierarchyNode *annotationHierarchy =
            vtkMRMLAnnotationHierarchyNode::SafeDownCast(d->annotationNodeComboBox->currentNode());
		vtkCollection *annotationChildrenCollection = vtkCollection::New();
		annotationHierarchy->GetAllChildren(annotationChildrenCollection);
		int maxNumCollection = annotationChildrenCollection->GetNumberOfItems();
		int numCollection = 0;
		double **coord = new double*[maxNumCollection];
		for (int i = 0; i < maxNumCollection; i++)
	    {
	        coord[i]  = new double[3];
	    }
		vtkCollectionIterator *iterator = annotationChildrenCollection->NewIterator();
		iterator->InitTraversal();
		vtkObject *object = NULL;
		vtkMRMLAnnotationFiducialNode *fiducial = NULL;

		while ( !(iterator->IsDoneWithTraversal()) ) {
			object = iterator->GetCurrentObject();
			iterator->GoToNextItem();

			fiducial = vtkMRMLAnnotationFiducialNode::SafeDownCast(object);

			if (fiducial) {
				if (fiducial->GetFiducialCoordinates(coord[numCollection]) ){
					numCollection ++;
	 			}
			}
		}

		// transfer double **coord to std::vector< std::vector<float> >
		std::vector< std::vector<float> > initPoints3D;
        for(int i = 0; i < numCollection; i++)
        {
            std::vector<float> point;
            initPoints3D.push_back(point);
            for (int j = 0; j < 3; j++)
                initPoints3D[i].push_back( (float) coord[i][j] );
        }
		

		// segmentation
		MeshContourEvolver::entry_main( sourcePD, initPoints3D, resultPD, init);

		this->resultList.append(resultPD);

		//	display and store the result model
		vtkSmartPointer<vtkMRMLModelNode> resultModel =
			vtkSmartPointer<vtkMRMLModelNode>::New();
		QString resultName = tr("FiducialsSegmentation Part_");
		resultName.append(QString::number(this->timeOfSegmentation));
		resultModel->SetName(resultName.toLatin1().data());
		resultModel->SetAndObservePolyData(this->resultList.at(this->timeOfSegmentation - 1));
		mrmlScene->SaveStateForUndo();
		resultModel->SetScene(mrmlScene);

		vtkSmartPointer<vtkMRMLModelDisplayNode> resultDisplay =
			vtkSmartPointer<vtkMRMLModelDisplayNode>::New();
		vtkSmartPointer<vtkMRMLModelStorageNode> resultStorage =
			vtkSmartPointer<vtkMRMLModelStorageNode>::New();
		resultDisplay->SetScene(mrmlScene);
		resultStorage->SetScene(mrmlScene);
		resultDisplay->SetInputPolyData(resultModel->GetPolyData());
		resultDisplay->SetColor(1.0, 0.0, 0.0);
		resultStorage->SetFileName(resultName.toLatin1().data());
		mrmlScene->AddNode(resultDisplay);
		mrmlScene->AddNode(resultStorage);
		resultModel->SetAndObserveDisplayNodeID(resultDisplay->GetID());
		resultModel->SetAndObserveStorageNodeID(resultStorage->GetID());

		mrmlScene->AddNode(resultModel);
		d->hintText->setText("");

	} else d->hintText->setText("Please select a model to mirror!");

}


void qSlicerFiducialsSegmentationModuleWidget::setup()
{
	Q_D(qSlicerFiducialsSegmentationModuleWidget);
	d->setupUi(this);
	this->Superclass::setup();

	QObject::connect(d->segmentButton, SIGNAL(clicked()), this, SLOT(segment()));
}
