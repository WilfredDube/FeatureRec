#ifndef PROJECT_H_
#define PROJECT_H_

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "User.h"
#include "ModelFile.h"
class ModelFile;
class User;

// namespace std;
namespace dbo = Wt::Dbo;

typedef dbo::collection< dbo::ptr<ModelFile> > ModelFiles;

class Project {
public:
  // dbo::ptr<User> creator;

  // std::string project_id;
  std::string title;
  std::string brief_desc;
  int nModelFiles;
  Wt::WDateTime  dateCreated;

  ModelFiles modelFiles;

  template<class Action>
  void persist(Action& a) {
    // dbo::id(a, project_id, "project_id", 20);
    dbo::field(a, title, "project_title");
    dbo::field(a, brief_desc, "project_brief_desc");
    dbo::field(a, nModelFiles, "project_model_count");
    dbo::field(a, dateCreated, "project_creation_date");

    // dbo::belongsTo(a, creator, "project_creator");
    //
    // dbo::hasMany(a, modelFiles, dbo::ManyToOne, "project_id");
  }
};

// namespace Wt {
//   namespace Dbo {
//     template<>
//     struct dbo_traits<Project> : public dbo_default_traits {
//       typedef std::string IdType;
//       static IdType invalidId() {
//         return std::string();
//       }
//       static const char *surrogateIdField() { return 0; }
//     };
//   }
// }

DBO_EXTERN_TEMPLATES(Project);

#endif // PROJECT_H_
