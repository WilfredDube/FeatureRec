#ifndef USER_H_
#define USER_H_

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Sqlite3.h>

#include "Project.h"
class Project;

// namespace std::;
namespace dbo = Wt::Dbo;

typedef dbo::collection< dbo::ptr<Project> > Projects;

enum class Role {
  GUEST = 0,
  MEMBER = 1,
  ADMIN = 2
};

class User {
public:
  // std::string userId;
  std::string userFullName;
  std::string userEmail;
  std::string userPassword;
  Role userRole;
  Wt::WDateTime  dateCreated;
  Wt::WDateTime dateExpires;

  // Projects userProjects;

  template<class Action>
  void persist(Action& a) {
    // dbo::id(a, userId, "user_id", 20);
    dbo::field(a, userFullName, "user_full_name");
    dbo::field(a, userEmail, "user_email");
    dbo::field(a, userPassword, "user_password");
    dbo::field(a, userRole, "user_role");
    dbo::field(a, dateCreated, "user_account_creation_date");
    dbo::field(a, dateExpires, "user_account_expiry_date");

    // dbo::hasMany(a, userProjects, dbo::ManyToOne, "user_projects");
  }
};

// namespace Wt {
//   namespace Dbo {
//     template<>
//     struct dbo_traits<User> : public dbo_default_traits {
//       typedef std::string IdType;
//       static IdType invalidId() {
//         return std::string();
//       }
//       static const char *surrogateIdField() { return 0; }
//     };
//   }
// }

DBO_EXTERN_TEMPLATES(User);

#endif // USER_H
