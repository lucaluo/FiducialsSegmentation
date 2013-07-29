/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// FooBar Widgets includes
#include "qSlicerFiducialsSegmentationFooBarWidget.h"
#include "ui_qSlicerFiducialsSegmentationFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_FiducialsSegmentation
class qSlicerFiducialsSegmentationFooBarWidgetPrivate
  : public Ui_qSlicerFiducialsSegmentationFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerFiducialsSegmentationFooBarWidget);
protected:
  qSlicerFiducialsSegmentationFooBarWidget* const q_ptr;

public:
  qSlicerFiducialsSegmentationFooBarWidgetPrivate(
    qSlicerFiducialsSegmentationFooBarWidget& object);
  virtual void setupUi(qSlicerFiducialsSegmentationFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicerFiducialsSegmentationFooBarWidgetPrivate
::qSlicerFiducialsSegmentationFooBarWidgetPrivate(
  qSlicerFiducialsSegmentationFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerFiducialsSegmentationFooBarWidgetPrivate
::setupUi(qSlicerFiducialsSegmentationFooBarWidget* widget)
{
  this->Ui_qSlicerFiducialsSegmentationFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerFiducialsSegmentationFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicerFiducialsSegmentationFooBarWidget
::qSlicerFiducialsSegmentationFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerFiducialsSegmentationFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicerFiducialsSegmentationFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerFiducialsSegmentationFooBarWidget
::~qSlicerFiducialsSegmentationFooBarWidget()
{
}
