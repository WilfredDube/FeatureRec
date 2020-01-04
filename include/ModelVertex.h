#pragma once
#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>


class ModelVertex
{
public:
  std::vector<gp_Pnt>
  getEdgeEndPoints(TopoDS_Edge edge)
  {
    std::vector<gp_Pnt> endpoints;

    size_t i = 0;

    for (TopExp_Explorer vertexEx(edge, TopAbs_VERTEX); vertexEx.More(); vertexEx.Next(), i++)
    {
      TopoDS_Vertex vertex = TopoDS::Vertex(vertexEx.Current());
      gp_Pnt currentPnt = BRep_Tool::Pnt(vertex);
      endpoints.push_back(currentPnt);
    }

    return endpoints;
  }
};
