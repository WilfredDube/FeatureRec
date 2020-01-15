#pragma once
#include <iostream>
#include <STEPControl_Reader.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>
#include "ModelMath.h"
#include "ModelFace.h"

using namespace std;

class StepProcessor
{
  std::vector<ModelFace> vface;
  std::vector<TopoDS_Face> tvfaces;

public:
  void extractFeactures(XSControl_Reader reader);

  std::vector<ModelFace> getModelFaces();
};
