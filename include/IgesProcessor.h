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
#include "../include/ModelVertex.h"

using namespace std;

class IgesProcessor
{
  std::vector<ModelFace> vface;
  std::vector<TopoDS_Face> tvfaces;

public:
  void extractFeactures(XSControl_Reader reader) {
    ModelVertex modelVertex;
    int edge_n = 0;
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
        std::cout << "000000000000: "  << '\n';

        test = new ModelFace(p, PlaneType::PLANAR);
      } else {
        test = new ModelFace(p, PlaneType::NON_PLANAR);
      }

      test->setCurvature(curvature);

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
      test->printUnitNormal();
      vface.push_back(*test);
      tvfaces.push_back(face);
    }
  }

  std::vector<ModelFace> getModelFaces(){ return vface; }
};
