#include <Wt/Dbo/Dbo.h>
#include <Wt/WSignal.h>
#include <Wt/Auth/Login.h>
#include <Wt/Dbo/Session.h>
#include <Wt/Dbo/ptr.h>
#include <Wt/Dbo/backend/Sqlite3.h>

// #include "../include/ui/filedrop/FileDropApplication.h"
// #include "../include/db/User.h"
// #include "../include/db/Project.h"
#include "../include/db/ModelFile.h"
// #include "../include/db/BendFeature.h"
#include "../include/config/Session.h"

// class User;
class ModelFile;

namespace dbo=Wt::Dbo;

void run(){
  // std::unique_ptr<dbo::backend::Sqlite3> connection{new dbo::backend::Sqlite3("fxtract.db")};

  // connection->setProperty("show-queries", "true");
  Session session_{"fxtract.db"};
  // session_.setConnection(std::move(connection));

  // session_.mapClass<User> ("user");
  // session_.mapClass<Project> ("project");
  // session_.mapClass<ModelFile> ("model_file");
  // session_.mapClass<BendFeature > ("bend_feature");

  try {
    // session_.createTables();
    std::cerr << "Created database." << std::endl;

    {
      dbo::Transaction transaction(session_);
      //
      dbo::ptr<ModelFile> MF(Wt::cpp14::make_unique<ModelFile>());
      // std::unique_ptr<ModelFile> MF{new ModelFile()};
      ModelFile *p = MF.modify();
      p->tensile_strength = 8.7;
      p->modelFileLocation = "uploaded/oooo.igs";

      std::cout << ">>>>>>>>>>>>>>>> TS : " << p->modelFileLocation << std::endl;
      // // p->nbends = 4;
      // // p->uploadDate = Wt::WDateTime::currentDateTime();
      // // dbo::ptr<ModelFile> joe =
      session_.add(MF);
      //
      // session_.flush();
      //
      // transaction.commit();
      // dbo::Transaction transaction(session_);
      {
        dbo::collection<dbo::ptr<ModelFile>> joe = session_.find<ModelFile>().where("model_file_location = ?").bind("uploaded/oooo.igs");

        for(dbo::ptr<ModelFile> j: joe)
          std::cerr << "Joe TS = : " << j->tensile_strength << std::endl;
      }
      // cout << " ID == " << joe->modelFileLocation << endl;
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "Using existing database";
  }


}

int main(int argc, char **argv)
{
  run();
  // return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env) {

    // std::unique_ptr<dbo::backend::Sqlite3> sqlite3{new dbo::backend::Sqlite3(":memory:")};
    // sqlite3->setProperty("show-queries", "true");
    // dbo::Session session;
    // session.setConnection(std::move(sqlite3));
    //
    // session.mapClass<User > ("user");
    // session.mapClass<Project> ("project");
    // session.mapClass<ModelFile> ("model_file");
    // session.mapClass<BendFeature > ("bend_feature");
    //
    // session.createTables();

    // return Wt::cpp14::make_unique<FileDropApplication>(env);
  // });
}
