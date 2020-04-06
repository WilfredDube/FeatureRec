#include "ModelBend.h"

namespace Fxt {

  ModelBend::ModelBend(const FaceID& faceID, TopoDS_Face& topoDSFace)
  {
    setFaceId(faceID);
    setTModelFace(topoDSFace);
    setPlaneType(PlaneType::NON_PLANAR);
    setFaceType(FaceType::BEND_FACE);
    setJoiningFaceID1(0);
    setJoiningFaceID2(0);

    init();
  }

  void ModelBend::init()
  {
    extractEdges();

    Standard_Real curvature = computeCurvature<TopoDS_Face>(getTModelFace());
    setCurvature(curvature);

    computeFaceNormal();
    setUnitNormal(computeUnitNormal(getTModelFace()));

    setBendLength(getFaceEdges());
  }

  void ModelBend::setJoiningFaceID1(FaceID mFaceID)
  {
    mJoinedFaceId1 = mFaceID;
  }

  Standard_Integer ModelBend::getJoiningFaceID1() { return mJoinedFaceId1; }

  void ModelBend::setJoiningFaceID2(FaceID mFaceID)
  {
    mJoinedFaceId2 = mFaceID;
  }

  Standard_Integer ModelBend::getJoiningFaceID2()
  {
    return mJoinedFaceId2;
  }

  void ModelBend::setBendAngle(long double angle)
  {
    mBendAngle = angle;
  }

  long double ModelBend::getBendAngle()
  {
    return mBendAngle;
  }

  Standard_Real ModelBend::getBendLength()
  {
    return mBendLength;
  }

  void ModelBend::setBendLength(std::vector<ModelEdge> mFaceEdges)
  {
    size_t i;
    for (i = 0; i < mFaceEdges.size(); i++) {
      if (mFaceEdges[i].getEdgeType() == EdgeType::LINE) {
        mBendLength = mFaceEdges[i].getEdgeLength();
        break;
      }
    }
  }

  /**
  * a normal member taking one argument to set the curvature attribute of the face. If the curvature != 0 then
  * the face is a bend face and so its radius is also set. *
  * @param cv a real/floating point.
  */
  void ModelBend::setCurvature(Standard_Real cv)
  {
    mCurvature = cv;

    if (mCurvature != 0.0) {
      // Divide by 2 to get radius else you get the diameter.
      mBendRadius = ((mCurvature < 0.0) ? -(1 / mCurvature) : (1 / mCurvature)) / 2;
    }
  }

  Standard_Real ModelBend::getBendRadius(){ return mBendRadius; }

}
