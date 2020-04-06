#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include <memory>
#include <TopoDS_Face.hxx>
#include "MFace.h"
#include "ModelFace.h"
#include "ModelBend.h"

namespace Fxt {

  class Model
  {
  public:
    Model(const char* modelFile);

    void init();

    std::vector<ModelFace> getFaces();
    std::vector<ModelBend> getBends();

    ModelFace getFacesAt(int index);

    void assignFaceAttributes(FaceID faceID, TopoDS_Shape& aShape);

    void classifyFaces();

    void computeBendAngles();
  void printBendInfo();

  private:
    const char* mModelFile;

    //!< thickness of the model sheetmetal.
    Standard_Real mThickness;
    
    std::vector<ModelFace> mModelFaces;
    std::vector<ModelBend> mModelBends;

    std::vector<FaceID> mFaceAdjacencyGraph;
    TopoDS_Shape mModelShape;

    gp_Dir getNormal(FaceID faceID);

    void addFace(ModelFace modelFace);
    void addBend(ModelBend modelFace);
  };

}

#endif
