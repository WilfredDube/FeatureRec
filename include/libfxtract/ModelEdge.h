#ifndef MODEL_EDGE_H_
#define MODEL_EDGE_H_

#include <BRep_Tool.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <GeomConvert.hxx>
#include <BRepLProp_SLProps.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>
#include <vector>

const int EDGE_NUM = -1;

namespace Fxt {

  using EdgeID = size_t;
  enum class EdgeType { DEFAULT = -1, LINE, ARC };
  enum class EdgePosition { DEFAULT = -1, JOINING_EDGE, SIDE_EDGE, DISJOINT_EDGE };

  class ModelEdge
  {
    EdgeID mEdgeNumber;
    EdgeType mEdgeType;
    EdgePosition mEdgePosition;
    gp_Pnt mEdgeLineVector;
    Standard_Real mEdgeLength;

    //! Whether its a straight line (rational = 0) or arc (rational = 1)
    bool isrational;

    //! Edge ID of an edge in the Model shared by another face.
    Standard_Integer mEdgeSameAsEdgeID;

    TopoDS_Edge mModelEdge;
  public:
    ModelEdge();
    ModelEdge(TopoDS_Edge edge);

    TopoDS_Edge getTModelEdge();

    std::vector<gp_Pnt> getEdgeEndPoints();

    void setIsRational(bool is_it);
    bool IsRational();

    void setEdgeNum(EdgeID enumb);
    EdgeID getEdgeNum() const;

    void setEdgeType(GeomAbs_CurveType type);
    EdgeType getEdgeType();

    void setEdgePosition(EdgePosition epos);
    EdgePosition getEdgePosition();

    void setEdgeLength(Standard_Real len);
    Standard_Real getEdgeLength();
  };

}

#endif // MODEL_EDGE_H_
