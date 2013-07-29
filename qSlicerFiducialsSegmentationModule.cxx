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

// Qt includes
#include <QtPlugin>

// FiducialsSegmentation Logic includes
#include <vtkSlicerFiducialsSegmentationLogic.h>

// FiducialsSegmentation includes
#include "qSlicerFiducialsSegmentationModule.h"
#include "qSlicerFiducialsSegmentationModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerFiducialsSegmentationModule, qSlicerFiducialsSegmentationModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerFiducialsSegmentationModulePrivate
{
public:
  qSlicerFiducialsSegmentationModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerFiducialsSegmentationModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerFiducialsSegmentationModulePrivate
::qSlicerFiducialsSegmentationModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerFiducialsSegmentationModule methods

//-----------------------------------------------------------------------------
qSlicerFiducialsSegmentationModule
::qSlicerFiducialsSegmentationModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerFiducialsSegmentationModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerFiducialsSegmentationModule::~qSlicerFiducialsSegmentationModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerFiducialsSegmentationModule::helpText()const
{
  return "This is a loadable module designated to mirror a model with a surface. You can find contact information in the Acknowledgement.";
}

//-----------------------------------------------------------------------------
QString qSlicerFiducialsSegmentationModule::acknowledgementText()const
{
  return "This module is done at Shanghai Jiao Tong University by Jiaxi Luo (JiaxiLuo@umich.edu) & Ruqing Ye (ruqing@umich.edu) directed by Xiaojun Chen (xiaojunchen@sjtu.edu.cn).";
}

//-----------------------------------------------------------------------------
QStringList qSlicerFiducialsSegmentationModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Jiaxi LUO, Ruqing YE, Xiaojun CHEN (SJTU)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerFiducialsSegmentationModule::icon()const
{
  return QIcon(":/Icons/FiducialsSegmentation.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerFiducialsSegmentationModule::categories() const
{
  return QStringList() << "Segmentation";
}

//-----------------------------------------------------------------------------
QStringList qSlicerFiducialsSegmentationModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerFiducialsSegmentationModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerFiducialsSegmentationModule
::createWidgetRepresentation()
{
  return new qSlicerFiducialsSegmentationModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerFiducialsSegmentationModule::createLogic()
{
  return vtkSlicerFiducialsSegmentationLogic::New();
}
