class ModelLineEquation {
  Standard_Real gradient;  // Line gradient
  Standard_Real intercept; // Y intercept

public:
  ModelLineEquation(ModelEdge edge){
    computeGradient(edge.start_vertex, edge.terminate_vertex);
  }
  ~ModelLineEquation();

  void computeGradient(gp_Pnt v1, gp_Pnt v2){
    gradient = (v1.Y() - v2.Y()) / (v1.X() - v2.X());
  }

  Standard_Real getGradient(){ return gradient; }
}
