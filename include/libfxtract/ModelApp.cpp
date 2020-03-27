#include "ModelApp.h"

ModelApp::ModelApp(Session& session): session_(session)
{}

int ModelApp::run(char const *modelFile, dbo::ptr<ModelFile> p)
{
  std::vector<ModelFace> vface;
  std::vector<TopoDS_Face> tvfaces;
  StepProcessor stepProcessor;
  IgesProcessor igesProcessor;
  ModelReaderClass mReader;
  // Session session_(connectionPool_);

  //loads file MyFile.igs
  std::string filename = modelFile;
  FileFormat fileFormat = mReader.checkFileFormat(modelFile);
  XSControl_Reader str = mReader.processModelFile(fileFormat, modelFile);

  Standard_Integer nbs = str.NbShapes();
  std::cout << "nbs : " << nbs << '\n';

  switch (fileFormat) {
    case FileFormat::IGES_FORMAT:
    igesProcessor.extractFeactures(str);
    vface = igesProcessor.getModelFaces();
    break;
    case FileFormat::STEP_FORMAT:
    stepProcessor.extractFeactures(str);
    vface = stepProcessor.getModelFaces();
    break;
    default:
    std::cout << "Unknown file format" << '\n';
  }

  ModelFace::classifyBS_BF(vface);
  ModelFace::classifyFaces(vface);
  ModelFace::classifyBends(vface);

  // std::cout << "Face iiiiid: " << vface[0].getFaceId() << '\n';

  size_t count = vface.size();

  std::sort(vface.begin(), vface.end(), [ ]( const auto& lhs, const auto& rhs )
  {
    return lhs.Radius < rhs.Radius;
  });

  size_t nbends = 0;
  for (size_t i = 0; i < count; i++) {
    if (vface[i].getFaceType() == FaceType::BEND_FACE) {
      // for (size_t j = 0; j < nbends/2; j++) {
      //   vface[i + j].setIsInternal(true);
      //   // std::cerr << j << '\n';
      // }
      // break;
      ++nbends;
    }
  }

  dbo::Transaction transaction(session_);
  for (size_t i = 0; i < count; i++) {
    if (vface[i].getFaceType() == FaceType::BEND_FACE) {
      if (vface[i].bend_type == BendType::INTERNAL) {
        gp_Pnt firstnorm = vface[i].getNormal(vface[i].getJoiningFaceID1(), vface);
        gp_Pnt secondnorm = vface[i].getNormal(vface[i].getJoiningFaceID2(), vface);
        vface[i].setBendAngle(compute_angle(firstnorm, secondnorm));
        std::cout << "F"<< vface[i].getJoiningFaceID1() << "---B" << vface[i].getFaceId() << "---F" <<vface[i].getJoiningFaceID2()<< " = ";
        std::cout << ", Angle : " << vface[i].getBendAngle();
        std::cout << ", Radius : " << vface[i].getFaceRadius();
        // std::cout << ", Dot product: " << compute_dot_product(&gp_Pnt(0, 1, 0), &gp_Pnt(3.60355e-16, -1, 0));
        std::cout << ", Bend Length: " << vface[i].getBendLength()<< " mm, ";
        vface[i].printUnitNormal();

        {
          dbo::ptr<BendFeature> BF(Wt::cpp14::make_unique<BendFeature>());
          // std::unique_ptr<ModelFile> MF{new ModelFile()};
          BendFeature *b = BF.modify();

          b->bend_id = vface[i].getFaceId();
          b->face_id1 = vface[i].getJoiningFaceID1();
          b->face_id2 =  vface[i].getJoiningFaceID2();
          b->bend_angle = roundd(vface[i].getBendAngle());
          b->bend_radius = roundd(vface[i].getFaceRadius());
          b->bend_thickness = roundd(ModelFace::thickness);
          b->bend_length = roundd(vface[i].getBendLength());
          b->modelFile = p;

          p.modify()->nbends = nbends / 2;


          session_.add(BF);
          //
          // ModelApp::run(saveName.c_str(), p);
        }
        // }
      }
    }
    session_.flush();
    transaction.commit();

    // findConnection(list);
    // std::vector<int> R1;
    // queue<FRlnship> Q;
    // preFAG(list);
    // // FAG(list, R1, Q, 0, list[0].face_id1);
    //
    // for (auto frl : R1){
    //   std::cout << frl << " - ";
    // }
    //
    // std::cout << '\n';

    std::cout << "SIZE : " <<vface.size()<< '\n';
    std::cout << "SIZE : " << list.size()<< '\n';
    std::cout << "File name : " << modelFile << endl;
    std::cout << "Thickness : " << std::setprecision(3) << ModelFace::thickness << " mm" << '\n';
    std::cout << "# of Bends: " << ModelFace::nbends / 2 << '\n';

    return 0;
  }
