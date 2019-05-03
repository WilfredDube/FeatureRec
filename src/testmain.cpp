#include <iostream>
#include <memory>
#include <cstring>
#include <IGESControl_Reader.hxx>
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
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>
#include <Interface_Static.hxx>
#include "../include/math.h"
#include "../include/bend.h"
#include "../include/ModelFace.h"
#include "../include/testtopo.h"

void printType(GeomAbs_CurveType curvetype);
gp_Pnt getNormal(Standard_Integer fid);

std::vector<ModelFace> vface;
std::vector<TopoDS_Face> tvfaces;
int main(int argc, char const *argv[]) {
  // ModelFace test;
  // int x = 4;
  // Edge edgex;
  IGESControl_Reader myIgesReader;
  Standard_Integer nIgesFaces,nTransFaces;

  //loads file MyFile.igs
  myIgesReader.ReadFile (argv[1]);

  Standard_Real rp = Interface_Static::RVal("read.precision.val");
  Standard_Integer ic = Interface_Static::IVal("read.precision.mode");
  Standard_Integer mv = Interface_Static::IVal("read.maxprecision.mode");

  //selects all IGES faces in the file and puts them into a list  called //MyList,
  Handle(TColStd_HSequenceOfTransient) myList =  myIgesReader.GiveList("iges-faces");
  myIgesReader.TransferRoots();

  //translates MyList,
  nIgesFaces = myList->Length();
  nTransFaces = myIgesReader.TransferList(myList);
  Standard_Integer nbs = myIgesReader.NbShapes();

  int edge_n = 0;
  size_t np = 0, p = 0;

  /* Go through the shape object and extract the faces. */
  for (int i = 1; i <= nbs; i++) {
    TopoDS_Shape shape = myIgesReader.Shape(i);
  //
    // TopAbs_ShapeEnum shapetype = shape.ShapeType();
    TopoDS_Face& face = static_cast<TopoDS_Face&>(TopoDS::Face(shape));
    ModelFace* test;

    // test->TopoDS_Face::operator=(face);
    // Bend bend;
    // printType(shapetype);

    // face.thickness = 0.88;
    // face.setFaceId("2ww");
    BRepAdaptor_Surface surface = BRepAdaptor_Surface(face);
    double u = (surface.FirstUParameter() + surface.LastUParameter()) / 2.0;
    double v = (surface.FirstVParameter() + surface.LastVParameter()) / 2.0;

    BRepLProp_SLProps surfaceProps(surface, u, v, 2, gp::Resolution());
    if (surfaceProps.MeanCurvature() == 0.0)
    {
      test = new ModelFace();
      ++p;
      test->setFaceId(p);
      test->setPlaneType(PlaneType::PLANAR);
      // std::cout << " PLANAR : ";
    }
    else
    {
      test = new Bend();
      ++np;
      test->setFaceId(np);
      test->setPlaneType(PlaneType::NON_PLANAR);
      // std::cout << " NON-PLANAR : ";
      // std::cout <<" Radius Curvature : "<<(1 / surfaceProps.MeanCurvature())<< "\n";
    }

    // std::cout << "Face_ID : "<< test->getFaceId() << '\n';
    test->setCurvature(surfaceProps.MeanCurvature());

    int c = 0;
    for (TopExp_Explorer edgeEx(face, TopAbs_EDGE); edgeEx.More(); edgeEx.Next()){
      TopoDS_Shape shape = edgeEx.Current();
      TopoDS_Edge edge = TopoDS::Edge(shape);

      BRepAdaptor_Curve curve = BRepAdaptor_Curve(edge);
      // printType(curve.GetType());
      gp_Pnt endpoints[2];
      // face.v.push_back(shape);
      // face.setEdge(edge);
      // edge.setEdgeNum(edge_n);
      // MEdge edgem = edge;

      // vec.push_back(edge);
      // face.face_edges.push_back(edge);

      // face.setFaceEdges(edge_n);
      size_t i = 0;
      for (TopExp_Explorer vertexEx(edge, TopAbs_VERTEX); vertexEx.More(); vertexEx.Next(), i++)
      {
        TopoDS_Vertex vertex = TopoDS::Vertex(vertexEx.Current());
        gp_Pnt currentPnt = BRep_Tool::Pnt(vertex);
        endpoints[i] = currentPnt;
        // std::cout << "Points : "<< endpoints[i].X()<<", "<< endpoints[i].Y()<<", "<<endpoints[i].Z()<< '\n';
      }

      ModelEdge edgex(endpoints);
      ++edge_n;
      edgex.setEdgeNum(edge_n);
      edgex.setIsRational(curve.IsRational());
      // std::cout << "Rational : " << (curve.IsRational()?"YES":"NO")<<'\n';
      Standard_Real len = compute_length(&edgex);
      edgex.setEdgeLength(len);
      edgex.setLineVector(compute_line_vector(&edgex));
      edgex.setLineUnitVector(compute_line_unit_vector(edgex.getLineVector()));
      // std::cout << "Points : "<< q.X()<<", "<< q.Y()<<", "<< q.Z()<< '\n';
      test->addEdge(edgex);
      c++;
      // std::cout << edgex.getEdgeNum() <<" | "<<edgex.getEdgeLength() << '\n';
    //   std::cout << "Size " << vec.size()<< '\n';
    }

    // std::cout <<"======>"<< c << '\n';
    Standard_Real umin, umax, vmin, vmax;
    BRepTools::UVBounds(face, umin, umax, vmin, vmax); // create surface
    Handle(Geom_Surface) surf=BRep_Tool::Surface(face); // get surface properties
    GeomLProp_SLProps props(surf, umin, vmin, 1, 0.01); // get surface normal
    gp_Dir norm=props.Normal(); // check orientation
    test->setUnitNormal(norm);

    test->computeFaceNormal();
    test->computeFaceEquation();
    // test->printUnitNormal();
    // std::cout << "" << '\n';
    vface.push_back(*test);
    tvfaces.push_back(face);
  }

  ModelFace::classifyEdges(vface);
  ModelFace::classifyBS_BF(vface);
  ModelFace::classifyFaces(vface);

  // tv_classifyFaces(vface, tvfaces);
  size_t count = vface.size();
  for (size_t i = 0; i < count; i++) {
    // std::cout << "F" << vface[i].getFaceId()<<endl;
      // switch (vface[i].getFaceType()) {
      //   case FaceType::THICKNESS_DEFINING_FACE :   std::cout << "TDF Face ID : T" << vface[i].getFaceId() << '\n';
      //   break;
      //   case FaceType::BEND_SIDE :   std::cout << "Bend side ID : BS" << vface[i].getFaceId()<<endl;
      //   break;
      //   case FaceType::FACE :   std::cout << "Face ID : F" << vface[i].getFaceId() << '\n';
      //   break;
      //   case FaceType::BEND_FACE :   std::cout << "Bend Face ID : B" << vface[i].getFaceId()<<endl;//"===";
      //   // std::cout << " F" << vface[i].getJoiningFaceID1() << " and F" << vface[i].getJoiningFaceID2() << '\n';
      //   break;
      //   default:
      //   std::cout << "Not Set" << '\n';
      // }
    if (vface[i].getFaceType() == FaceType::BEND_FACE) {
      std::cout << "F"<< vface[i].getJoiningFaceID1() << "---B" << vface[i].getFaceId() << "---F" <<vface[i].getJoiningFaceID2()<< '=';
      std::cout << ", Angle : " << compute_angle(getNormal(vface[i].getJoiningFaceID1()), getNormal(vface[i].getJoiningFaceID2()));
      std::cout << ", Radius : " << vface[i].getFaceRadius() << '\n';
    }
    // else {
    //   std::cout << "No of edges : " << vface[i].getFaceEdges().size() << '\n';
    // }
    }

  // for (size_t i = 0; i < vface.size(); i++) {
  //   switch (vface[i].getFaceType()) {
  //     case FaceType::THICKNESS_DEFINING_FACE :   std::cout << "TDF Face ID : T" << vface[i].getFaceId() << '\n';
  //     break;
  //     case FaceType::BEND_SIDE :   std::cout << "Bend side ID : BS" << vface[i].getFaceId()<<endl;
  //     break;
  //     case FaceType::FACE :   std::cout << "Face ID : F" << vface[i].getFaceId() << '\n';
  //     break;
  //     case FaceType::BEND_FACE :   std::cout << "Bend Face ID : B" << vface[i].getFaceId()<<"===";
  //     std::cout << " F" << vface[i].getJoiningFaceID1() << " and F" << vface[i].getJoiningFaceID2() << '\n';
  //     break;
  //     default:
  //     std::cout << "Not Set" << '\n';
  //   }
  // }

  std::cout << "SIZE : " <<vface.size()<< '\n';
  // test.setVector(edgex);
  cout<<"IGES Faces: "<<nIgesFaces<<"   Transferred:"<<nTransFaces<<"  Number of shapes: "<<nbs<<" Precision : "<<rp<<"\nMode : "<<ic<<" Max Precision : "<<mv<<endl;

  return 0;
}

gp_Pnt getNormal(Standard_Integer fid){
  gp_Pnt vt(Standard_Real(NULL), Standard_Real(NULL), Standard_Real(NULL));
  for (auto& face : vface) {
    if (face.getFaceId() == fid) {
      vt = face.getFaceNormal();
    }
  }
  return vt;
}

void printType(GeomAbs_CurveType curvetype)
{
  switch (curvetype) {
    case GeomAbs_Line:std::cout << "GeomAbs_Line" << '\n';
    break;
    case GeomAbs_Circle:std::cout << "GeomAbs_Circle" << '\n';
    break;
    case GeomAbs_BSplineCurve:std::cout << "GeomAbs_BsplineCurve" << '\n';
    break;
    default:
    std::cout << "NOTHING" << '\n';
  }
}
