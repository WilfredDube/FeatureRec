#pragma once
#include <iostream>
#include <IGESControl_Reader.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>
#include "../include/ModelMath.h"
#include "../include/ModelFace.h"

using namespace std;

class IgesProcessor
{
  std::vector<ModelFace> vface;
  std::vector<TopoDS_Face> tvfaces;

public:
  void extractFeactures(XSControl_Reader reader) {
    size_t p = 0;

    Standard_Integer nbs = reader.NbShapes();

    /* Go through the shape object and extract the faces. */
    for (int i = 1; i <= nbs; i++) {
      TopoDS_Shape ashape = reader.Shape(i);
      TopoDS_Face& face = static_cast<TopoDS_Face&>(TopoDS::Face(ashape));
      ModelFace* test;

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
      test->printUnitNormal();
      vface.push_back(*test);
      tvfaces.push_back(face);
    }
  }

  std::vector<ModelFace> getModelFaces(){ return vface; }
};
