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
#include <Geom_BSplineCurve.hxx>
#include <Adaptor3d_Curve.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <vector>
#include "ModelMath.h"
#include "ModelVertex.h"

enum class PlaneType { PLANAR, NON_PLANAR };
enum class FaceType { NONE, THICKNESS_DEFINING_FACE, BEND_FACE, BEND_SIDE, FACE };
enum class BendType { UNASSIGNED = -1, INTERNAL, EXTERNAL };

class ModelFace : public TopoDS_Face{
  Standard_Integer face_id; /*!< an integer value  represents the Face Identification number */
  PlaneType plane_type;
  FaceType face_type;
  Standard_Real Curvature; //!< face curvature value : zero for planar type faces and non-zero for non-planar type faces.
  Standard_Real D_element; //!< represents the D value in the equation of the plane. Ax+By+Cz = D
  gp_Pnt face_normal; //!< the normal vector to the face.
  gp_Dir face_unit_normal; //!< the unit normal vector to the face.
  bool taken;
  bool isInternal;
  Standard_Real bend_length;

  Standard_Integer joined_to_faceid1; //!< the face ID of the face connected to the bend face.
  Standard_Integer joined_to_faceid2; //!< the face ID of the second face connected to the bend face.
  Standard_CString parallel_to_faceid; //!< the face ID of the face parallel to this one.

  // Bend face attributes
  ModelEdge imaginary_bend_line;
  Standard_Real bending_force; //!< force required to create the bend.
  Standard_Real bend_angle; //!< bend angle.
  unsigned long bend_same_as_id;

  std::vector<ModelEdge> face_edges; //!< edges of the face.
  std::vector<ModelEdge> parallel_face_edges; //!< edges of the parallel face.
  std::vector<ModelEdge> central_face_edges; //!< edges of the central face. code for generating the central face not yet available.
  size_t num_edges; //!< number of edges
public:
  BendType bend_type;
  static int nbends;
  static Standard_Real thickness; //!< thickness of the face.
  Standard_Real Radius; //!< bend radius : zero if face is planar and non-zero if face is a bend.

  ModelFace(Standard_Integer fid, PlaneType planetype);
  ModelFace();
  ~ModelFace(){}

  void setIsInternal(bool is_internal) ;
  bool getIsInternal();

  void setJoiningFaceID1(Standard_Integer face_id);
  Standard_Integer getJoiningFaceID1();

  void setJoiningFaceID2(Standard_Integer face_id);
  Standard_Integer getJoiningFaceID2();

  void setBendAngle(long double angle);
  long double getBendAngle();

  Standard_Real getBendLength();
  void setBendLength(std::vector<ModelEdge> face_edges);

  void setCurvature(Standard_Real cv);

  Standard_Real getFaceRadius();

  void setFaceId(Standard_Integer fid);
  Standard_Integer getFaceId();

  void setUnitNormal(gp_Dir unit_normal);
  gp_Pnt getNormal(Standard_Integer fid, std::vector<ModelFace> vface);

  void computeFaceEquation();
  gp_Pnt getFaceNormal();

  void setFaceType(FaceType ftype);
  FaceType getFaceType();

  void setPlaneType(PlaneType ptype);
  PlaneType getPlaneType();

  void printUnitNormal();
  void printNormal();

  void addEdge(const ModelEdge& n);

  void extractEdges(TopoDS_Face face);
  void setNumEdges(size_t nEdges);
  size_t getNumEdges();
  std::vector<ModelEdge> getFaceEdges();
  void computeFaceNormal();

  static void classifyBS_BF(std::vector<ModelFace>& faces);
  static void classifyFaces(std::vector<ModelFace>& faces);
  static void classifyBends(std::vector<ModelFace>& v);
};
// TODO :
// Fill in all members of each class
// create bend class
// get bend face association
