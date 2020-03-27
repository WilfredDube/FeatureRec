#ifndef MODEL_VERTEX_H_
#define MODEL_VERTEX_H_

#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Edge.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <vector>

class ModelVertex
{
public:
  std::vector<gp_Pnt> getEdgeEndPoints(TopoDS_Edge edge);
};

#endif // MODEL_VERTEX_H_
