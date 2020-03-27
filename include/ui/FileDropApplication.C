/*
* Copyright (C) 2016 Emweb bvba, Herent, Belgium.
*
* See the LICENSE file for terms of use.
*/

#include "FileDropApplication.h"

#include <boost/algorithm/string.hpp>
#include <Wt/WContainerWidget.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WFileDropWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WMessageBox.h>
#include "Wt/Utils.h"
#include "../libfxtract/ModelApp.h"

#include <iostream>
#include <fstream>

using namespace Wt;

namespace Fxt {

  static const std::string UPLOAD_FOLDER = "./uploaded/";
  static const int MAX_FILES = 36;

  std::string DialogExampleCustom(Wt::WFileDropWidget::File *file)
  {
    WDialog dialog("Personalia");
    dialog.setClosable(true);
    dialog.setResizable(true);
    dialog.rejectWhenEscapePressed(true);

    dialog.contents()->addWidget(cpp14::make_unique<WText>("Enter your name: "));
    WLineEdit *edit = dialog.contents()->addWidget(cpp14::make_unique<WLineEdit>());
    WPushButton *ok = dialog.footer()->addWidget(cpp14::make_unique<WPushButton>("Ok"));
    ok->setDefault(true);

    edit->setFocus();
    ok->clicked().connect(&dialog, &WDialog::accept);

    if (dialog.exec() == DialogCode::Accepted) {
      return edit->text().toUTF8();
    } else {
      return file->clientFileName();
    }
  }

  FileDropApplication::FileDropApplication(const Wt::WEnvironment& env, Wt::Dbo::SqlConnectionPool& appDb)
  : WApplication(env),
  session_(appDb),
  // connectionPool_(appDb),
  nbUploads_(0)
  {
    setTitle("File Drop Example");
    useStyleSheet("style.css");

    root()->addNew<WText>("<h1>Try dropping a file in the widget below</h1>");

    drop_ = root()->addNew<WFileDropWidget>();

    drop_->setDropIndicationEnabled(true);
    // drop_->setGlobalDropEnabled(true);

    drop_->drop().connect(this, &FileDropApplication::handleDrop);
    drop_->newUpload().connect(this,&FileDropApplication::updateProgressListener);
    drop_->uploaded().connect(this, &FileDropApplication::saveFile);
    drop_->uploadFailed().connect(this, &FileDropApplication::failed);
    drop_->tooLarge().connect(this, &FileDropApplication::tooLarge);

    log_ = root()->addWidget(Wt::cpp14::make_unique<Wt::WText>());
    log_->setInline(false);
    log_->setTextFormat(Wt::TextFormat::XHTML);

    Wt::WPushButton *abort = root()->addNew<WPushButton>("Abort current upload");
    abort->clicked().connect(this, &FileDropApplication::cancelUpload);
  }

  void FileDropApplication::handleDrop(std::vector<Wt::WFileDropWidget::File *> files)
  {
    for (unsigned i=0; i < files.size(); i++) {
      WFileDropWidget::File *file = files[i];
      if (nbUploads_ >= MAX_FILES) {
        drop_->cancelUpload(file);
        continue;
      }

      std::vector<std::string> values;
      boost::split(values, (file->clientFileName()), [](char c){ return c == '.';});
      std::string name = DialogExampleCustom(file);

      if (name.find(values[values.size() - 1]) == std::string::npos) {
        name += "." + values[values.size() - 1];
      }

      file_name = name;
      saveName = UPLOAD_FOLDER + file_name;

      auto block = drop_->addNew<Wt::WContainerWidget>();
      block->setToolTip(file->clientFileName() + " [" + file->mimeType() + "]");
      block->addStyleClass("upload-block");

      icons_[file] = block;
      nbUploads_++;
    }

    if (nbUploads_ >= MAX_FILES) {
      log_->setText("That's enough ...");
      drop_->setAcceptDrops(false);
    }
  }

  void FileDropApplication::cancelUpload()
  {
    if (drop_->uploads().size() == (unsigned)drop_->currentIndex())
    return;

    Wt::WFileDropWidget::File *currentFile = drop_->uploads()[drop_->currentIndex()];
    drop_->cancelUpload(currentFile);
    icons_[currentFile]->addStyleClass("cancelled");
  }

  void FileDropApplication::tooLarge(Wt::WFileDropWidget::File *file, ::uint64_t)
  {
    icons_[file]->addStyleClass("invalid");

    log_->setText("File too large: " + file->clientFileName());
  }

  void FileDropApplication::failed(Wt::WFileDropWidget::File *file)
  {
    icons_[file]->addStyleClass("invalid");

    log_->setText("Upload failed: " + file->clientFileName());
  }

  void FileDropApplication::saveFile(Wt::WFileDropWidget::File *file)
  {
    std::string spool = file->uploadedFile().spoolFileName();
    std::ifstream src(spool.c_str(), std::ios::binary);

    //std::vector<std::string> values;
    //boost::split(values, (file->clientFileName()), [](char c){ return c == '.';});

    //saveName = UPLOAD_FOLDER + DialogExampleCustom(file) + "." + values[values.size() - 1];

    std::ofstream dest(saveName.c_str(), std::ios::binary);
    if (dest.fail()) {
      std::cerr << "**** ERROR: The output file could not be opened"
      << std::endl;
      return;
    }

    dest << src.rdbuf();

    if (icons_.find(file) != icons_.end()) {
      icons_[file]->addStyleClass("ready");
      drop_->remove(file);
    }

    dbo::Transaction transaction(session_);
    //
    dbo::ptr<ModelFile> MF(Wt::cpp14::make_unique<ModelFile>());
    // std::unique_ptr<ModelFile> MF{new ModelFile()};
    ModelFile *p = MF.modify();
    p->tensile_strength = 8.7;
    p->modelFileLocation = file_name;
    p->nbends = 0;
    p->uploadDate = Wt::WDateTime::currentDateTime();
    // // dbo::ptr<ModelFile> joe =
    cout << ">>>>>>>>>>>>>>>> TS >>>>>>>>>>>> : " << p->modelFileLocation << endl;

    session_.add(MF);
    //
    ModelApp* mp = new ModelApp(session_);
    mp->run(saveName.c_str(), MF);

    session_.flush();
    //
    transaction.commit();
    // cout << " ID == " << joe->modelFileLocation << endl;

    // ModelFile uploadedModel
  }

  void FileDropApplication::updateProgressListener()
  {
    // if there is a next file listen for progress
    if ((unsigned)drop_->currentIndex() < drop_->uploads().size()) {
      Wt::WFileDropWidget::File *file = drop_->uploads()[drop_->currentIndex()];
      file->dataReceived().connect(this, &FileDropApplication::showProgress);
      std::string fileName = Wt::Utils::htmlEncode(file_name);
      log_->setText("uploading file &quot;" + fileName + "&quot;");
    }
  }

  void FileDropApplication::showProgress(::uint64_t current, ::uint64_t total)
  {
    // Wt::WFileDropWidget::File *file = drop_->uploads()[drop_->currentIndex()];
    std::string c = std::to_string(current/1024);
    std::string t = std::to_string(total/1024);
    std::string fileName = Wt::Utils::htmlEncode(file_name);
    log_->setText("uploading file <i>&quot;" + fileName + "&quot;</i>"
    + " (" + c + "kB" + " out of " + t + "kB)");
  }

}
