template<typename T>
Standard_Real computeCurvature(T face)
{
  BRepAdaptor_Surface surface = BRepAdaptor_Surface(face);
  double u = (surface.FirstUParameter() + surface.LastUParameter()) / 2.0;
  double v = (surface.FirstVParameter() + surface.LastVParameter()) / 2.0;

  BRepLProp_SLProps surfaceProps(surface, u, v, 2, gp::Resolution());

  return surfaceProps.MeanCurvature();
}
