#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <IGESControl_Reader.hxx>
#include <XSControl_Reader.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#include <TopoDS_Shape.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepLib_MakeWire.hxx>
#include <GeomConvert.hxx>
#include <Geom_Surface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <GeomLProp_SLProps.hxx>
#include <BRepLProp_SLProps.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>
#include <Interface_Static.hxx>
#include <ShapeFix_Shape.hxx>
#include <ShapeFix_Wireframe.hxx>
#include <ShapeFix_Face.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <ShapeFix_Edge.hxx>
#include <ShapeBuild_ReShape.hxx>
#include "../include/ModelMath.h"
#include "../include/ModelFace.h"
#include "../include/StepProcessor.h"
#include "../include/IgesProcessor.h"
#include "../include/ModelReaderClass.h"

// #include "../include/testtopo.h"

/*
gcc -Wall -I /usr/local/include/opencascade/ -L/usr/local/lib/ -lTKIGES -lTKSTEP -lTKXSBase -lTKBO -lTKBRep -lTKBool -lTKFeat -lTKFillet -lTKG2d -lTKG3d -lTKGeomAlgo -lTKGeomBase -lTKHLR -lTKMath -lTKMesh -lTKOffset -lTKPrim -lTKShHealing -lTKTopAlgo -lTKXMesh -lTKernel -lstdc++ -std=c++1y testmain.cpp -o test -lTKTopAlgo -lTKBRep -lTKernel -ltk -ltcl -lGL -ltbb -lXmu -lgl2ps -lm -Wl,-R/usr/local/lib

valgrind ./test ../model\ test\ files/new/90.IGS --track-origins=yes --leak-check=full

*/

std::vector<ModelFace> vface;
std::vector<TopoDS_Face> tvfaces;

int main(int argc, char const *argv[])
{
  StepProcessor stepProcessor;
  IgesProcessor igesProcessor;
  ModelReaderClass mReader;

  //loads file MyFile.igs
  std::string filename = argv[1];
  FileFormat fileFormat = mReader.checkFileFormat(argv[1]);
  XSControl_Reader str = mReader.processModelFile(fileFormat, argv[1]);

  Standard_Integer nbs = str.NbShapes();
  std::cout << "nbs : " << nbs << '\n';

  switch (fileFormat) {
    case FileFormat::IGES_FORMAT:
    igesProcessor.extractFeactures(str);
    vface = igesProcessor.getModelFaces();
    break;
    case FileFormat::STEP_FORMAT:
    stepProcessor.extractFeactures(str);
    vface = stepProcessor.getModelFaces();
    break;
    default:
    std::cout << "Unknown file format" << '\n';
  }

  ModelFace::classifyBS_BF(vface);
  ModelFace::classifyFaces(vface);

  size_t count = vface.size();
  for (size_t i = 0; i < count; i++) {
    if (vface[i].getFaceType() == FaceType::BEND_FACE) {
      if (vface[i].getIsInternal() == true) {
        gp_Pnt firstnorm = vface[i].getNormal(vface[i].getJoiningFaceID1(), vface);
        gp_Pnt secondnorm = vface[i].getNormal(vface[i].getJoiningFaceID2(), vface);
        vface[i].setBendAngle(compute_angle(firstnorm, secondnorm));
        std::cout << "F"<< vface[i].getJoiningFaceID1() << "---B" << vface[i].getFaceId() << "---F" <<vface[i].getJoiningFaceID2()<< " = ";
        std::cout << ", Angle : " << vface[i].getBendAngle();
        std::cout << ", Radius : " << vface[i].getFaceRadius();
        std::cout << ", Bend Length: " << vface[i].getBendLength()<< " mm" << '\n';
      }
    }
  }

  std::cout << "SIZE : " <<vface.size()<< '\n';
  std::cout << "# of Bends: " << ModelFace::nbends / 2 << '\n';

  return 0;
}
