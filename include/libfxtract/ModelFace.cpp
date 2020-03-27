/**
* \class ModelFace
*
* This class describes a face of a model shape. It inherits from the OpenCascade TopoDS_Face
* class.
*
* Contact: wilfreddube@gmail.com
*
* Created on: Wed Apr 10 18:39:37 2019
*
*/

#include "ModelFace.h"

ModelFace::ModelFace(Standard_Integer fid, PlaneType planetype): isInternal(false), bend_length(0), bend_type(BendType::UNASSIGNED){
  face_id = fid;
  plane_type = planetype;

  if (this->plane_type == PlaneType::NON_PLANAR) {
    ++nbends;
  }
}

ModelFace::ModelFace() : TopoDS_Face(), joined_to_faceid1(0), joined_to_faceid2(0){
  this->setFaceType(FaceType::NONE);
}

void ModelFace::setIsInternal(bool is_internal) {
  isInternal = is_internal;
}

bool ModelFace::getIsInternal() {
  return isInternal;
}

void ModelFace::setJoiningFaceID1(Standard_Integer face_id) {
  joined_to_faceid1 = face_id;
}
Standard_Integer ModelFace::getJoiningFaceID1() { return joined_to_faceid1; }

void ModelFace::setJoiningFaceID2(Standard_Integer face_id) {
  joined_to_faceid2 = face_id;
}
Standard_Integer ModelFace::getJoiningFaceID2() { return joined_to_faceid2; }

void ModelFace::setBendAngle(long double angle) { bend_angle = angle; }
long double ModelFace::getBendAngle(){ return bend_angle; }

Standard_Real ModelFace::getBendLength(){ return bend_length; }
void ModelFace::setBendLength(std::vector<ModelEdge> face_edges){
  size_t i;
  for (i = 0; i < face_edges.size(); i++) {
    if (face_edges[i].getEdgeType() == EdgeType::LINE) {
      bend_length = face_edges[i].getEdgeLength();
      break;
    }
  }
}

/**
* a normal member taking one argument to set the curvature attribute of the face. If the curvature != 0 then
* the face is a bend face and so its radius is also set. *
* @param cv a real/floating point.
*/
void ModelFace::setCurvature(Standard_Real cv) {
  Curvature = cv;

  if (Curvature != 0) {
    Radius = ((Curvature < 0) ? -(1 / Curvature) : (1 / Curvature)) / 2;
    // std::cout << "Radius : " << Radius <<'\n';
  }
}

Standard_Real ModelFace::getFaceRadius(){ return Radius; }

void ModelFace::setFaceId(Standard_Integer fid) {
  face_id = fid;
}

Standard_Integer ModelFace::getFaceId(){
  return face_id;
}

void ModelFace::setUnitNormal(gp_Dir unit_normal) {
  face_unit_normal = unit_normal;
}

gp_Pnt ModelFace::getNormal(Standard_Integer fid, std::vector<ModelFace> vface){
  gp_Pnt vt(Standard_Real(NULL), Standard_Real(NULL), Standard_Real(NULL));
  for (auto& face : vface) {
    if (face.getFaceId() == fid) {
      vt = face.getFaceNormal();
    }
  }
  return vt;
}

/**
* calculates and sets the D value of the equation of a plane (Ax+By+Cz = D).
*/
void ModelFace::computeFaceEquation(){
  D_element = compute_plane_equation(face_edges, face_normal);
}

gp_Pnt ModelFace::getFaceNormal(){
  return face_normal;
}

void ModelFace::setFaceType(FaceType ftype) {
  face_type = ftype;
}

FaceType ModelFace::getFaceType(){ return face_type; }

void ModelFace::setPlaneType(PlaneType ptype){
  plane_type = ptype;
}

PlaneType ModelFace::getPlaneType(){
  return plane_type;
}

/**
* prints out the unit normal vector of the face.
*/
void ModelFace::printUnitNormal() {
  /* code */
  std::cout << "Unit Normal : ("<< face_unit_normal.X()<<", "<< face_unit_normal.Y()<<", "<<face_unit_normal.Z()<< ")\n";
}

void ModelFace::printNormal() {
  /* code */
  // std::cout << "Unit Normal : ("<< face_normal.X()<<", "<< face_normal.Y()<<", "<<face_normal.Z()<< ")\n";
}

void ModelFace::addEdge(const ModelEdge& n){
  face_edges.push_back(n);
}

void ModelFace::extractEdges(TopoDS_Face face) {
  ModelVertex modelVertex;
  int edge_n = 0;
  for (TopExp_Explorer edgeEx(face, TopAbs_EDGE); edgeEx.More(); edgeEx.Next()){
    TopoDS_Edge edge = TopoDS::Edge(edgeEx.Current());

    std::vector<gp_Pnt> endpoints;
    endpoints = modelVertex.getEdgeEndPoints(edge);

    ModelEdge edgex(endpoints);
    ++edge_n;
    edgex.setEdge(edge);
    edgex.setEdgeNum(edge_n);
    if (edge_n > 4) {
      /* code */
    }

    edgex.setIsRational(BRepAdaptor_Curve(edge).IsRational());
    BRepAdaptor_Curve curve = BRepAdaptor_Curve(edge);
    // Handle_Geom_BSplineCurve spline = curve.BSpline();
    //
    // std::cout << "Type: " << BRepAdaptor_Curve(edge).GetType()<< '\n';
    // std::cout << "Rational: " << BRepAdaptor_Curve(edge).IsRational() << '\n';
    edgex.setEdgeType(BRepAdaptor_Curve(edge).GetType());
    edgex.setEdgeLength(compute_length(&edgex));
    edgex.setLineVector(compute_line_vector(&edgex));
    edgex.setLineUnitVector(compute_line_unit_vector(edgex.getLineVector()));
    addEdge(edgex);
  }

void ModelFace::setNumEdges(size_t nEdges){
  num_edges = nEdges;
}

size_t ModelFace::getNumEdges(){
  return num_edges;
}

std::vector<ModelEdge> ModelFace::getFaceEdges(){
  return face_edges;
}

ModelEdge ModelFace::getFaceEdgesAt(int i){
  // std::cout << "Len : " << face_edges[i].getEdgeLength() << '\n';
  return face_edges[i];
}

/**
* computes and sets the normal vector of the face.
*/
void ModelFace::computeFaceNormal(){
  face_normal = compute_normal(this->face_edges);
  // std::cout << "FACE NORM : (" << face_normal.X()<< ", " << face_normal.Y()<< ", " << face_normal.Z() << ")\n";
}

/**
* Identifies BEND_FACE and BEND_SIDE faces types.
* @param faces a list for faces.
*/
void ModelFace::classifyBS_BF(std::vector<ModelFace>& faces){
  for (size_t i = 0; i < faces.size(); i++) {
    int arc_edge_cnt = 0;
    switch (faces[i].plane_type) {
      case PlaneType::PLANAR :
      for (size_t j = 0; j < faces[i].face_edges.size(); j++) {
        /* If the face is PLANAR check whether there is an ARC edge in the face. */
        if (faces[i].face_edges[j].getEdgeType() == EdgeType::ARC) {
          ++arc_edge_cnt; /* count the arcs. */
        }

        /* If the face has exactly 2 ARC edges then it is a BEND_SIDE face. */
        if (arc_edge_cnt == 2) {
          faces[i].setFaceType(FaceType::BEND_SIDE);
          // std::cout << "BEND_SIDE Face ID = " << faces[i].getFaceId()<<" Type = "<< (faces[i].getPlaneType() == PlaneType::NON_PLANAR ? "NON-PLANAR" : "PLANAR") << '\n';
          arc_edge_cnt = 0;
          break;
        }
      }
      break;
      case PlaneType::NON_PLANAR : faces[i].setFaceType(FaceType::BEND_FACE);
      // std::cout << "BEND_FACE Face ID = " << faces[i].getFaceId()<<" Type = "<< (faces[i].getPlaneType() == PlaneType::NON_PLANAR ? "NON-PLANAR" : "PLANAR") << '\n';
      faces[i].printNormal();
      break;
    }
  }
}
/**
* Identifies FACE and THICKNESS_DEFINING_FACE faces types. Retrieves the BEND_FACE type faces and compares its
* STRAIGHT_LINE edge type edge to all the PLANAR type face. If there is a matching edge then the bend is connected
* to that face. The joining face ID is set to the connected face's ID. The connected face's FaceType is also set
* to FACE.
* @param faces a list for faces.
*/
void ModelFace::classifyFaces(std::vector<ModelFace>& faces) {
  size_t count = faces.size();
  // bool done = false;

  for (size_t i = 0; i < count; i++) {
    /* retrieve the BEND_FACE type faces. */
    if (faces[i].face_type == FaceType::BEND_FACE) {
      // std::cout << "=======Bend ID : "<< faces[i].face_id << '\n';
      for (size_t j = 0; j < count; j++) {
        // if ((faces[j].face_type != FaceType::BEND_FACE) && (faces[j].face_type != FaceType::BEND_SIDE)) {

        /* compare the STRAIGHT_LINE type bend  */
        if (faces[j].plane_type == PlaneType::PLANAR) {
          // std::cout << "/* message */" << '\n';
          for (size_t k = 0; k < 4; k++) {
            for (size_t l = 0; l < 4; l++) {
              if (faces[i].face_edges[k].getEdgeType() == EdgeType::LINE) {
                // if (ModelEdge::compare_edges(faces[i].face_edges[k], faces[j].face_edges[l]))
                if(faces[i].face_edges[k].getEdge().IsSame(faces[j].face_edges[l].getEdge())){
                  // std::cout << "Face ID : "<< faces[j].face_id  << '\n';
                  faces[j].setFaceType(FaceType::FACE);
                  // std::cout << "FACE" << '\n';
                  faces[i].face_edges[k].setEdgePosition(EdgePosition::JOINING_EDGE);
                  faces[j].face_edges[l].setEdgePosition(EdgePosition::JOINING_EDGE);

                  if ((faces[i].getJoiningFaceID1() == 0) && (faces[i].getJoiningFaceID2() == 0)) {
                    faces[i].setJoiningFaceID1(faces[j].face_id);
                  } else if (faces[i].getJoiningFaceID2() == 0){
                    faces[i].setJoiningFaceID2(faces[j].face_id);
                  }
                }
              }
            }
          }
        }
      }

    }

  }
  /* For the unclassified faces set their type to THICKNESS_DEFINING_FACE */
  for (size_t i = 0; i < count; i++) {
    if (faces[i].face_type == FaceType::NONE) {
      // std::cerr << "THICKNESS_DEFINING_FACE" << '\n';
      faces[i].setFaceType(FaceType::THICKNESS_DEFINING_FACE);
    }
  }
}

void ModelFace::classifyBends(std::vector<ModelFace>& v){
  // size_t nb = nbends;

  for (size_t i = 0; i < v.size(); i++) {
    // cout << "FFFFFFF : " << v[i].face_id << endl;
    if (v[i].getFaceType() == FaceType::BEND_FACE) {
      ModelFace *b = &v[i];
      for (size_t j = 0; j < v.size(); j++) {
        if (v[j].getFaceType() == FaceType::BEND_FACE)
        if(b->face_id != v[j].face_id){
          if (b->Radius != v[j].Radius)
          if (b->bend_type == BendType::UNASSIGNED && v[j].bend_type == BendType::UNASSIGNED) {
            if (b->bend_angle == v[j].bend_angle) {
              if (compute_angle(b->getFaceNormal(), v[j].getFaceNormal()) ==  180) {
                if (b->Radius < v[j].Radius) {
                  b->bend_type = BendType::INTERNAL;
                  v[j].bend_type = BendType::EXTERNAL;
                  b->bend_same_as_id = v[j].face_id;
                  v[j].bend_same_as_id = b->face_id;
                  // thickness = fabsl(roundd(v[j].Radius) - roundd(b->Radius));
                  // std::cout << b->face_id << " - " << v[j].face_id << "  ";
                  // std::cout << setprecision(10) << b->Radius << " < " << v[j].Radius << '\n';
                } else {
                  b->bend_type = BendType::EXTERNAL;
                  v[j].bend_type = BendType::INTERNAL;
                  b->bend_same_as_id = v[j].face_id;
                  v[j].bend_same_as_id = b->face_id;
                  // thickness = fabsl(roundd(b->Radius) - roundd(v[j].Radius));
                  // std::cout << b->face_id << " - " << v[j].face_id << "  ";
                  // std::cout << b->Radius << " > " << v[j].Radius << '\n';
                }
                // cout << compute_angle(b->getFaceNormal(), v[j].getFaceNormal()) << endl;
              }
              break;
            }
          }
        }
      }
    }
  }

  // for (size_t i = 0; i < v.size(); i++) {
  //   if (v[i].getFaceType() == FaceType::BEND_FACE) {
  //     // cout << "Joing face : " << v[i].getJoiningFaceID1() << endl;
  //     // cout << "Same as : " << v[i].bend_same_as_id << endl;
  //     // cout << "Joing face : " << v[i].getJoiningFaceID2() << endl;
  //     // ModelFace *b = &v[i];
  //
  //     if (findBendType(v, i, nbends, v.size(), v[i].getJoiningFaceID1()) == BendType::INTERNAL &&
  //         findBendType(v, i, nbends, v.size(), v[i].getJoiningFaceID2()) == BendType::INTERNAL){
  //           cout << "Internal " << endl;
  //           v[i].bend_type = BendType::INTERNAL;
  //     } else if (findBendType(v, i, nbends, v.size(), v[i].getJoiningFaceID1()) == BendType::EXTERNAL &&
  //         findBendType(v, i, nbends, v.size(), v[i].getJoiningFaceID2()) == BendType::EXTERNAL){
  //           v[i].bend_type = BendType::EXTERNAL;
  //           cout << "external " << endl;
  //     }
  //   }
  // }
}

// static BendType findBendType(std::vector<ModelFace>& v, size_t start_pos, size_t n, size_t s, Standard_Integer search_id){
//   BendType btype;
//   for (size_t i = 0; i < s; i++) {
//     if (v[i].getJoiningFaceID1() == search_id || v[i].getJoiningFaceID2() == search_id) {
//       btype = v[i].bend_type;
//       break;
//     }
//   }
//
//   return btype;
// }

int ModelFace::nbends = 0;
Standard_Real ModelFace::thickness = 0.0;
// TODO :
// Fill in all members of each class
// create bend class
// get bend face association
