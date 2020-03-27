#ifndef IGES_PROCESSOR_H_
#define IGES_PROCESSOR_H_

#include <IGESControl_Reader.hxx>
#include "ModelProcessor.h"

using namespace std;

namespace Fxt {

  class IgesProcessor : public ModelProcessor
  {
  public:
    void extractFeactures(XSControl_Reader reader);
  };
}
#endif // IGES_PROCESSOR_H_
