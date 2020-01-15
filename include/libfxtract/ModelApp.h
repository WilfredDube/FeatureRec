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
#include <algorithm>
#include "../libfxtract/ModelMath.h"
#include "../libfxtract/ModelFace.h"
#include "../libfxtract/StepProcessor.h"
#include "../libfxtract/IgesProcessor.h"
#include "../libfxtract/ModelReaderClass.h"
#include "../config/Session.h"
#include "../db/BendFeature.h"

#include <Wt/WApplication.h>
// #include "../include/testtopo.h"

/*
gcc -Wall -I /usr/local/include/opencascade/ -L/usr/local/lib/ -lTKIGES -lTKSTEP -lTKXSBase -lTKBO -lTKBRep -lTKBool -lTKFeat -lTKFillet -lTKG2d -lTKG3d -lTKGeomAlgo -lTKGeomBase -lTKHLR -lTKMath -lTKMesh -lTKOffset -lTKPrim -lTKShHealing -lTKTopAlgo -lTKXMesh -lTKernel -lstdc++ -std=c++1y testmain.cpp -o test -lTKTopAlgo -lTKBRep -lTKernel -ltk -ltcl -lGL -ltbb -lXmu -lgl2ps -lm -Wl,-R/usr/local/lib

valgrind ./test ../model\ test\ files/new/90.IGS --track-origins=yes --leak-check=full

*/

static const std::string DB_FOLDER = "../../src/";

class ModelApp {
  // dbo::SqlConnectionPool& connectionPool_;
  Session& session_;
public:
  // ModelApp(dbo::SqlConnectionPool& connectionPool);
  ModelApp(Session& session);
  ~ModelApp();

  // static
  int run(char const *modelFile, dbo::ptr<ModelFile> p);
};
