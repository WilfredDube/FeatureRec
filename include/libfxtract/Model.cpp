#include "Model.h"
#include "ModelMath.h"
#include "ModelReader.h"


#include <TopoDS_Shape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_ShapeMapHasher.hxx>
#include <NCollection_IndexedDataMap.hxx>
#include <TopOpeBRepBuild_Tools.hxx>
#include <TopExp.hxx>

namespace Fxt {

  Model::Model(const char* modelFile)
  {
    mModelFile = modelFile;
  }

  void Model::init()
  {
    auto mReader = make_unique<ModelReader>(*this);
    std::cerr << "File name : " << mModelFile << '\n';
    mReader->processModel(mModelFile);
  }

  void Model::addBend(ModelBend modelbend)
  {
    mModelBends.push_back(modelbend);
  }

  std::vector<ModelBend> Model::getBends()
  {
    return mModelBends;
  }

  void Model::addFace(ModelFace face)
  {
    mModelFaces.push_back(face);
  }

  void Model::assignFaceAttributes(const FaceID faceID, TopoDS_Shape& aShape)
  {
    mModelShape = aShape;

    TopoDS_Face pTopoDSFace = TopoDS::Face(mModelShape);

    Standard_Real curvature = computeCurvature(pTopoDSFace);

    if (curvature == 0.0){

      ModelFace pModelFace(faceID, pTopoDSFace);

      addFace(pModelFace);

    } else {

      ModelBend pModelFace(faceID, pTopoDSFace);

      addBend(pModelFace);
    }
  }

  void Model::classifyFaces()
  {
    std::cout << "Number of plane faces : " << mModelFaces.size() << '\n';
    std::cout << "Number of bends : " << mModelBends.size() << '\n';

    for (auto& bend: mModelBends){
      for (auto& edge: bend.getFaceEdges()){
        if (edge.getEdgeType() == EdgeType::LINE) {
          TopoDS_Edge anEdge = edge.getTModelEdge();

          for (auto& face: mModelFaces)
          {
            if (face.getFaceType() == FaceType::NONE) {
              TopoDS_Face aFace = face.getTModelFace();
              for( TopExp_Explorer anExp(aFace, TopAbs_EDGE); anExp.More(); anExp.Next()) {
                if(anExp.Current().IsPartner(anEdge)) {
                  face.setFaceType(FaceType::FACE);
                  if ((bend.getJoiningFaceID1() == 0) && (bend.getJoiningFaceID2() == 0)) {
                    bend.setJoiningFaceID1(face.getFaceId());
                  } else if (bend.getJoiningFaceID2() == 0){
                    bend.setJoiningFaceID2(face.getFaceId());
                  } else {
                  }
                }
              }
            }
          }
        }
      }
    }

    for (auto& a: mModelFaces) {
      if (a.getFaceType() == FaceType::NONE) {
        a.setFaceType(FaceType::THICKNESS_DEFINING_FACE);
      }
    }

    std::vector<ModelFace> bs, fc, tdf;
    for (auto& a: mModelFaces) {
      if (a.getFaceType() == FaceType::THICKNESS_DEFINING_FACE){
        mThickness = computeThickness(a);
        break;
      }
    }

    std::cout << "Number of Faces : " << fc.size() << '\n';
    std::cout << "Number of Bs : " << bs.size() << '\n';
    std::cout << "Number of THF : " << tdf.size() << '\n';
  }

  std::vector<ModelFace> Model::getFaces()
  {
    return mModelFaces;
  }

  void Model::computeBendAngles()
  {
    for (auto& bend : mModelBends){
      if ((bend.getJoiningFaceID1() != 0) && (bend.getJoiningFaceID2() != 0)) {
        gp_Dir fn1 = getNormal(bend.getJoiningFaceID1());
        gp_Dir fn2 = getNormal(bend.getJoiningFaceID2());

        bend.setBendAngle(round(fn1.Angle(fn2) * (180.0 / PI)));
      }
    }
  }

  void Model::printBendInfo()
  {
    std::cout << "\n=============================================" << '\n';
    std::cout << "Thickness : " << mThickness << " mm" << '\n';
    for (auto& a: mModelBends) {
      std::cout << "F"<< a.getJoiningFaceID1() <<
      "---B" << a.getFaceId() <<
      "---F" << a.getJoiningFaceID2()<<
      " Angle : " << a.getBendAngle() <<
      " Radius : " << a.getBendRadius() << "\n";
    }
  }

  gp_Dir Model::getNormal(FaceID faceID)
  {
    gp_Dir vt;
    for (auto& face : mModelFaces) {
      if (face.getFaceId() == faceID) {
        vt = face.getFaceNormal();
      }
    }
    return vt;
  }
}
