#ifndef MODEL_READER_H_
#define MODEL_READER_H_

#include <XSControl_Reader.hxx>
#include <IGESControl_Reader.hxx>
#include <STEPControl_Reader.hxx>
#include "ModelUtils.h"
#include "Model.h"

using namespace std;

namespace Fxt {

  enum class FileFormat{ UNKNOWN_FORMAT, IGES_FORMAT, STEP_FORMAT };

  class ModelReader
  {
  private:
    /**
    *   Parses an IGES/STEP model file and returns a reader containing all the
    *   topological objects.
    */
    IGESControl_Reader processIgesFile(const char* igesFile);
    STEPControl_Reader processStepFile(const char* stepFile);

    /**
    *   Checks and returns the file format on a file.
    */
    FileFormat checkFileFormat(const string fileName);

    /*
    *   Extracts all the features in a model based on the uploaded file format.
    */
    void processModelFile(FileFormat fileFormat, const char* fileName);

  public:
    ModelReader(Model& model) : model_(model){}

    // Interface for processModelFile()
    void processModel(const string fileName);

  private:
    //! The current model being processed.
    Model& model_;
  };

}
#endif // MODEL_READER_H_
