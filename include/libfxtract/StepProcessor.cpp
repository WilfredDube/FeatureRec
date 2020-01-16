#include "StepProcessor.h"

void StepProcessor::extractFeactures(XSControl_Reader reader) {
  TopoDS_Shape aShape;
  size_t p = 0, j = 0;

  Standard_Integer nbs = reader.NbShapes();

  for (Standard_Integer i=1; i<=nbs; i++, ++j) {
    aShape = reader.Shape(i);
    TopExp_Explorer myFaceExplorer(aShape, TopAbs_FACE);

    while (myFaceExplorer.More())
    {
      TopoDS_Shape shape = myFaceExplorer.Current();
      TopoDS_Face face = TopoDS::Face(shape);
      ModelFace *test;

      ++p;
      Standard_Real curvature = compute_curvature(face);
      if (curvature == 0.0){
        test = new ModelFace(p, PlaneType::PLANAR);
      } else {
        test = new ModelFace(p, PlaneType::NON_PLANAR);
      }

      test->setUnitNormal(compute_unit_normal(face));
      test->setCurvature(curvature);
      test->extractEdges(face);
      test->setBendLength(test->getFaceEdges());
      test->computeFaceNormal();
      test->computeFaceEquation();
      addFace(*test);
      addTopoDSFace(face);

      myFaceExplorer.Next();
    }
  }
}
