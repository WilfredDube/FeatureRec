#include "ModelEdge.h"
#include "ModelUtils.h"
#include "ModelMath.h"

namespace Fxt {
  ModelEdge::ModelEdge(){}
  ModelEdge::ModelEdge(TopoDS_Edge edge)
  {
    mModelEdge = edge;

    setIsRational(BRepAdaptor_Curve(edge).IsRational());

    BRepAdaptor_Curve curve = BRepAdaptor_Curve(edge);
    setEdgeType(BRepAdaptor_Curve(edge).GetType());

    setEdgeLength(computeLength(*this));
  }

  TopoDS_Edge ModelEdge::getTModelEdge()
  {
    return mModelEdge;
  }

  std::vector<gp_Pnt> ModelEdge::getEdgeEndPoints()
  {
    std::vector<gp_Pnt> endPoints;

    for (TopExp_Explorer vertexEx(mModelEdge, TopAbs_VERTEX); vertexEx.More(); vertexEx.Next())
    {
      TopoDS_Vertex vertex = TopoDS::Vertex(vertexEx.Current());
      gp_Pnt currentVertex = BRep_Tool::Pnt(vertex);
      endPoints.push_back(currentVertex);
    }

    return endPoints;
  }

  void ModelEdge::setIsRational(bool is_it) { isrational = is_it; }
  bool ModelEdge::IsRational(){ return isrational; }

  void ModelEdge::setEdgeNum(EdgeID enumb){  mEdgeNumber = enumb; }
  EdgeID ModelEdge::getEdgeNum() const { return mEdgeNumber; }

  void ModelEdge::setEdgeType(GeomAbs_CurveType type){
    switch (type) {
      case GeomAbs_Line:
      mEdgeType = EdgeType::LINE;
      break;
      case GeomAbs_Circle:
      mEdgeType = EdgeType::ARC;
      break;
      case GeomAbs_Ellipse:
      mEdgeType = EdgeType::ARC;
      break;
      case GeomAbs_Hyperbola:
      mEdgeType = EdgeType::ARC;
      break;
      case GeomAbs_Parabola:
      mEdgeType = EdgeType::ARC;
      break;
      case GeomAbs_BezierCurve:
      mEdgeType = EdgeType::ARC;
      break;
      case GeomAbs_BSplineCurve:
      mEdgeType = EdgeType::ARC;
      if (IsRational() != true)
      {
        mEdgeType = EdgeType::LINE;
      }
      break;
      case GeomAbs_OffsetCurve:
      mEdgeType = EdgeType::ARC;
      break;
      case GeomAbs_OtherCurve:
      default:;
      mEdgeType = EdgeType::ARC;
    }
  }

  EdgeType ModelEdge::getEdgeType(){ return mEdgeType; }

  void ModelEdge::setEdgePosition(EdgePosition epos){ mEdgePosition = epos; }
  EdgePosition ModelEdge::getEdgePosition(){ return mEdgePosition; }

  void ModelEdge::setEdgeLength(Standard_Real len) { mEdgeLength = len; }
  Standard_Real ModelEdge::getEdgeLength(){
    return  mEdgeLength;
  }
}
