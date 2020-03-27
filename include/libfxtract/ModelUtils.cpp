#include "ModelUtils.h"

namespace Fxt {

  void printShapeType(TopAbs_ShapeEnum shapetype)
  {
    switch (shapetype) {
      case TopAbs_COMPOUND : std::cout << "TopAbs_COMPOUND" << '\n';
      break;
      case TopAbs_COMPSOLID : std::cout << "TopAbs_COMPSOLID" << '\n';
      break;
      case TopAbs_SOLID : std::cout << "TopAbs_SOLID" << '\n';
      break;
      default:
      std::cout << "NOTHING" << '\n';
    }
  }

  void printType(GeomAbs_CurveType curvetype)
  {
    switch (curvetype) {
      case GeomAbs_Line:std::cout << "GeomAbs_Line" << '\n';
      break;
      case GeomAbs_Circle:std::cout << "GeomAbs_Circle" << '\n';
      break;
      case GeomAbs_BSplineCurve:std::cout << "GeomAbs_BsplineCurve" << '\n';
      break;
      default:
      std::cout << "NOTHING" << '\n';
    }
  }

  void printPrecision(){
    Standard_Real rp = Interface_Static::RVal("read.precision.val");
    Standard_Integer ic = Interface_Static::IVal("read.precision.mode");
    Standard_Integer mv = Interface_Static::IVal("read.maxprecision.mode");

    std::cout << "Precision value : " << rp << '\n';
    std::cout << "Precision mode : " << ic << '\n';
    std::cout << "Max precision mode : " << mv << '\n';
  }


  //
  // struct FRlnship{
  //   int face_id1;
  //   int bend_id;
  //   int face_id2;
  //
  //   bool empty(){
  //     if (face_id1 == -1 && face_id2 == -1 && bend_id == -1) {
  //       return true;
  //     }
  //
  //     return false;
  //   }
  //
  //   bool operator==(const FRlnship &rhs) const{
  //     return rhs.bend_id == bend_id;
  //   }
  // };
  //
  // int findQ(int query, deque<FRlnship> Q);
  // FRlnship findFace(int query, std::vector<FRlnship> BL);
  //
  // void preFAG(std::vector<FRlnship>& BL){
  //   std::vector<FRlnship> L;
  //   FRlnship firstObj = BL[0], found;
  //   deque<FRlnship> Q;
  //
  //   Q.push_back(BL[0]);
  //
  //   std::cout << "0. SIZE : " << BL.size() << '\n';
  //   auto it = std::find(BL.begin(), BL.end(), firstObj);
  //   if (it != BL.end()) {
  //     BL.erase(it);
  //   }
  //
  //   int search = Q.front().face_id1;
  //
  //   while (!BL.empty()) {
  //     found = findFace(search, BL);
  //
  //     if ((found.face_id1 == Q.front().face_id1) || (found.face_id2 == Q.front().face_id1)) {
  //       Q.push_front(found);
  //       auto it = std::find(BL.begin(), BL.end(), found);
  //       if (it != BL.end()) {
  //         BL.erase(it);
  //       }
  //       //
  //       std::cout << "1. SIZE : " << Q.size() << '\n';
  //       search = found.face_id2;
  //       if ((found.face_id2 == Q.front().face_id1)) {
  //         search = found.face_id1;
  //       }
  //     } else if ((found.face_id2 == Q.front().face_id2) || (found.face_id1 == Q.front().face_id2)) {
  //       Q.push_back(found);
  //       auto it = std::find(BL.begin(), BL.end(), found);
  //       if (it != BL.end()) {
  //         BL.erase(it);
  //       }
  //       std::cout << "2. SIZE : " << BL.size() << '\n';
  //       search = found.face_id2;
  //       if ((found.face_id1 == Q.front().face_id2)) {
  //         search = found.face_id2;
  //       }
  //     } else {
  //       if (Q.empty() && !BL.empty()) {
  //         L.push_back({-1, -1, -1});
  //         Q.push_back(BL[0]);
  //         //
  //         // // std::cout << "BL[0] : " << BL[0].face_id1 << ", " << BL[0].bend_id << ", " << BL[0].face_id2 << '\n';
  //         auto it = std::find(BL.begin(), BL.end(), BL[0]);
  //         if (it != BL.end()) {
  //           BL.erase(it);
  //         }
  //         //
  //         search = Q.front().face_id1;
  //       } else {
  //         if (!Q.empty()) {
  //           if (search != Q.front().face_id2) {
  //             search = Q.front().face_id2;
  //             FRlnship value = Q.front();
  //             L.push_back(value);
  //             Q.pop_front();
  //             std::cout << "3. SIZE : " << BL.size() << " Q = " << Q.size() << " Search : " << value.face_id1 << '\n';
  //             // std::cerr << "/* error message */ " << search << '\n';
  //           } else {
  //             search = findQ(search, Q);
  //
  //             if (-1 == search) {
  //               search = Q.front().face_id1;
  //             }
  //
  //             FRlnship value = Q.front();
  //             L.push_back(value);
  //             // Q.pop_front();
  //           }
  //         } else{
  //           std::cerr << "/* error message */" << '\n';
  //           if (!BL.empty()) {
  //             break;
  //             Q.push_front(BL[0]);
  //             search = Q.front().face_id1;
  //
  //             auto it = std::find(BL.begin(), BL.end(), BL[0]);
  //             if (it != BL.end()) {
  //               BL.erase(it);
  //             }
  //           }
  //         }
  //       }
  //     }
  //   }
  //
  //   for (size_t i = 0; i < L.size(); i++) {
  //     std::cout << "F" << L[i].face_id1 << '\n';
  //     std::cout << "B" << L[i].bend_id << '\n';
  //     std::cout << "F" << L[i].face_id2 << '\n';
  //
  //     // if ((L[i].face_id2 == -1) && (L[i + 1] != NULL)) {
  //     //   std::cout << "F" << L[i + 1].face_id1 << '\n';
  //     // }
  //   }
  //
  //
  // }
  //
  // void FAG(std::vector<FRlnship>& temp, std::vector<int>& R1, queue<FRlnship> Q, unsigned long index, int search) {
  //
  //   if (Q.empty() && temp.empty()) {
  //     return;
  //   } else {
  //     // std::string ID {"F" + std::to_string(search)};
  //     R1.push_back(search);
  //
  //     std::cout << "MSG : " << search << '\n';
  //     auto found = findFace(search, temp);
  //
  //     if (found.empty()){
  //       if (temp.empty()) {
  //         return;
  //       }
  //
  //       std::cout << "Q.front().face_id2 == search : " << Q.front().face_id2 <<  '\n';
  //       if (Q.front().face_id2 == search) {
  //         Q.pop();
  //         search = temp[index].face_id1;
  //         // ID = "NEXT";
  //         R1.push_back(-1000);
  //         // Q.push(temp[index]);
  //         // auto it = std::find(temp.begin(), temp.end(), temp[index]);
  //         // if (it != temp.end()) {
  //         //   temp.erase(it);
  //         // }
  //         index = 0;
  //         FAG(temp, R1, Q, index, search);
  //
  //       } else {
  //         search = temp[index].face_id2;
  //         // ID = "B" + temp[index].bend_id;
  //         R1.push_back(temp[index].bend_id);
  //         Q.push(temp[index]);
  //
  //         auto it = std::find(temp.begin(), temp.end(), temp[index]);
  //         if (it != temp.end()) {
  //           temp.erase(it);
  //         }
  //
  //         index = 0;
  //         FAG(temp, R1, Q, index, search);
  //       }
  //     } else {
  //       Q.pop();
  //       R1.push_back(temp[index].bend_id);
  //       Q.push(found);
  //
  //       if (search == found.face_id2) {
  //         /* code */
  //       }
  //       search = found.face_id2;
  //       // ID = "B" + found.bend_id;
  //
  //       R1.push_back(found.bend_id);
  //
  //       auto it = std::find(temp.begin(), temp.end(), found);
  //       if (it != temp.end()) {
  //         temp.erase(it);
  //       }
  //
  //       index = 0;
  //       FAG(temp, R1, Q, index, search);
  //     }
  //
  //     if (Q.empty() && !temp.empty()) {
  //       // ID = "NEXT";
  //       R1.push_back(-10000);
  //       FAG(temp, R1, Q, index, search);
  //     }
  //   }
  //
  // }
  //
  // int findQ(int query, deque<FRlnship> Q){
  //   int found = -1;
  //
  //   // std::cout << "Found : " << found.face_id1 << '\n';
  //
  //   for (auto frl : Q){
  //     if (query == frl.face_id1) {
  //       found = frl.face_id2;
  //     } else if (query == frl.face_id2) {
  //       found = frl.face_id1;
  //     }
  //   }
  //
  //   return found;
  // }
  //
  // FRlnship findFace(int query, std::vector<FRlnship> BL){
  //   FRlnship found {-1, -1, -1};
  //
  //   // std::cout << "Found : " << found.face_id1 << '\n';
  //
  //   for (auto frl : BL){
  //     if (query == frl.face_id1) {
  //       found = frl;
  //     } else if (query == frl.face_id2) {
  //       found = frl;
  //     }
  //   }
  //
  //   return found;
  // }

}
