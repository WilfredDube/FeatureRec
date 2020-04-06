  #ifndef MFACE_H
  #define MFACE_H

  #include <TopoDS_Face.hxx>
  #include <vector>
  #include "ModelUtils.h"
  #include "ModelEdge.h"

  namespace Fxt {

    class MFace
    {
      FaceID mFaceID; /*!< an integer value  represents the Face Identification number */
      PlaneType mPlaneType;
      FaceType mFaceType;
      gp_Dir mFaceNormal; //!< the normal vector to the face.
      gp_Dir mFaceUnitNormal; //!< the unit normal vector to the face.

      std::vector<ModelEdge> mFaceEdges; //!< edges of the face.

      TopoDS_Face mModelFace;
    public:

      MFace(){}

      void setTModelFace(TopoDS_Face tface);
      TopoDS_Face getTModelFace();

      void setFaceId(FaceID faceID);
      FaceID getFaceId();

      void setUnitNormal(gp_Dir unit_normal);
      gp_Dir getUnitNormal();

      gp_Dir getFaceNormal();
      void computeFaceNormal();

      void setFaceType(FaceType ftype);
      FaceType getFaceType();

      void setPlaneType(PlaneType ptype);
      PlaneType getPlaneType();

      void extractEdges();

      void addEdge(const ModelEdge& edge);

      std::vector<ModelEdge> getFaceEdges();

      ModelEdge getFaceEdgesAt(int index);
    };

  }

  #endif // MFACE_H
