#include "IgesProcessor.h"
#include "ModelUtils.h"

namespace Fxt {

  void IgesProcessor::extractFaces(Model& model_, const XSControl_Reader reader)
  {
    if (reader.NbShapes() <= 0)
      throw invalid_argument("Unable to read model.");

    FaceID faceID = 0;
    Standard_Integer numShapes = reader.NbShapes();

    /* Go through the shape object and extract the faces. */
    for (size_t i = 1; i <= numShapes; i++) {
      TopoDS_Shape aShape = reader.Shape(i);

      ++faceID;

      model_.assignFaceAttributes(faceID, aShape);
    }
  }

}
