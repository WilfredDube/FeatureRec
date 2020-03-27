#ifndef MODEL_PROCESSOR_H_
#define MODEL_PROCESSOR_H_

#include <iostream>
#include <TopoDS_Shape.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>
#include "ModelMath.h"
#include "ModelFace.h"

using namespace std;

class ModelProcessor
{
  std::vector<ModelFace> vface;
  std::vector<TopoDS_Face> tvfaces;

public:
  virtual void extractFeactures(XSControl_Reader reader) = 0;

  std::vector<ModelFace> getModelFaces();
  void addFace(ModelFace& face);
  void addTopoDSFace(TopoDS_Face face);
};

#endif // MODEL_PROCESSOR_H_
