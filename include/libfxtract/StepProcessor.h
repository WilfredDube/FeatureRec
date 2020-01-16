#pragma once
#include <STEPControl_Reader.hxx>
#include "ModelProcessor.h"

using namespace std;

class StepProcessor : public ModelProcessor
{
public:
  void extractFeactures(XSControl_Reader reader);
};
