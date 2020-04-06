#ifndef STEP_PROCESSOR_H_
#define STEP_PROCESSOR_H_

#include <XSControl_Reader.hxx>
#include "Model.h"

using namespace std;

namespace Fxt {

  class StepProcessor
  {
  public:
    /**
    *   Extracts all the TopoDS_Face objects from a step file model
    *   @param model_ the current model from which to extract TopoDS_Faces
    *   @param reader a STEPControl_Reader object
    */
    void extractFaces(Model& model_, const XSControl_Reader reader);
  };

}

#endif // STEP_PROCESSOR_H_
