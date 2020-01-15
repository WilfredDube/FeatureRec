#ifndef BEND_FEATURE_
#define BEND_FEATURE_

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "ModelFile.h"
class ModelFile;
// namespace std;
namespace dbo = Wt::Dbo;

class BendFeature {
public:
  int bend_id;
  int face_id1;
  int face_id2;
  double bend_angle;
  double bend_radius;
  double bend_thickness;
  double bend_length; // error when type is long double
  // vector<string> bend_parallelTo;
  // vector<string> bend_pependicularTo;

  dbo::ptr<ModelFile> modelFile;

  template<class Action>
  void persist(Action& a) {
    // dbo::id(a, bend_id, "bend_id", 20);
    dbo::field(a, bend_id, "bend_id");
    dbo::field(a, face_id1, "face_id1");
    dbo::field(a, face_id2, "face_id2");
    dbo::field(a, bend_angle, "bend_angle");
    dbo::field(a, bend_radius, "bend_radius");
    dbo::field(a, bend_thickness, "bend_thickness");
    dbo::field(a, bend_length, "bend_length");

    dbo::belongsTo(a, modelFile, "model_file", dbo::OnDeleteCascade);
  }
};

// namespace Wt {
//   namespace Dbo {
//     template<>
//     struct dbo_traits<BendFeature> : public dbo_default_traits {
//       typedef std::string IdType;
//       static IdType invalidId() {
//         return std::string();
//       }
//       static const char *surrogateIdField() { return 0; }
//     };
//   }
// }

DBO_EXTERN_TEMPLATES(BendFeature)

#endif
