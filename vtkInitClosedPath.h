
#ifndef __vtkInitClosedPath_h
#define __vtkInitClosedPath_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkSmartPointer.h"

#include <iostream> // debug

class vtkInitClosedPath : public vtkPolyDataAlgorithm
{
public:
  vtkTypeRevisionMacro( vtkInitClosedPath, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Default <TODO>
  static vtkInitClosedPath *New();

  // Description:
  //  Macro for ... ?
  //vtkSetClampMacro( SomeParam,double,0.0,1.0);
  //vtkGetMacro( SomeParam,double);

  // Description:
  // Specify the number of iterations for Laplacian smoothing,
  //vtkSetClampMacro(NumberOfIterations,int,0,VTK_LARGE_INTEGER);
  //vtkGetMacro(NumberOfIterations,int);

  // Description:
  // Specify t..
//  vtkSetMacro(RelaxationFactor,double);
 // vtkGetMacro(RelaxationFactor,double);

  // Description:
  // Turn on/off ....
  //vtkSetMacro(FeatureEdgeSmoothing,int);
  //vtkGetMacro(FeatureEdgeSmoothing,int);
  //vtkBooleanMacro(FeatureEdgeSmoothing,int);

  // Description:
  // Specify the source object ... not needed ??
  void SetSource(vtkPolyData *source);



  void SetInitPointVertexIdx( vtkIntArray* initVertIdx )
        { this->activeContourSeedIdx->DeepCopy( initVertIdx ); }
  vtkPolyData *GetSource();
  vtkIntArray* GetActiveContour( ) 
        { return this->activeContourVertIdx;} // return list of vertex indices of the 'dense curve'
  bool SetForceRecompute( bool bOnOff ){ bool bPrev = bForceRecompute; bForceRecompute = bOnOff; return bPrev; } // set, and return the previous setting to user 

protected:
  vtkInitClosedPath();
  ~vtkInitClosedPath() {};

  virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  virtual int FillInputPortInformation(int port, vtkInformation *info);

private:
  std::vector< std::vector<bool> > connectedCircuit;
  std::vector< std::vector<bool> > connectedEdge;
  bool haveConnectedCircuit (int iiSource, int jjSink) 
    {
      // cout << "haveConnectedCircuit Start" << endl;
      if (connectedCircuit[iiSource][jjSink] || connectedCircuit[jjSink][iiSource])
        return true;
      if (connectedEdge[iiSource][jjSink] || connectedEdge[jjSink][iiSource]) {
        connectedCircuit[iiSource][jjSink] = true;
        return true;
      } else {
        for (int i = 0; i < connectedEdge.size(); i++) {
          if (connectedEdge[iiSource][i]) {
            connectedEdge[iiSource][i] = false;
            connectedEdge[i][iiSource] = false;
            bool result = haveConnectedCircuit (i, jjSink);
            connectedEdge[iiSource][i] = true;
            if (result) {
              connectedCircuit[iiSource][jjSink] = true;
              return true;
            }
          }
          else if (connectedEdge[i][iiSource]) {
            connectedEdge[i][iiSource] = false;
            connectedEdge[iiSource][i] = false;
            bool result = haveConnectedCircuit (i, jjSink);
            connectedEdge[i][iiSource] = true;
            if (result) {
              connectedCircuit[iiSource][jjSink] = true;
              return true;
            }
          }
          // if (connectedEdge[jjSink][i]) {
          //   connectedEdge[jjSink][i] = false;
          //   bool result = haveConnectedCircuit (i, iiSource);
          //   connectedEdge[jjSink][i] = true;
          //   if (result) {
          //     connectedCircuit[iiSource][jjSink] = true;
          //     return true;
          //   }
          // }
          // else if (connectedEdge[i][jjSink]) {
          //   connectedEdge[i][jjSink] = false;
          //   bool result = haveConnectedCircuit (i, iiSource);
          //   connectedEdge[i][jjSink] = true;
          //   if (result) {
          //     connectedCircuit[iiSource][jjSink] = true;
          //     return true;
          //   }
          // }  
        }
      }
      return false;
    }
  bool bForceRecompute;   // Force recomputation, even if active contour init array exists
  vtkSmartPointer<vtkIntArray> activeContourVertIdx; // vertex indices of resulting initial active contour
  vtkSmartPointer<vtkIntArray> activeContourSeedIdx; // vertex indices of resulting initial active contour
  vtkInitClosedPath(const vtkInitClosedPath&);  // Not implemented.
  void operator=(const vtkInitClosedPath&);  // Not implemented.
};

#endif
