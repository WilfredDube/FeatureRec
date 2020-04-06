#ifndef MODEL_BEND_H_
#define MODEL_BEND_H_

#include <TopoDS_Face.hxx>
#include <vector>
#include "ModelEdge.h"
#include "ModelUtils.h"
#include "MFace.h"
#include "ModelMath.h"

namespace Fxt {

  class ModelBend : public MFace
  {
    Standard_Real mBendLength;
    Standard_Real mCurvature; //!< face curvature value : zero for planar type faces and non-zero for non-planar type faces.

    //!< the face ID of the face connected to the bend face via its Straight edge.
    Standard_Integer mJoinedFaceId1;

    //!< the face ID of the second face connected to the bend face via its Straight edge.
    Standard_Integer mJoinedFaceId2;

    Standard_Real mBendingForce; //!< force required to create the bend.
    Standard_Real mBendAngle; //!< bend angle.
    unsigned long mBendSameAsId;
    BendType mBendType;
    Standard_Real mBendRadius; //!< bend radius : zero if face is planar and non-zero if face is a bend.
  public:

    ModelBend(const FaceID& faceID, TopoDS_Face& topoDSFace);
    void init();

    void setJoiningFaceID1(FaceID mFaceID);
    Standard_Integer getJoiningFaceID1();

    void setJoiningFaceID2(FaceID mFaceID);
    Standard_Integer getJoiningFaceID2();

    void setBendAngle(long double angle);
    long double getBendAngle();

    Standard_Real getBendLength();
    void setBendLength(std::vector<ModelEdge> mFaceEdges);

    void setCurvature(Standard_Real cv);
    Standard_Real getBendRadius();

    // TODO : calculate Bending force, direction, orientation & distance
  };
}

#endif // MODEL_FACE_H_
