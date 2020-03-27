#include "ModelApp.h"
#include <string>
#include <queue>
#include <deque>

const Standard_Real MODULE_LENGTH = 1000.0;

struct FRlnship{
  int face_id1;
  int bend_id;
  int face_id2;

  bool empty(){
    if (face_id1 == -1 && face_id2 == -1 && bend_id == -1) {
      return true;
    }

    return false;
  }

  bool operator==(const FRlnship &rhs) const{
    return rhs.bend_id == bend_id;
  }
};

int findQ(int query, deque<FRlnship> Q);
FRlnship findFace(int query, std::vector<FRlnship> BL);

void preFAG(std::vector<FRlnship>& BL){
  std::vector<FRlnship> L;
  FRlnship firstObj = BL[0], found;
  deque<FRlnship> Q;

  Q.push_back(BL[0]);

  std::cout << "0. SIZE : " << BL.size() << '\n';
  auto it = std::find(BL.begin(), BL.end(), firstObj);
  if (it != BL.end()) {
    BL.erase(it);
  }

  int search = Q.front().face_id1;

  while (!BL.empty()) {
    found = findFace(search, BL);

    if ((found.face_id1 == Q.front().face_id1) || (found.face_id2 == Q.front().face_id1)) {
      Q.push_front(found);
      auto it = std::find(BL.begin(), BL.end(), found);
      if (it != BL.end()) {
        BL.erase(it);
      }
      //
      std::cout << "1. SIZE : " << Q.size() << '\n';
      search = found.face_id2;
      if ((found.face_id2 == Q.front().face_id1)) {
        search = found.face_id1;
      }
    } else if ((found.face_id2 == Q.front().face_id2) || (found.face_id1 == Q.front().face_id2)) {
      Q.push_back(found);
      auto it = std::find(BL.begin(), BL.end(), found);
      if (it != BL.end()) {
        BL.erase(it);
      }
      std::cout << "2. SIZE : " << BL.size() << '\n';
      search = found.face_id2;
      if ((found.face_id1 == Q.front().face_id2)) {
        search = found.face_id2;
      }
    } else {
      if (Q.empty() && !BL.empty()) {
        L.push_back({-1, -1, -1});
        Q.push_back(BL[0]);
        //
        // // std::cout << "BL[0] : " << BL[0].face_id1 << ", " << BL[0].bend_id << ", " << BL[0].face_id2 << '\n';
        auto it = std::find(BL.begin(), BL.end(), BL[0]);
        if (it != BL.end()) {
          BL.erase(it);
        }
        //
        search = Q.front().face_id1;
      } else {
        if (!Q.empty()) {
          if (search != Q.front().face_id2) {
            search = Q.front().face_id2;
            FRlnship value = Q.front();
            L.push_back(value);
            Q.pop_front();
            std::cout << "3. SIZE : " << BL.size() << " Q = " << Q.size() << " Search : " << value.face_id1 << '\n';
            // std::cerr << "/* error message */ " << search << '\n';
          } else {
            search = findQ(search, Q);

            if (-1 == search) {
              search = Q.front().face_id1;
            }

            FRlnship value = Q.front();
            L.push_back(value);
            // Q.pop_front();
          }
        } else{
          std::cerr << "/* error message */" << '\n';
          if (!BL.empty()) {
            break;
            Q.push_front(BL[0]);
            search = Q.front().face_id1;

            auto it = std::find(BL.begin(), BL.end(), BL[0]);
            if (it != BL.end()) {
              BL.erase(it);
            }
          }
        }
      }
    }
  }

  for (size_t i = 0; i < L.size(); i++) {
    std::cout << "F" << L[i].face_id1 << '\n';
    std::cout << "B" << L[i].bend_id << '\n';
    std::cout << "F" << L[i].face_id2 << '\n';

    // if ((L[i].face_id2 == -1) && (L[i + 1] != NULL)) {
    //   std::cout << "F" << L[i + 1].face_id1 << '\n';
    // }
  }


}

void FAG(std::vector<FRlnship>& temp, std::vector<int>& R1, queue<FRlnship> Q, unsigned long index, int search) {

  if (Q.empty() && temp.empty()) {
    return;
  } else {
    // std::string ID {"F" + std::to_string(search)};
    R1.push_back(search);

    std::cout << "MSG : " << search << '\n';
    auto found = findFace(search, temp);

    if (found.empty()){
      if (temp.empty()) {
        return;
      }

      std::cout << "Q.front().face_id2 == search : " << Q.front().face_id2 <<  '\n';
      if (Q.front().face_id2 == search) {
        Q.pop();
        search = temp[index].face_id1;
        // ID = "NEXT";
        R1.push_back(-1000);
        // Q.push(temp[index]);
        // auto it = std::find(temp.begin(), temp.end(), temp[index]);
        // if (it != temp.end()) {
        //   temp.erase(it);
        // }
        index = 0;
        FAG(temp, R1, Q, index, search);

      } else {
        search = temp[index].face_id2;
        // ID = "B" + temp[index].bend_id;
        R1.push_back(temp[index].bend_id);
        Q.push(temp[index]);

        auto it = std::find(temp.begin(), temp.end(), temp[index]);
        if (it != temp.end()) {
          temp.erase(it);
        }

        index = 0;
        FAG(temp, R1, Q, index, search);
      }
    } else {
      Q.pop();
      R1.push_back(temp[index].bend_id);
      Q.push(found);

      if (search == found.face_id2) {
        /* code */
      }
      search = found.face_id2;
      // ID = "B" + found.bend_id;

      R1.push_back(found.bend_id);

      auto it = std::find(temp.begin(), temp.end(), found);
      if (it != temp.end()) {
        temp.erase(it);
      }

      index = 0;
      FAG(temp, R1, Q, index, search);
    }

    if (Q.empty() && !temp.empty()) {
      // ID = "NEXT";
      R1.push_back(-10000);
      FAG(temp, R1, Q, index, search);
    }
  }

}

int findQ(int query, deque<FRlnship> Q){
  int found = -1;

  // std::cout << "Found : " << found.face_id1 << '\n';

  for (auto frl : Q){
    if (query == frl.face_id1) {
      found = frl.face_id2;
    } else if (query == frl.face_id2) {
      found = frl.face_id1;
    }
  }

  return found;
}

FRlnship findFace(int query, std::vector<FRlnship> BL){
  FRlnship found {-1, -1, -1};

  // std::cout << "Found : " << found.face_id1 << '\n';

  for (auto frl : BL){
    if (query == frl.face_id1) {
      found = frl;
    } else if (query == frl.face_id2) {
      found = frl;
    }
  }

  return found;
}

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

    // TODO : Find the longest bend length to find the number of required modules
    Standard_Real longest = 0.0;
    for (size_t i = 0; i < count; i++) {
      if (vface[i].getFaceType() == FaceType::BEND_FACE) {
        if (longest < roundd(vface[i].getBendLength())) {
          longest = roundd(vface[i].getBendLength());
          std::cout << "length : " << longest << '\n';
        }
      }
    }

    std::cout << "longest length : " << longest << '\n';

    vector<FRlnship> list;
    dbo::Transaction transaction(session_);
    for (size_t i = 0; i < count; i++) {
      if (vface[i].getFaceType() == FaceType::BEND_FACE) {
        // if (vface[i].bend_type == BendType::INTERNAL) {
        gp_Pnt firstnorm = vface[i].getNormal(vface[i].getJoiningFaceID1(), vface);
        gp_Pnt secondnorm = vface[i].getNormal(vface[i].getJoiningFaceID2(), vface);
        vface[i].setBendAngle(compute_angle(firstnorm, secondnorm));
        std::cout << "F"<< vface[i].getJoiningFaceID1() << "---B" << vface[i].getFaceId() << "---F" <<vface[i].getJoiningFaceID2()<< " = ";

        struct FRlnship rln;
        rln.face_id1 = vface[i].getJoiningFaceID1();
        rln.bend_id = vface[i].getFaceId();
        rln.face_id2 = vface[i].getJoiningFaceID2();

        list.push_back(rln);

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

          int num_of_modules = 0;
          if (longest > MODULE_LENGTH){
            num_of_modules = longest / MODULE_LENGTH;
            if ((static_cast<int>(longest) % static_cast<int>(MODULE_LENGTH)) > 0) {
              ++num_of_modules;
            }
          } else {
            num_of_modules = 1;
          }
          p.modify()->nbends = nbends / 2;
          p.modify()->number_of_modules = num_of_modules;

          typedef dbo::collection< dbo::ptr<Tool> > Tools;
          // dbo::ptr<Tool> joe = session_.query<dbo::ptr<Tool>>("select u from tools u").where("tool_angle = " + std::to_string(b->bend_angle)).bind("Joe");
          dbo::collection<dbo::ptr<Tool>> joe = session_.find<Tool>().where("tool_angle = ?").bind(b->bend_angle);
          for (Tools::const_iterator i = joe.begin(); i != joe.end(); ++i){
            dbo::ptr<Tool> tool = *i;
            std::cout << "Tool ID : " << tool->tool_id << " Angle : " << tool->tool_angle << '\n';
            b->bending_tool_id = tool->tool_id;
            break;
          }


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
