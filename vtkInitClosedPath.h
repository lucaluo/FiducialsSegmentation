
#ifndef __vtkInitClosedPath_h
#define __vtkInitClosedPath_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkSmartPointer.h"

struct Edge {
  int pt1;
  int pt2;
};

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
  bool haveConnectedCircuit (std::vector<Edge> connectedEdge, int idxSource, int idxSink) 
    {
      for (int i = 0; i < connectedEdge.size(); i++) {
        if (connectedEdge[i].pt1 == idxSource) {
          if (connectedEdge[i].pt2 == idxSink)
            return true;
          else {
            int newIdx = connectedEdge[i].pt2;
            std::vector<Edge> newConnectedEdge = connectedEdge;
            newConnectedEdge.erase( newConnectedEdge.begin() + i );
            if( haveConnectedCircuit (newConnectedEdge, newIdx, idxSink) )
              return true;
          }
        } else if (connectedEdge[i].pt2 == idxSource) {
          if (connectedEdge[i].pt1 == idxSink)
            return true;
          else {
            int newIdx = connectedEdge[i].pt1;
            std::vector<Edge> newConnectedEdge = connectedEdge;
            newConnectedEdge.erase( newConnectedEdge.begin() + i );
            if( haveConnectedCircuit (newConnectedEdge, newIdx, idxSink) )
              return true;
          }
        }
        if (connectedEdge[i].pt1 == idxSink) {
          if (connectedEdge[i].pt2 == idxSource)
            return true;
          else {
            int newIdx = connectedEdge[i].pt2;
            std::vector<Edge> newConnectedEdge = connectedEdge;
            newConnectedEdge.erase( newConnectedEdge.begin() + i );
            if( haveConnectedCircuit (newConnectedEdge, idxSource, newIdx) )
              return true;
          }
        } else if (connectedEdge[i].pt2 == idxSink) {
          if (connectedEdge[i].pt1 == idxSource)
            return true;
          else {
            int newIdx = connectedEdge[i].pt1;
            std::vector<Edge> newConnectedEdge = connectedEdge;
            newConnectedEdge.erase( newConnectedEdge.begin() + i );
            if( haveConnectedCircuit (newConnectedEdge, idxSource, newIdx) )
              return true;
          }
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
