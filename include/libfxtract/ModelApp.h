#ifndef MODEL_APP_H_
#define MODEL_APP_H_

#include <string>

#include "../libfxtract/ModelMath.h"
#include "../libfxtract/Model.h"
#include "../libfxtract/StepProcessor.h"
#include "../libfxtract/IgesProcessor.h"
#include "../libfxtract/ModelReader.h"
#include "../config/Session.h"
#include "../db/BendFeature.h"

#include <Wt/WApplication.h>
#include <Wt/Dbo/Dbo.h>

/*
gcc -Wall -I /usr/local/include/opencascade/ -L/usr/local/lib/ -lTKIGES -lTKSTEP -lTKXSBase -lTKBO -lTKBRep -lTKBool -lTKFeat -lTKFillet -lTKG2d -lTKG3d -lTKGeomAlgo -lTKGeomBase -lTKHLR -lTKMath -lTKMesh -lTKOffset -lTKPrim -lTKShHealing -lTKTopAlgo -lTKXMesh -lTKernel -lstdc++ -std=c++1y testmain.cpp -o test -lTKTopAlgo -lTKBRep -lTKernel -ltk -ltcl -lGL -ltbb -lXmu -lgl2ps -lm -Wl,-R/usr/local/lib

valgrind ./test ../model\ test\ files/new/90.IGS --track-origins=yes --leak-check=full

*/
namespace Fxt {

  static const std::string DB_FOLDER = "../../src/";

  class ModelApp {
    Session& session_;
  public:
    ModelApp(Session& session);

    ~ModelApp();

    int run(char const *modelFile, dbo::ptr<ModelFile> p);
  };
}
#endif // MODEL_APP_H_
