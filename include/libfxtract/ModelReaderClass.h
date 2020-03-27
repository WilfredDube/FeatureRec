#pragma once
#include <IGESControl_Reader.hxx>
#include <STEPControl_Reader.hxx>
#include <XSControl_Reader.hxx>
#include "ModelUtils.h"

using namespace std;

enum class FileFormat{ UNKNOWN_FORMAT, IGES_FORMAT, STEP_FORMAT };

class ModelReaderClass
{
public:
  FileFormat checkFileFormat(string filename);
  XSControl_Reader processModelFile(FileFormat fileFormat, const char* filename);
  IGESControl_Reader processIgesFile(const char* igesFile);
  STEPControl_Reader processStepFile(const char* stepFile);
};