// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2016 Emweb bvba, Herent, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#ifndef FILEDROPAPPLICATION_H_
#define FILEDROPAPPLICATION_H_

#include <Wt/WApplication.h>
#include <Wt/WFileDropWidget.h>

#include "../config/Session.h"

namespace Wt {
  class WContainerWidget;
}

class FileDropApplication : public Wt::WApplication
{
public:
  FileDropApplication(const Wt::WEnvironment& env, Wt::Dbo::SqlConnectionPool& appDb);

private:
  Session session_;
  // dbo::SqlConnectionPool&     connectionPool_;
  Wt::WText *log_;
  Wt::WFileDropWidget *drop_;
  int nbUploads_;
  std::string saveName, file_name;

  std::map<Wt::WFileDropWidget::File*, Wt::WContainerWidget*> icons_;

  void handleDrop(std::vector<Wt::WFileDropWidget::File *> files);
  void tooLarge(Wt::WFileDropWidget::File *file, ::uint64_t);
  void failed(Wt::WFileDropWidget::File *file);
  void saveFile(Wt::WFileDropWidget::File *file);
  void cancelUpload();
  void updateProgressListener();

  void showProgress(::uint64_t current, ::uint64_t total);
};


#endif // FILEDROPAPPLICATION_H_
