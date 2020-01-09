#pragma once
#include <cmath>
#include <vector>
#include "ModelEdge.h"

long double roundd(float var)
{
    // we use array of chars to store number
    // as a string.
    char str[40];

    // Print in string the value of var
    // with two decimal point
    sprintf(str, "%.2f", var);

    // scan string value in var
    sscanf(str, "%f", &var);

    return var;
}

gp_Dir
compute_unit_normal(TopoDS_Face face)
{
  Standard_Real umin, umax, vmin, vmax;
  BRepTools::UVBounds(face, umin, umax, vmin, vmax); // create surface
  Handle(Geom_Surface) surf=BRep_Tool::Surface(face); // get surface properties
  GeomLProp_SLProps props(surf, umin, vmin, 1, 0.01); // get surface normal
  gp_Dir norm=props.Normal(); // check orientation

  norm = gp_Dir(roundd(norm.X()), roundd(norm.Y()), roundd(norm.Z()));

  return norm;
}

Standard_Real
compute_curvature(TopoDS_Face face)
{
  BRepAdaptor_Surface surface = BRepAdaptor_Surface(face);
  double u = (surface.FirstUParameter() + surface.LastUParameter()) / 2.0;
  double v = (surface.FirstVParameter() + surface.LastVParameter()) / 2.0;

  BRepLProp_SLProps surfaceProps(surface, u, v, 2, gp::Resolution());

  return surfaceProps.MeanCurvature();
}

long double
compute_euclidean_norm(gp_Pnt *vt)
{
  long double A = vt->X() * vt->X();
  long double B = vt->Y() * vt->Y();
  long double C = vt->Z() * vt->Z();

  return sqrt(A + B + C);
}

const Standard_Real PI = 3.14159265358979323;

long double
compute_plane_equation(std::vector<ModelEdge> edges, gp_Pnt normal)
{
  long double d1, d2;
  size_t count = edges.size();

  for (size_t i = 0; i < count; i++) {
    ModelEdge edge = edges[i];

    d1 = (normal.X() * edge.start_vertex.X()) + (normal.Y() * edge.start_vertex.Y()) + (normal.Z() * edge.start_vertex.Z());
    d2 = (normal.X() * edge.terminate_vertex.X()) + (normal.Y() * edge.terminate_vertex.Y()) + (normal.Z() * edge.terminate_vertex.Z());

    if (fabsl(d1 - d2) < (long double)0.00001) {
      return -d1;
    }

  }

  return -10000000;
}

static bool
compare_vl(gp_Pnt v1, gp_Pnt v2)
{
  bool truth = false;

  if (v1.X() == v2.X() && v1.Y() == v2.Y() && v1.Z() == v2.Z()) {
    truth = true;
  }

  return truth;
}

gp_Pnt
compute_cross_product(gp_Pnt *a, gp_Pnt *b)
{
  // gp_Pnt *normal;// = (gp_Pnt *)malloc(sizeof(* normal));

  Standard_Real X = (a->Y() * b->Z()) - (a->Z() * b->Y());
  Standard_Real Y = - ((a->X() * b->Z()) - (a->Z() * b->X()));
  Standard_Real Z = (a->X() * b->Y()) - (a->Y() * b->X());

  gp_Pnt normal(X, Y, Z);
  return normal;
}

gp_Pnt
compute_line_vector(ModelEdge *edge)
{
  gp_Pnt vt(
            edge->start_vertex.X() - edge->terminate_vertex.X(),
            edge->start_vertex.Y() - edge->terminate_vertex.Y(),
            edge->start_vertex.Z() - edge->terminate_vertex.Z());

  return vt;
}

gp_Pnt
compute_line_unit_vector(gp_Pnt line_vector)
{
  long double magnitude = compute_euclidean_norm(&line_vector);

  gp_Pnt vt (
    line_vector.X() / magnitude,
    line_vector.Y() / magnitude,
    line_vector.Z() / magnitude
  );

  return vt;
}

long double
compute_dot_product(gp_Pnt *vt1, gp_Pnt *vt2)
{
  long double A = vt1->X() * vt2->X();
  long double B = vt1->Y() * vt2->Y();
  long double C = vt1->Z() * vt2->Z();

  return (A + B + C);
}


long double
compute_length(ModelEdge *edge)
{
  gp_Pnt vt(
            edge->start_vertex.X() - edge->terminate_vertex.X(),
            edge->start_vertex.Y() - edge->terminate_vertex.Y(),
            edge->start_vertex.Z() - edge->terminate_vertex.Z());

  return compute_euclidean_norm(&vt);
}

long double
compute_angle(gp_Pnt n1, gp_Pnt n2)
{
  long double cosine, angle;
  long double dotp = compute_dot_product(&n1, &n2);

  long double en1 = compute_euclidean_norm(&n1);
  long double en2 = compute_euclidean_norm(&n2);

  if (dotp == 0) {
    cosine = 0;
  } else {
    cosine = dotp / (en1 * en2);
  }

  long double value = 180.0 / PI;

  angle = acos(roundd(cosine)) * value;

  return round(angle);
}

gp_Pnt
compute_normal(std::vector<ModelEdge> edges)
{
  ModelEdge edge1, edge2;

  edge1 = edges[0];
  for (size_t i = 0; i < edges.size(); i++) {
    if (edge1.getEdgeNum() == edges[i].getEdgeNum()) {
      continue;
    }

    if (compare_vl(edge1.start_vertex, (edges[i]).start_vertex) ||
        compare_vl(edge1.terminate_vertex, (edges[i]).start_vertex) ||
        compare_vl(edge1.terminate_vertex, (edges[i]).terminate_vertex) ||
        compare_vl(edge1.start_vertex, (edges[i]).terminate_vertex)) {
          edge2 = (edges[i]);
          // printf("%d\n", edge2->edge_number);
          break;
    }

  }

  gp_Pnt vta = compute_line_vector(&edge1);
  gp_Pnt vtb = compute_line_vector(&edge2);

  vta = compute_cross_product(&vta, &vtb);

  return vta;
}
