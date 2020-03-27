#ifndef STEP_PROCESSOR_H_
#define STEP_PROCESSOR_H_

#include <STEPControl_Reader.hxx>
#include "ModelProcessor.h"

using namespace std;

namespace Fxt {

  class StepProcessor : public ModelProcessor
  {
  public:
    void extractFeactures(XSControl_Reader reader);
  };

}

#endif // STEP_PROCESSOR_H_
