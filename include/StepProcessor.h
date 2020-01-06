#pragma once
#include <iostream>
#include <STEPControl_Reader.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>
#include "../include/ModelMath.h"
#include "../include/ModelFace.h"

using namespace std;

class StepProcessor
{
  std::vector<ModelFace> vface;
  std::vector<TopoDS_Face> tvfaces;

public:
  void extractFeactures(XSControl_Reader reader) {
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

        test->setUnitNormal(compute_normal(face));
        test->setCurvature(curvature);
        test->extractEdges(face);
        test->setBendLength(test->getFaceEdges());
        test->setUnitNormal(compute_Unit_normal(face));
        test->computeFaceNormal();
        test->computeFaceEquation();
        vface.push_back(*test);
        tvfaces.push_back(face);

        myFaceExplorer.Next();
      }
    }
  }

  std::vector<ModelFace> getModelFaces(){ return vface; }
};
