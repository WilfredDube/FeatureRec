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

#pragma once
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <vector>
#include "ModelEdge.h"

/**
 * enum PlaneType
 * Classifies faces into PLANAR or NON PLANAR types.
 */
enum class PlaneType { PLANAR, NON_PLANAR };

/**
 * enum FaceType
 * Identifies various faces. If the face is not Identified yet it has the FaceType value of NONE.
 *  - NONE : no definition
 *  - THICKNESS_DEFINING_FACE : defines the side of the sheetmetal i.e where the thickness is measured.
 *  - BEND_FACE : defines a bend in the model.
 *  - BEND_SIDE : defines the side face of a bend i.e thickness defining face of the bend.
 *  - FACE : defines the face of the model.
 */
enum class FaceType { NONE, THICKNESS_DEFINING_FACE, BEND_FACE, BEND_SIDE, FACE };

class ModelFace : public TopoDS_Face{
  Standard_Integer face_id; /*!< an integer value  represents the Face Identification number */
  PlaneType plane_type;
  FaceType face_type;
  Standard_Real Curvature; //!< face curvature value : zero for planar type faces and non-zero for non-planar type faces.
  Standard_Real thickness; //!< thickness of the face.
  Standard_Real D_element; //!< represents the D value in the equation of the plane. Ax+By+Cz = D
  gp_Pnt face_normal; //!< the normal vector to the face.
  gp_Dir face_unit_normal; //!< the unit normal vector to the face.
  bool taken;

  Standard_Integer joined_to_faceid1; //!< the face ID of the face connected to the bend face.
  Standard_Integer joined_to_faceid2; //!< the face ID of the second face connected to the bend face.
  Standard_CString parallel_to_faceid; //!< the face ID of the face parallel to this one.

  // Bend face attributes
  ModelEdge imaginary_bend_line;
  Standard_Real bending_force; //!< force required to create the bend.
  Standard_Real bend_angle; //!< bend angle.

  std::vector<ModelEdge> face_edges; //!< edges of the face.
  std::vector<ModelEdge> parallel_face_edges; //!< edges of the parallel face.
  std::vector<ModelEdge> central_face_edges; //!< edges of the central face. code for generating the central face not yet available.
public:
  static int nbends;
  Standard_Real Radius; //!< bend radius : zero if face is planar and non-zero if face is a bend.
  ModelFace(Standard_Integer fid, PlaneType planetype): isInternal(false), bend_length(0){
    face_id = fid;
    plane_type = planetype;

    if (this->plane_type == PlaneType::NON_PLANAR) {
      ++nbends;
    }
  }
  ModelFace() : TopoDS_Face(), joined_to_faceid1(0), joined_to_faceid2(0){
    this->setFaceType(FaceType::NONE);
  }
  ~ModelFace(){}

  void setIsInternal(bool is_internal) {
    isInternal = is_internal;
  }

  bool getIsInternal() {
    return isInternal;
  }

  void setJoiningFaceID1(Standard_Integer face_id) {
    joined_to_faceid1 = face_id;
  }
  Standard_Integer getJoiningFaceID1() { return joined_to_faceid1; }

  void setJoiningFaceID2(Standard_Integer face_id) {
    joined_to_faceid2 = face_id;
  }
  Standard_Integer getJoiningFaceID2() { return joined_to_faceid2; }

  void setBendAngle(long double angle) { bend_angle = angle; }
  long double getBendAngle(){ return bend_angle; }

  Standard_Real getBendLength(){ return bend_length; }
  void setBendLength(std::vector<ModelEdge> face_edges){
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
  void setCurvature(Standard_Real cv) {
    Curvature = cv;

    if (Curvature != 0) {
      Radius = ((Curvature < 0) ? -(1 / Curvature) : (1 / Curvature)) / 2;
      // std::cout << "Radius : " << Radius <<'\n';
    }
  }

  Standard_Real getFaceRadius(){ return Radius; }

  void setFaceId(Standard_Integer fid) {
    face_id = fid;
  }

  Standard_Integer getFaceId(){
    return face_id;
  }

  void setUnitNormal(gp_Dir unit_normal) {
    face_unit_normal = unit_normal;
  }

  gp_Pnt getNormal(Standard_Integer fid, std::vector<ModelFace> vface){
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
  void computeFaceEquation(){
    D_element = compute_plane_equation(face_edges, face_normal);
  }

  gp_Pnt getFaceNormal(){
    return face_normal;
  }

  void setFaceType(FaceType ftype) {
    face_type = ftype;
  }

  FaceType getFaceType(){ return face_type; }

  void setPlaneType(PlaneType ptype){
    plane_type = ptype;
  }

  PlaneType getPlaneType(){
    return plane_type;
  }

  /**
   * prints out the unit normal vector of the face.
   */
  void printUnitNormal() {
    /* code */
    // std::cout << "Unit Normal : ("<< face_unit_normal.X()<<", "<< face_unit_normal.Y()<<", "<<face_unit_normal.Z()<< ")\n";
  }

  void printNormal() {
    /* code */
    // std::cout << "Unit Normal : ("<< face_normal.X()<<", "<< face_normal.Y()<<", "<<face_normal.Z()<< ")\n";
  }

  void addEdge(const ModelEdge& n){
    face_edges.push_back(n);
  }

  void extractEdges(TopoDS_Face face) {
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
      edgex.setIsRational(BRepAdaptor_Curve(edge).IsRational());
      edgex.setEdgeType();
      edgex.setEdgeLength(compute_length(&edgex));
      edgex.setLineVector(compute_line_vector(&edgex));
      edgex.setLineUnitVector(compute_line_unit_vector(edgex.getLineVector()));
      addEdge(edgex);
    }
  }

  std::vector<ModelEdge> getFaceEdges(){
    return face_edges;
  }

/**
 * computes and sets the normal vector of the face.
 */
  void computeFaceNormal(){
    face_normal = compute_normal(this->face_edges);
    // std::cout << "FACE NORM : (" << face_normal.X()<< ", " << face_normal.Y()<< ", " << face_normal.Z() << ")\n";
  }

/**
 * Identifies BEND_FACE and BEND_SIDE faces types.
 * @param faces a list for faces.
 */
  static void classifyBS_BF(std::vector<ModelFace>& faces){
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
  static void classifyFaces(std::vector<ModelFace>& faces) {
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

};

int ModelFace::nbends = 0;
// TODO :
// Fill in all members of each class
// create bend class
// get bend face association
