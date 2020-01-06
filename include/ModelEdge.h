#pragma once

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

  ModelEdge(){}
  ModelEdge(std::vector<gp_Pnt> startv) : count(0), edge_number(EDGE_NUM), edge_type(EdgeType::DEFAULT){
    edge_type = EdgeType::DEFAULT;
    start_vertex = startv[0];
    terminate_vertex = startv[1];
    // setEdgeNum(++count);
  };

  void setIsRational(bool is_it) { isrational = is_it; }
  bool IsRational(){ return isrational; }
  // void setIsJoiningEdge(bool is_it) { isjoining_edge = is_it;  }
  // void setIsSideEdge(bool is_it) { isside_edge = is_it;  }
  // void setIsDisjointEdge(bool is_it) { isdisjoint_edge = is_it;  }

  void setEdge(TopoDS_Edge edge){ this->edge = edge; }
  TopoDS_Edge getEdge(){ return this->edge; }

  void setEdgeNum(Standard_Integer enumb){  edge_number = enumb; }//std::cout << edge_number << ": ";  }
  Standard_Integer getEdgeNum() const { return edge_number; }

  void setEdgeType(){
      if (IsRational() == true) {
        edge_type = EdgeType::ARC;
      } else {
        edge_type = EdgeType::LINE;
      }
    }

  EdgeType getEdgeType(){ return edge_type; }

  void setEdgePosition(EdgePosition epos){ edge_position = epos; }
  EdgePosition getEdgePosition(){ return edge_position; }

  void setEdgeLength(Standard_Real len) { edge_length = len; }
  Standard_Real getEdgeLength(){
    std::cout << "in edge: " << edge_length << '\n';
    return  edge_length;
  }

  void setLineVector(gp_Pnt l_vector) { line_vector = l_vector; }
  gp_Pnt getLineVector() { return line_vector; }

  void setLineUnitVector(gp_Pnt lu_vector) { line_unit_vector = lu_vector; }
  gp_Pnt getLineUnitVector() { return line_unit_vector; }

  /* compare edges */
  static bool compare_edges(ModelEdge edge1, ModelEdge edge2){
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

  static bool
  compare_vl(gp_Pnt v1, gp_Pnt v2)
  {
    bool truth = false;

    if ((v1.X() == v2.X()) && (v1.Y() == v2.Y()) && (v1.Z() == v2.Z())) {
      truth = true;
    }

    return truth;
  }

};
