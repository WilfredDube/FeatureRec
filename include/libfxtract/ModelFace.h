#ifndef MODEL_FACE_H_
#define MODEL_FACE_H_

#include "MFace.h"
#include <TopoDS_Face.hxx>

namespace Fxt {

  class ModelFace:  public MFace
  {
  public:
    ModelFace(FaceID faceID, TopoDS_Face& topoDSFace);

    void init();
  };
}

#endif // MODEL_FACE_H_
