#include "ModelReader.h"

FileFormat ModelReader::checkFileFormat(string filename){
  if (filename.find("igs") != std::string::npos || filename.find("iges") != std::string::npos) {
    return FileFormat::IGES_FORMAT;
  } else if (filename.find("step") != std::string::npos || filename.find("stp") != std::string::npos) {
    return FileFormat::STEP_FORMAT;
  }

  return FileFormat::UNKNOWN_FORMAT;
}

XSControl_Reader ModelReader::processModelFile(FileFormat fileFormat, const char* filename)
{
  switch (fileFormat) {
    case FileFormat::IGES_FORMAT:
    return processIgesFile(filename);
    case FileFormat::STEP_FORMAT:
    return processStepFile(filename);
    default:
    std::cout << "Unknown file format" << '\n';
  }

  return NULL;
}

IGESControl_Reader ModelReader::processIgesFile(const char* igesFile){
  IGESControl_Reader myIgesReader;

  myIgesReader.ReadFile (igesFile);

  IFSelect_PrintCount mode = IFSelect_ListByItem;
  myIgesReader.PrintCheckLoad(false, mode);

  // printPrecision();
  //selects all IGES faces in the file and puts them into a list  called //MyList,
  Handle(TColStd_HSequenceOfTransient) myList =  myIgesReader.GiveList("iges-faces");
  myIgesReader.TransferRoots();

  // //translates MyList,
  myList->Length();
  myIgesReader.TransferList(myList);

  return myIgesReader;
}

STEPControl_Reader ModelReader::processStepFile(const char* stepFile){
  STEPControl_Reader myStepReader;

  myStepReader.ReadFile(stepFile);

  IFSelect_PrintCount mode = IFSelect_ListByItem;
  myStepReader.PrintCheckLoad(false, mode);

  Standard_Integer NbRoots = myStepReader.NbRootsForTransfer();                   //Transfer whole file
  myStepReader.ClearShapes();

  for (Standard_Integer n = 1; n<= NbRoots; n++) {
    std::cout << "STEP: Transferring Root" << n << '\n';
    myStepReader.TransferRoot(n);
  }

  return myStepReader;
}
