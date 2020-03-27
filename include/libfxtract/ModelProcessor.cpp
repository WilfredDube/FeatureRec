#include "ModelProcessor.h"

namespace Fxt {

  void ModelProcessor::addFace(ModelFace& face) {  vface.push_back(face); }
  void ModelProcessor::addTopoDSFace(TopoDS_Face face) {  tvfaces.push_back(face); }
  std::vector<ModelFace> ModelProcessor::getModelFaces(){ return vface; }
}
