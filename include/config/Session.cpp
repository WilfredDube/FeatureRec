#include "Session.h"
#include <iostream>
#include <Wt/Dbo/FixedSqlConnectionPool.h>
#include <Wt/Dbo/SqlConnectionPool.h>

using namespace Wt;

namespace dbo=Wt::Dbo;

Session::Session(dbo::SqlConnectionPool& connectionPool)
: connectionPool_(connectionPool)
{
  setConnectionPool(connectionPool_);

  // mapClass<User> ("user");
  // mapClass<Project> ("project");
  mapClass<Tool> ("tools");
  mapClass<ModelFile> ("model_file");
  mapClass<BendFeature > ("bend_feature");


  try {
    createTables();
    fillTools();
    std::cerr << "Created database." << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "Using existing database" << std::endl;
  }
}

void Session::fillTools(){
  dbo::Transaction transaction(*this);
  //
  dbo::ptr<Tool> MFA(Wt::cpp14::make_unique<Tool>());
  Tool *a = MFA.modify();
  a->addTool("AC26", "ACUTE", 26, 1000, 0.2, 50);
  add(MFA);

  dbo::ptr<Tool> MFB(Wt::cpp14::make_unique<Tool>());
  Tool *b = MFB.modify();
  b->addTool("AC30", "ACUTE", 30, 1000, 0.2, 50);
  add(MFB);

  dbo::ptr<Tool> MFC(Wt::cpp14::make_unique<Tool>());
  Tool *c = MFC.modify();
  c->addTool("AC35", "ACUTE", 35, 1000, 0.2, 50);
  add(MFC);

  dbo::ptr<Tool> MFD(Wt::cpp14::make_unique<Tool>());
  Tool *d = MFD.modify();
  d->addTool("AC45", "ACUTE", 45, 1000, 0.2, 50);
  add(MFD);

  dbo::ptr<Tool> MFE(Wt::cpp14::make_unique<Tool>());
  Tool *e = MFE.modify();
  e->addTool("AC60", "ACUTE", 60, 1000, 0.2, 50);
  add(MFE);

  dbo::ptr<Tool> MFF(Wt::cpp14::make_unique<Tool>());
  Tool *f = MFF.modify();
  f->addTool("AC88", "GOOSENECK", 88, 1000, 0.2, 50);
  add(MFF);

  dbo::ptr<Tool> MFG(Wt::cpp14::make_unique<Tool>());
  Tool *g = MFG.modify();
  g->addTool("AC90", "GOOSENECK", 90, 1000, 0.2, 50);
  add(MFG);

  dbo::ptr<Tool> MFH(Wt::cpp14::make_unique<Tool>());
  Tool *h = MFH.modify();
  h->addTool("VT15", "VTOOL", 15, 1000, 0.2, 50);
  add(MFH);

  dbo::ptr<Tool> MFI(Wt::cpp14::make_unique<Tool>());
  Tool *i = MFI.modify();
  i->addTool("VT20", "VTOOL", 20, 1000, 0.2, 50);
  add(MFI);

  dbo::ptr<Tool> MFJ(Wt::cpp14::make_unique<Tool>());
  Tool *j = MFJ.modify();
  j->addTool("VT30", "VTOOL", 30, 1000, 0.2, 50);
  add(MFJ);

  dbo::ptr<Tool> MFK(Wt::cpp14::make_unique<Tool>());
  Tool *k = MFK.modify();
  k->addTool("VT35", "VTOOL", 35, 1000, 0.2, 50);
  add(MFK);

  dbo::ptr<Tool> MFL(Wt::cpp14::make_unique<Tool>());
  Tool *l = MFL.modify();
  l->addTool("VT45", "VTOOL", 45, 1000, 0.2, 50);
  add(MFL);

  dbo::ptr<Tool> MFM(Wt::cpp14::make_unique<Tool>());
  Tool *m = MFM.modify();
  m->addTool("VT60", "VTOOL", 60, 1000, 0.2, 50);
  add(MFM);

  dbo::ptr<Tool> MFN(Wt::cpp14::make_unique<Tool>());
  Tool *n = MFN.modify();
  n->addTool("VT88", "VTOOL", 88, 1000, 0.2, 50);
  add(MFN);

  dbo::ptr<Tool> MFO(Wt::cpp14::make_unique<Tool>());
  Tool *o = MFO.modify();
  o->addTool("VT90", "VTOOL", 90, 1000, 0.2, 50);
  add(MFO);

  transaction.commit();
}

std::unique_ptr<dbo::SqlConnectionPool> Session::createConnectionPool(const std::string& sqliteDb)
{
  auto connection = Wt::cpp14::make_unique<dbo::backend::Sqlite3>(sqliteDb);

  connection->setProperty("show-queries", "true");
  connection->setDateTimeStorage(Wt::Dbo::SqlDateTimeType::DateTime, Wt::Dbo::backend::DateTimeStorage::PseudoISO8601AsText);

  return Wt::cpp14::make_unique<dbo::FixedSqlConnectionPool>(std::move(connection), 10);
}

Session::~Session()
{
}
