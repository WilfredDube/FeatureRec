#pragma once

#include <Wt/WDate.h>
#include <string>
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>
#include <Wt/Dbo/WtSqlTraits.h>
#include "BendFeature.h"
class BendFeature;
// class User;
// class Project;
// namespace std;
namespace dbo = Wt::Dbo;

typedef dbo::collection< dbo::ptr<BendFeature> > BendFeatures;

class ModelFile {
public:
  ModelFile(){}
  ~ModelFile(){}
  // enum Material {
  // };

  // dbo::ptr<Project> project;
  // dbo::ptr<User> creator;

  // std::string model_id;
  double tensile_strength;
  int nbends;
  std::string modelFileLocation;
  Wt::WDateTime  uploadDate;

  BendFeatures bendFeatures;

  template<class Action>
  void persist(Action& a) {
    // dbo::id(a, model_id, "model_id", 20);
    dbo::field(a, tensile_strength, "model_tensile_strength");
    dbo::field(a, nbends, "model_bend_count");
    dbo::field(a, modelFileLocation, "model_file_location");
    dbo::field(a, uploadDate, "model_upload_date");

    // dbo::belongsTo(a, project, "project_id", dbo::OnDeleteCascade);
    // dbo::belongsTo(a, creator, "user_id", dbo::OnDeleteCascade);
    //
    dbo::hasMany(a, bendFeatures, dbo::ManyToOne, "model_file");
  }
};

// namespace Wt {
//   namespace Dbo {
//     template<>
//     struct dbo_traits<ModelFile> : public dbo_default_traits {
//       typedef std::string IdType;
//       static IdType invalidId() {
//         return std::string();
//       }
//       static const char *surrogateIdField() { return 0; }
//     };
//   }
// }

DBO_EXTERN_TEMPLATES(ModelFile);
