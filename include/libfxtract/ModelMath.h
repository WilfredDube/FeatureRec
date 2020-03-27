#pragma once
#include <cmath>
#include <vector>
#include "ModelEdge.h"
#include "ModelFace.h"

class ModelFace;

const Standard_Real PI = 3.14159265358979323;

long double roundd(long double var);
gp_Dir compute_unit_normal(TopoDS_Face face);
Standard_Real compute_curvature(TopoDS_Face face);
long double compute_euclidean_norm(gp_Pnt *vt);
long double compute_plane_equation(std::vector<ModelEdge> edges, gp_Pnt normal);
static bool compare_vl(gp_Pnt v1, gp_Pnt v2);
gp_Pnt compute_cross_product(gp_Pnt *a, gp_Pnt *b);
gp_Pnt compute_line_vector(ModelEdge *edge);
gp_Pnt compute_line_unit_vector(gp_Pnt line_vector);
long double compute_dot_product(gp_Pnt *vt1, gp_Pnt *vt2);
long double compute_length(ModelEdge *edge);
long double compute_angle(gp_Pnt n1, gp_Pnt n2);
gp_Pnt compute_normal(std::vector<ModelEdge> edges);
Standard_Real compute_thickness(ModelFace face);
