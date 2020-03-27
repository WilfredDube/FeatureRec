#ifndef SESSION_H_
#define SESSION_H_

#include <Wt/Dbo/Session.h>
#include <Wt/Dbo/ptr.h>
#include <Wt/Dbo/backend/Sqlite3.h>

// #include "../db/User.h"
// #include "../db/Project.h"
#include "../db/Tool.h"
#include "../db/ModelFile.h"
#include "../db/BendFeature.h"

using namespace Wt;
namespace dbo=Wt::Dbo;

namespace Fxt {

  class Session : public dbo::Session
  {
    dbo::SqlConnectionPool& connectionPool_;
  public:
    Session(dbo::SqlConnectionPool& connectionPool);
    ~Session();

    void fillTools();
    static std::unique_ptr<dbo::SqlConnectionPool> createConnectionPool(const std::string& sqlite3);
  };
}

#endif // SESSION_H_
