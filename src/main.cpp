#include <Wt/Dbo/Dbo.h>
#include <Wt/WSignal.h>
#include <Wt/Auth/Login.h>
#include <Wt/Dbo/Session.h>
#include <Wt/Dbo/ptr.h>
#include <Wt/WServer.h>
#include <Wt/Dbo/backend/Sqlite3.h>
#include <Wt/Dbo/SqlConnectionPool.h>

#include "../include/ui/FileDropApplication.h"
// #include "../include/db/User.h"
// #include "../include/db/Project.h"
// #include "../include/db/ModelFile.h"
// #include "../include/db/BendFeature.h"
// #include "../include/config/Session.h"

namespace dbo=Wt::Dbo;

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment& env,
                                Wt::Dbo::SqlConnectionPool *appDb)
{
  return Wt::cpp14::make_unique<FileDropApplication>(env, *appDb);
}

int main(int argc, char **argv)
{
  Wt::WServer server(argc, argv, WTHTTP_CONFIGURATION);

  std::unique_ptr<Wt::Dbo::SqlConnectionPool> appDb = Session::createConnectionPool("fxtract.db");

  server.addEntryPoint(Wt::EntryPointType::Application,
                      std::bind(&createApplication, std::placeholders::_1, appDb.get()), "/");

  server.run();


  // return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env) {
  //
  //   // Wt::WServer server(argc, argv, WTHTTP_CONFIGURATION);
  //
  //   // BlogSession::configureAuth();
  //
  //   // std::unique_ptr<dbo::backend::Sqlite3> sqlite3{new dbo::backend::Sqlite3(":memory:")};
  //   // sqlite3->setProperty("show-queries", "true");
  //   // dbo::Session session;
  //   // session.setConnection(std::move(sqlite3));
  //   //
  //   // session.mapClass<User > ("user");
  //   // session.mapClass<Project> ("project");
  //   // session.mapClass<ModelFile> ("model_file");
  //   // session.mapClass<BendFeature > ("bend_feature");
  //   //
  //   // session.createTables();
  //
  //   return Wt::cpp14::make_unique<FileDropApplication>(env);
  // });
}
