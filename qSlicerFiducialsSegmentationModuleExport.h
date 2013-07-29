/*=auto=========================================================================

 Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) 
 All Rights Reserved.

 See COPYRIGHT.txt
 or http://www.slicer.org/copyright/copyright.txt for details.

 Program:   3D Slicer

=========================================================================auto=*/


// .NAME __qSlicerFiducialsSegmentationModuleExport - manage Windows system differences
// .SECTION Description
// The __qSlicerFiducialsSegmentationModuleExport captures some system differences between Unix
// and Windows operating systems. 

#ifndef __qSlicerFiducialsSegmentationModuleExport_h
#define __qSlicerFiducialsSegmentationModuleExport_h

#if defined(WIN32) && !defined(qSlicerFiducialsSegmentationModule_STATIC)
 #if defined(qSlicerFiducialsSegmentationModule_EXPORTS)
  #define Q_SLICER_QTMODULES_FIDUCIALSSEGMENTATION_EXPORT __declspec( dllexport )
 #else
  #define Q_SLICER_QTMODULES_FIDUCIALSSEGMENTATION_EXPORT __declspec( dllimport )
 #endif
#else
 #define Q_SLICER_QTMODULES_FIDUCIALSSEGMENTATION_EXPORT
#endif

#endif
