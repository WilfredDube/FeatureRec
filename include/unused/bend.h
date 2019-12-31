#pragma once
#include "ModelFace.h"

class Bend : public ModelFace {
  ModelEdge imaginary_bend_line;
  Standard_Real Radius;
  Standard_Real bending_force;
  Standard_Real bend_angle;
  Standard_Integer joined_to_faceid;

  void setJoiningFaceID(Standard_Integer face_id) {
    joined_to_faceid = face_id;
  }

  void setBendAngle(long double angle) { bend_angle = angle; }
  long double getBendAngle(){ return bend_angle; }
};
