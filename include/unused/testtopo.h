#include <TColStd_HSequenceOfTransient.hxx>
#include <TopoDS_Shape.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <GeomConvert.hxx>
#include <Geom_Surface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <GeomLProp_SLProps.hxx>
#include <BRepLProp_SLProps.hxx>
#include <TopoDS.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>
#include <Interface_Static.hxx>
#include <vector>

Standard_Real calculateCurvature(TopoDS_Face face)
{
  BRepAdaptor_Surface surface = BRepAdaptor_Surface(face);
  double u = (surface.FirstUParameter() + surface.LastUParameter()) / 2.0;
  double v = (surface.FirstVParameter() + surface.LastVParameter()) / 2.0;

  BRepLProp_SLProps surfaceProps(surface, u, v, 2, gp::Resolution());

  return surfaceProps.MeanCurvature();
}

bool isBendFace(Standard_Real Curvature){
  // std::cout << "Curvature : " << Curvature << '\n';
  return ((((Curvature < 0) ? -(Curvature) : (Curvature)) > 0) ? true : false);
}

static void tv_classifyFaces(std::vector<ModelFace>& faces, std::vector<TopoDS_Face>& tvfaces) {
  // size_t count = tvfaces.size();
  size_t i = 0;//, j = 0;
  // for (size_t i = 0; i < count; i++)
  for(auto bend : tvfaces) {
    if (isBendFace(calculateCurvature(bend))) {
      std::cout << "BEND " << (++i) << '\n';
      for(auto cmpface : tvfaces){
        if ((bend != cmpface) && !isBendFace(calculateCurvature(cmpface))) {
          // std::cout << "NOT SAME " << ++j << '\n';
          for (TopExp_Explorer bedgeEx(bend, TopAbs_EDGE); bedgeEx.More(); bedgeEx.Next()){
            TopoDS_Shape bshape = bedgeEx.Current();
            TopoDS_Edge bend_edge = TopoDS::Edge(bshape);
            for (TopExp_Explorer fedgeEx(cmpface, TopAbs_EDGE); fedgeEx.More(); fedgeEx.Next()){
              TopoDS_Shape shape = fedgeEx.Current();
              TopoDS_Edge face_edge = TopoDS::Edge(shape);
              BRepAdaptor_Curve curve = BRepAdaptor_Curve(face_edge);
              if (BRep_Tool::Degenerated(face_edge)) {
                std::cout << "ARC" << '\n';
              }
              if (BRepTools::Compare(bend_edge, face_edge)) {
                std::cout << "Checking edges..." << '\n';
              }
            }
          }
        }
      }
      //j = 0;
    }
  }
}
