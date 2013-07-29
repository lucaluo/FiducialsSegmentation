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

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#include "vtkSlicerConfigure.h" // For Slicer_USE_PYTHONQT

// Qt includes
#include <QDir>
#include <QTimer>
#ifdef Slicer_USE_PYTHONQT
# include <QVariant>
#endif
#include <QWidget>

// CTK includes
#include <ctkErrorLogModel.h>

// SlicerQt includes
#include <qSlicerAbstractModuleRepresentation.h>
#include <qSlicerAbstractModuleWidget.h>
#include <qSlicerApplication.h>
#ifndef GENERIC_CXX_MODULE_TEST_SKIP_DEPENDENCIES
# include <qSlicerApplicationHelper.h>
# include <qSlicerModuleFactoryManager.h>
# include <qSlicerModuleManager.h>
#endif
#ifdef Slicer_USE_PYTHONQT
# include <qSlicerLoadableModule.h>
# include <qSlicerPythonManager.h>
#endif

// FiducialsSegmentation includes
#include "qSlicerFiducialsSegmentationModule.h"

// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkNew.h>

const int minimumWidth = 500;

//-----------------------------------------------------------------------------
int qSlicerFiducialsSegmentationModuleWidgetGenericTest( int argc, char * argv[] )
{
  qSlicerApplication app(argc, argv);
  app.errorLogModel()->setTerminalOutputs(ctkErrorLogModel::All);

  qSlicerFiducialsSegmentationModule module(0);

#ifndef GENERIC_CXX_MODULE_TEST_SKIP_DEPENDENCIES
  if (!module.dependencies().isEmpty())
    {
    qSlicerModuleFactoryManager * moduleFactoryManager = app.moduleManager()->factoryManager();
    qSlicerApplicationHelper::setupModuleFactoryManager(moduleFactoryManager);
    moduleFactoryManager->setExplicitModules(module.dependencies());

    moduleFactoryManager->registerModules();
    qDebug() << "Number of registered modules:"
             << moduleFactoryManager->registeredModuleNames().count();

    moduleFactoryManager->instantiateModules();
    qDebug() << "Number of instantiated modules:"
             << moduleFactoryManager->instantiatedModuleNames().count();

    // Load all available modules
    foreach(const QString& name, moduleFactoryManager->instantiatedModuleNames())
      {
      Q_ASSERT(!name.isNull());
      moduleFactoryManager->loadModule(name);
      }
    }
#endif

  QString modulePathWithoutIntDir = QLatin1String("/Users/luoluca/Documents/Programing/Slicer/Modules/Loadable/FiducialsSegmentation/lib/Slicer-4.2/qt-loadable-modules");
  if (!QFile::exists(modulePathWithoutIntDir))
    {
    std::cerr << "Line " << __LINE__ << " Problem with @" << "MODULEPATH_WITHOUT_INTDIR@ - "
              << " File " << qPrintable(modulePathWithoutIntDir) << " doesn't exist " << std::endl;
    return EXIT_FAILURE;
    }

  QString moduleFileName = "libqSlicerFiducialsSegmentationModule.so";

  QDir moduleDir(modulePathWithoutIntDir);
  if (!app.intDir().isEmpty())
    {
    moduleDir.cd(app.intDir());
    }
  QString modulePath = moduleDir.filePath(moduleFileName);

  // Path can be required for modules depending on locale ressources.
  // This usually applies to widget representation implemented in python.
  module.setPath(modulePath);

#ifdef Slicer_USE_PYTHONQT
  qSlicerPythonManager * pythonManager = app.pythonManager();
  {
    bool current = qSlicerLoadableModule::importModulePythonExtensions(
          pythonManager, app.intDir(), module.path());
    bool expected = true;
    if (current != expected)
      {
      std::cerr << "Line " << __LINE__ << " Problem with importModulePythonExtensions\n"
                << "\tcurrent:" << current << "\n"
                << "\texpected:" << expected << std::endl;
      return EXIT_FAILURE;
      }
  }
  {
    bool current = qSlicerLoadableModule::addModuleToSlicerModules(
          pythonManager, &module, "FiducialsSegmentation");
    bool expected = true;
    if (current != expected)
      {
      std::cerr << "Line " << __LINE__ << " Problem with addModuleToSlicerModules\n"
                << "\tcurrent:" << current << "\n"
                << "\texpected:" << expected << std::endl;
      return EXIT_FAILURE;
      }
  }
  {
    bool current = qSlicerLoadableModule::addModuleNameToSlicerModuleNames(
          pythonManager, "FiducialsSegmentation");
    bool expected = true;
    if (current != expected)
      {
      std::cerr << "Line " << __LINE__ << " Problem with addModuleNameToSlicerModuleNames\n"
                << "\tcurrent:" << current << "\n"
                << "\texpected:" << expected << std::endl;
      return EXIT_FAILURE;
      }
  }
#endif

  module.initialize(0);

  vtkNew<vtkMRMLScene> scene;
  module.setMRMLScene(scene.GetPointer());

  // Instantiate the widget if any
  qSlicerAbstractModuleWidget* widget =
    dynamic_cast<qSlicerAbstractModuleWidget*>(module.widgetRepresentation());
  if (!widget)
    {
    return EXIT_SUCCESS;
    }

  // If the test fails here, there might be in the panel a GUI element (e.g. a
  // combobox, a push button...) with a too large minimum size hint.
  // Ideally this minimum size hint should be fixed, it is unlikely that such
  // large size hint is "ideal". Alternatively, you might want to investigate
  // the following:
  // Consider setting the horizontal size policy of such element to "Ignored"
  // If the element is in a form layout, QFormLayout::layoutFieldGrowthPolicy
  // might need to be set to AllNonFixedFieldsGrow.
  // All those changes can be set from Qt Designer.
  if (widget->minimumSizeHint().width() > minimumWidth)
    {
    std::cerr << "Line " << __LINE__
              << " qSlicerFiducialsSegmentationModule widget has a minimum size hint width"
              << " of " << widget->minimumSizeHint().width() << "px. It is wider"
              << " than the max width of " << minimumWidth << "px." << std::endl;
    return EXIT_FAILURE;
    }

  widget->enter();
  widget->exit();

  widget->show();

  widget->enter();
  widget->exit();

#ifdef Slicer_USE_PYTHONQT
  if (pythonManager->pythonErrorOccured())
    {
    return EXIT_FAILURE;
    }
#endif

  if (argc < 2 || QString(argv[1]) != "-I")
    {
    QTimer::singleShot(200, &app, SLOT(quit()));
    }
  return app.exec();
}
