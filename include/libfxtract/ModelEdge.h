#ifndef MODEL_EDGE_H_
#define MODEL_EDGE_H_

#include <IGESControl_Reader.hxx>
#include <XSControl_Reader.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#include <TopoDS_Shape.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepLib_MakeWire.hxx>
#include <GeomConvert.hxx>
#include <Geom_Surface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <GeomLProp_SLProps.hxx>
#include <BRepLProp_SLProps.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>
#include <Interface_Static.hxx>
#include <vector>

const int EDGE_NUM = -1;

enum class EdgeType { DEFAULT = -1, LINE, ARC };
enum class EdgePosition { DEFAULT = -1, JOINING_EDGE, SIDE_EDGE, DISJOINT_EDGE };

class ModelEdge
{
  int count;
  Standard_Integer edge_number;
  EdgeType edge_type;
  EdgePosition edge_position;
  gp_Pnt line_vector;
  gp_Pnt line_unit_vector;
  Standard_Real edge_length;
  bool isrational;
  Standard_Integer same_as_edge_number;
  TopoDS_Edge edge;
public:
  gp_Pnt start_vertex;
  gp_Pnt terminate_vertex;
  // bool isjoining_edge;
  // bool isside_edge;
  // bool isdisjoint_edge;

  ModelEdge(std::vector<gp_Pnt> startv);
  ModelEdge(){}
  ~ModelEdge(){}

  void setIsRational(bool is_it);
  bool IsRational();
  // void setIsJoiningEdge(bool is_it) { isjoining_edge = is_it;  }
  // void setIsSideEdge(bool is_it) { isside_edge = is_it;  }
  // void setIsDisjointEdge(bool is_it) { isdisjoint_edge = is_it;  }

  void setEdge(TopoDS_Edge edge);
  TopoDS_Edge getEdge();

  void setEdgeNum(Standard_Integer enumb);//std::cout << edge_number << ": ";  }
  Standard_Integer getEdgeNum() const;

  void setEdgeType(GeomAbs_CurveType type);
  EdgeType getEdgeType();

  void setEdgePosition(EdgePosition epos);
  EdgePosition getEdgePosition();

  void setEdgeLength(Standard_Real len);
  Standard_Real getEdgeLength();

  void setLineVector(gp_Pnt l_vector);
  gp_Pnt getLineVector();

  void setLineUnitVector(gp_Pnt lu_vector);
  gp_Pnt getLineUnitVector();

  static bool compare_edges(ModelEdge edge1, ModelEdge edge2);

  static bool compare_vl(gp_Pnt v1, gp_Pnt v2);
};

#endif // MODEL_EDGE_H_
