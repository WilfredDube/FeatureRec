#include "ModelEdge.h"

namespace Fxt {

  ModelEdge::ModelEdge(std::vector<gp_Pnt> startv) : count(0), edge_number(EDGE_NUM), edge_type(EdgeType::DEFAULT){
    edge_type = EdgeType::DEFAULT;
    start_vertex = startv[0];
    terminate_vertex = startv[1];
    // setEdgeNum(++count);
  }

  void ModelEdge::setIsRational(bool is_it) { isrational = is_it; }
  bool ModelEdge::IsRational(){ return isrational; }
  // void setIsJoiningEdge(bool is_it) { isjoining_edge = is_it;  }
  // void setIsSideEdge(bool is_it) { isside_edge = is_it;  }
  // void setIsDisjointEdge(bool is_it) { isdisjoint_edge = is_it;  }

  void ModelEdge::setEdge(TopoDS_Edge edge){ this->edge = edge; }
  TopoDS_Edge ModelEdge::getEdge(){ return this->edge; }

  void ModelEdge::setEdgeNum(Standard_Integer enumb){  edge_number = enumb; }//std::cout << edge_number << ": ";  }
  Standard_Integer ModelEdge::getEdgeNum() const { return edge_number; }

  void ModelEdge::setEdgeType(GeomAbs_CurveType type){
    switch (type) {
      case GeomAbs_Line:
      // std::cout << "Line" << '\n';
      edge_type = EdgeType::LINE;
      break;
      case GeomAbs_Circle:
      // std::cout << "Circle" << '\n';
      edge_type = EdgeType::ARC;
      break;
      case GeomAbs_Ellipse:
      // std::cout << "Ellipse" << '\n';
      edge_type = EdgeType::ARC;
      break;
      case GeomAbs_Hyperbola:
      // std::cout << "Hyperbola" << '\n';
      edge_type = EdgeType::ARC;
      break;
      case GeomAbs_Parabola:
      // std::cout << "Parabola" << '\n';
      edge_type = EdgeType::ARC;
      break;
      case GeomAbs_BezierCurve:
      // std::cout << "BezierCurve" << '\n';
      edge_type = EdgeType::ARC;
      break;
      case GeomAbs_BSplineCurve:
      // std::cout << "BSplineCurve" << '\n';
      edge_type = EdgeType::ARC;
      break;
      case GeomAbs_OffsetCurve:
      // std::cout << "OffsetCurve" << '\n';
      edge_type = EdgeType::ARC;
      break;
      case GeomAbs_OtherCurve:
      default:
      // std::cout << "OtherCurve" << '\n';
      edge_type = EdgeType::ARC;
    }
    // if (type >= 1) {
    //
    //   std::cout << "Circle" << '\n';
    //   edge_type = EdgeType::ARC;
    //
    if (type == GeomAbs_BSplineCurve ) {
      if (IsRational() != true)
      {
        // std::cout << "BSpline" << '\n';
        edge_type = EdgeType::LINE;
      }
    }
    // } else {
    //   edge_type = EdgeType::LINE;
    // }
  }

  EdgeType ModelEdge::getEdgeType(){ return edge_type; }

  void ModelEdge::setEdgePosition(EdgePosition epos){ edge_position = epos; }
  EdgePosition ModelEdge::getEdgePosition(){ return edge_position; }

  void ModelEdge::setEdgeLength(Standard_Real len) { edge_length = len; }
  Standard_Real ModelEdge::getEdgeLength(){
    // std::cout << "in edge: " << edge_length << '\n';
    return  edge_length;
  }

  void ModelEdge::setLineVector(gp_Pnt l_vector) { line_vector = l_vector; }
  gp_Pnt ModelEdge::getLineVector() { return line_vector; }

  void ModelEdge::setLineUnitVector(gp_Pnt lu_vector) { line_unit_vector = lu_vector; }
  gp_Pnt ModelEdge::getLineUnitVector() { return line_unit_vector; }

  /* compare edges */
  bool ModelEdge::compare_edges(ModelEdge edge1, ModelEdge edge2){
    bool equal = false;

    if (((compare_vl(edge1.start_vertex, edge2.start_vertex) && compare_vl(edge1.terminate_vertex, edge2.terminate_vertex)) ||
    (compare_vl(edge1.start_vertex, edge2.terminate_vertex) && compare_vl(edge1.terminate_vertex, edge2.start_vertex))) ) {
      // if (compare_vl(edge1.line_unit_vector, edge2.line_unit_vector)) {
      // }
      // std::cout << "("<< edge1.start_vertex.X()<< ", "<< edge1.start_vertex.Y()<< ", "<< edge1.start_vertex.Z() << ") ==== ";
      // std::cout << "("<< edge1.terminate_vertex.X()<< ", "<< edge1.terminate_vertex.Y()<< ", "<< edge1.terminate_vertex.Z() << ")\n";
      //
      // std::cout << "("<< edge2.start_vertex.X()<< ", "<< edge2.start_vertex.Y()<< ", "<< edge2.start_vertex.Z() << ") ==== ";
      // std::cout << "("<< edge2.terminate_vertex.X()<< ", "<< edge2.terminate_vertex.Y()<< ", "<< edge2.terminate_vertex.Z() << ")\n";
      // std::cout << "" << '\n';
      equal = true;
    } else {

    }

    if (((compare_vl(edge1.start_vertex, edge2.start_vertex) || compare_vl(edge1.terminate_vertex, edge2.terminate_vertex)) && compare_vl(edge1.line_unit_vector, edge2.line_unit_vector)) ||
    ((compare_vl(edge1.start_vertex, edge2.terminate_vertex) || compare_vl(edge1.terminate_vertex, edge2.start_vertex)) && compare_vl(edge1.line_unit_vector, edge2.line_unit_vector)) ) {
      // std::cout << "/* message */" << '\n';
      equal = true;
    }

    return equal;
  }

  bool ModelEdge::compare_vl(gp_Pnt v1, gp_Pnt v2)
  {
    bool truth = false;

    if ((v1.X() == v2.X()) && (v1.Y() == v2.Y()) && (v1.Z() == v2.Z())) {
      truth = true;
    }

    return truth;
  }

}
