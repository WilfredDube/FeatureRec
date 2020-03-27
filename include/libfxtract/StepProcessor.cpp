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
      if (roundd(curvature) == 0.0){
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
      // test->setTopoDSFace(face);
      addFace(*test);
      addTopoDSFace(face);

      if (test->getPlaneType() == PlaneType::NON_PLANAR){
        // std::cout << "B"<< p;
      }
      // BRepAdaptor_Surface faceDir = BRepAdaptor_Surface(face, Standard_False);
      // gp_Dir dir = faceDir.Direction();
      // std::cout << " Orientation : " << face.Orientation() << " for Face ID : " << p << '\n';

      myFaceExplorer.Next();
    }
  }

  std::cout << "Number of faces : " << nfaces << " Number of bends: " << nbends << '\n';
}
