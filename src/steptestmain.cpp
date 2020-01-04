#include <iostream>
#include <STEPControl_Reader.hxx>
#include <string>
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
#include <ShapeFix_Wireframe.hxx>
#include <ShapeFix_Shape.hxx>
#include <ShapeAnalysis.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include "../include/math.h"
#include "../include/bend.h"
#include "../include/ModelFace.h"
// #include "../include/testtopo.h"

void printType(GeomAbs_CurveType curvetype);

void printShapeType(TopAbs_ShapeEnum shapetype)
{
  switch (shapetype) {
    case TopAbs_COMPOUND : std::cout << "TopAbs_COMPOUND" << '\n';
    break;
    case TopAbs_COMPSOLID : std::cout << "TopAbs_COMPSOLID" << '\n';
    break;
    case TopAbs_SOLID : std::cout << "TopAbs_SOLID" << '\n';
    break;
    default:
    std::cout << "NOTHING" << '\n';
  }
}
gp_Pnt getNormal(Standard_Integer fid);

std::vector<ModelFace> vface;
std::vector<TopoDS_Face> tvfaces;

using namespace std;

int main(int argc, char const *argv[]) {

  STEPControl_Reader reader;
  TopoDS_Shape aShape;
  Standard_Real prec = 1e-04;

  // IFSelect_ReturnStatus stat =
  reader.ReadFile(argv[1]);

  // if(!Interface_Static::SetRVal("read.precision.val",0.1))
  //   std::cout << ".. error .." << '\n';

  Standard_Real rp = Interface_Static::RVal("read.precision.val");
  Standard_Integer ic = Interface_Static::IVal("read.precision.mode");
  Standard_Integer mv = Interface_Static::IVal("read.maxprecision.mode");

  std::cout << "Precision value : " << rp << '\n';
  std::cout << "Precision mode : " << ic << '\n';
  std::cout << "Max precision mode : " << mv << '\n';

  IFSelect_PrintCount mode = IFSelect_ListByItem;

  reader.PrintCheckLoad(false, mode);
  TopoDS_Shape result = reader.OneShape();

  Standard_Integer NbRoots = reader.NbRootsForTransfer();                   //Transfer whole file
  reader.ClearShapes();
  // reader.PrintCheckTransfer (failsonly, IFSelect_ItemsByEntity);
  for (Standard_Integer n = 1; n<= NbRoots; n++) {
    std::cout << "STEP: Transferring Root" << n << '\n';
    reader.TransferRoot(n);
  }

  int edge_n = 0;
  size_t np = 0, p = 0;
  Standard_Integer nbs = reader.NbShapes();
  std::cout << "nbs : " << nbs << '\n';
  if (nbs == 0) {
    std::cout << "No shapes found in file " << "\n";
  } else {
    for (Standard_Integer i=1; i<=nbs; i++) {
      std::cout << "STEP: Transferring Shape" << i << '\n';
      aShape = reader.Shape(i);

      // Handle(ShapeFix_Wireframe) sfwf = new ShapeFix_Wireframe(aShape);
      // sfwf->SetPrecision(prec);
      // // sfwf->SetMaxTolerance(BRep_Tool::Tolerance(aShape));
      //
      // // sfwf->ModeDropSmallEdgesMode() = Standard_True;
      // sfwf->FixSmallEdges();
      //
      // sfwf->FixWireGaps();
      // TopoDS_Shape Result = sfwf->Shape();

      // void printShapeType(TopAbs_ShapeEnum shapetype);
      printShapeType(aShape.ShapeType());
      // std::cerr << "/* error message 1 */" << '\n';


      TopExp_Explorer myFaceExplorer(aShape, TopAbs_FACE);
      std::cout << "nbs : " << myFaceExplorer.Nb << '\n';
      size_t j = 0;
      while (myFaceExplorer.More())
      {
        ++j;

        TopoDS_Shape shape = myFaceExplorer.Current();
        TopoDS_Face face = TopoDS::Face(shape);
        ModelFace *test;

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
          // std::cout << j << '\n';
        }
        else
        {
          // test = new Bend();
          test = new Bend();
          ++np;
          test->setFaceId(np);
          test->setPlaneType(PlaneType::NON_PLANAR);
          // std::cout << " NON-PLANAR : ";
          // std::cout << j ;
          // std::cout <<" Radius Curvature : "<<(1 / surfaceProps.MeanCurvature())<< "\n";
        }

        test->setCurvature(surfaceProps.MeanCurvature());

        if(!ShapeAnalysis::IsOuterBound(face)) {
          cout<<"Face has not outer boundary"<<endl;
        }

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
        }

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

        myFaceExplorer.Next();
      }
    }
  }

  ModelFace::classifyEdges(vface);
  ModelFace::classifyBS_BF(vface);
  ModelFace::classifyFaces(vface);

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
      std::cout << "F"<< vface[i].getJoiningFaceID1() << "---B" << vface[i].getFaceId() << "---F" <<vface[i].getJoiningFaceID2()<< " = ";
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
  // cout<<"IGES Faces: "<<nIgesFaces<<"   Transferred:"<<nTransFaces<<"  Number of shapes: "<<nbs<<" Precision : "<<rp<<"\nMode : "<<ic<<" Max Precision : "<<mv<<endl;

  // Standard_Integer num = reader.TransferRoots();
  //
  // Standard_Integer NbTrans = reader.TransferRoots();

  // TopoDS_Shape result = reader.OneShape();
  // TopoDS_Shape shape = reader.Shape();

  // cout << NbRoots << endl;
  // cout << NbTrans << endl;
  // cout << num << endl;
  std::cout << "Number of resulting shapes : " << reader.NbShapes() << '\n';

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
