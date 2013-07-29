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

#ifndef __qSlicerFiducialsSegmentationModuleWidget_h
#define __qSlicerFiducialsSegmentationModuleWidget_h

//	SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"
#include <qSlicerApplication.h>
#include "qSlicerFiducialsSegmentationModuleExport.h"

//	Qt includes
#include "QPlane.h"

//	vtk includes
#include <vtkClipPolyData.h>
#include <vtkImplicitBoolean.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkPlane.h>
#include <vtkPlaneCollection.h>
#include "vtkPlaneExtend.h"
#include <vtkPlaneWidget.h>
#include <vtkPlaneSource.h>
#include <vtkPolyData.h>

#include <vtkPolyLine.h>

#include <vtkGeneralTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkReverseSense.h>

#include <math.h>

class qSlicerFiducialsSegmentationModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_FIDUCIALSSEGMENTATION_EXPORT qSlicerFiducialsSegmentationModuleWidget : 
	public qSlicerAbstractModuleWidget
{
	Q_OBJECT

public:
	typedef qSlicerAbstractModuleWidget Superclass;
	qSlicerFiducialsSegmentationModuleWidget(QWidget *parent=0);
	virtual ~qSlicerFiducialsSegmentationModuleWidget();

public slots:

protected:
	vtkRenderWindowInteractor* iren;
	vtkRenderWindow* renderWindow;
	vtkRenderer* renderer;

	vtkSmartPointer<vtkPolyData> sourcePD;

	QList<vtkSmartPointer<vtkPolyData> > resultList;

	QScopedPointer<qSlicerFiducialsSegmentationModuleWidgetPrivate> d_ptr;
	virtual void setup();
	int timeOfSegmentation;

protected slots:
	void segment();

private:
	Q_DECLARE_PRIVATE(qSlicerFiducialsSegmentationModuleWidget);
	Q_DISABLE_COPY(qSlicerFiducialsSegmentationModuleWidget);
};

#endif
