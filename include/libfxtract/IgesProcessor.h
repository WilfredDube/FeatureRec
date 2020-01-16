#pragma once
#include <IGESControl_Reader.hxx>
#include "ModelProcessor.h"

using namespace std;

class IgesProcessor : public ModelProcessor
{
public:
  void extractFeactures(XSControl_Reader reader);
};
