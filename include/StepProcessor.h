#pragma once
#include <iostream>
#include <STEPControl_Reader.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>
#include <ShapeAnalysis.hxx>
#include "../include/ModelMath.h"
#include "../include/ModelFace.h"
#include "../include/ModelVertex.h"

using namespace std;

class StepProcessor
{
  std::vector<ModelFace> vface;
  std::vector<TopoDS_Face> tvfaces;

public:
  void extractFeactures(XSControl_Reader reader) {
    ModelVertex modelVertex;
    TopoDS_Shape aShape;
    int edge_n = 0;
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
        // std::cout << "face id: " << p << '\n';
        Standard_Real curvature = compute_curvature(face);
        if (curvature == 0.0){
          test = new ModelFace(p, PlaneType::PLANAR);
          std::cout << "Face id: "<< test->getFaceId() <<"---------: " << p  << '\n';
        } else {
          test = new ModelFace(p, PlaneType::NON_PLANAR);
        }

        test->setCurvature(curvature);

        if(!ShapeAnalysis::IsOuterBound(face)) {
          cout<<"Face has not outer boundary"<<endl;
        }

        // int c = 0;
        for (TopExp_Explorer edgeEx(face, TopAbs_EDGE); edgeEx.More(); edgeEx.Next()){
          TopoDS_Edge edge = TopoDS::Edge(edgeEx.Current());

          std::vector<gp_Pnt> endpoints;
          endpoints = modelVertex.getEdgeEndPoints(edge);

          ModelEdge edgex(endpoints);
          ++edge_n;
          edgex.setEdge(edge);
          edgex.setEdgeNum(edge_n);
          // std::cout << "Rational: " << BRepAdaptor_Curve(edge).IsRational() << '\n';
          edgex.setIsRational(BRepAdaptor_Curve(edge).IsRational());
          edgex.setEdgeLength(compute_length(&edgex));
          edgex.setLineVector(compute_line_vector(&edgex));
          edgex.setLineUnitVector(compute_line_unit_vector(edgex.getLineVector()));
          test->addEdge(edgex);
          // c++;
        }

        test->setUnitNormal(compute_normal(face));
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
