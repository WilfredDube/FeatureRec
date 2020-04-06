#include <TopoDS_Edge.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Adaptor3d_Curve.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <BRepAdaptor_Surface.hxx>
#include "MFace.h"
#include "ModelMath.h"

namespace Fxt {

  void MFace::setTModelFace(TopoDS_Face tface)
  {
    mModelFace = tface;
  }

  TopoDS_Face MFace::getTModelFace()
  {
    return mModelFace;
  }

  void MFace::setFaceId(FaceID faceID)
  {
    mFaceID = faceID;
  }

  FaceID MFace::getFaceId()
  {
    return mFaceID;
  }

  void MFace::setUnitNormal(gp_Dir unit_normal)
  {
    mFaceUnitNormal = unit_normal;
  }

  gp_Dir MFace::getUnitNormal()
  {
    return mFaceUnitNormal;
  }

  gp_Dir MFace::getFaceNormal()
  {
    return mFaceNormal;
  }

  void MFace::setFaceType(FaceType ftype)
  {
    mFaceType = ftype;
  }

  FaceType MFace::getFaceType(){ return mFaceType; }

  void MFace::setPlaneType(PlaneType ptype)
  {
    mPlaneType = ptype;
  }

  PlaneType MFace::getPlaneType()
  {
    return mPlaneType;
  }

  /**
  * computes and sets the normal vector of the face.
  */
  void MFace::computeFaceNormal()
  {
    mFaceNormal = computeNormal(mFaceEdges);
  }

  void MFace::addEdge(const ModelEdge& edge)
  {
    mFaceEdges.push_back(edge);
  }

  void MFace::extractEdges()
  {
    EdgeID edgeid = 0;

    for (TopExp_Explorer edgeEx((mModelFace), TopAbs_EDGE); edgeEx.More(); edgeEx.Next())
    {
      TopoDS_Edge edge = TopoDS::Edge(edgeEx.Current());
      ++edgeid;

      ModelEdge edgex(edge);
      edgex.setEdgeNum(edgeid);

      if (getPlaneType() == PlaneType::PLANAR) {
        if (edgex.getEdgeType() == EdgeType::ARC) {
          setFaceType(FaceType::BEND_SIDE);
        }
      }

      if (edgex.getEdgeType() == EdgeType::LINE) {
        if (getPlaneType() == PlaneType::NON_PLANAR) {
          edgex.setEdgePosition(EdgePosition::JOINING_EDGE);
        }
      }

      addEdge(edgex);
    }
  }

  std::vector<ModelEdge> MFace::getFaceEdges()
  {
    return mFaceEdges;
  }

  ModelEdge MFace::getFaceEdgesAt(int index)
  {
    return mFaceEdges[index];
  }

}
