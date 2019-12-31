#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <IGESControl_Reader.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#include <TopoDS_Shape.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepLib_MakeWire.hxx>
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
#include <ShapeFix_Shape.hxx>
#include <ShapeFix_Wireframe.hxx>
#include <ShapeFix_Face.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <ShapeFix_Edge.hxx>
#include <ShapeBuild_ReShape.hxx>
#include "../include/math.h"
#include "../include/bend.h"
#include "../include/ModelFace.h"
// #include "../include/testtopo.h"

/*
gcc -Wall -I /usr/local/include/opencascade/ -L/usr/local/lib/ -lTKIGES -lTKSTEP -lTKXSBase -lTKBO -lTKBRep -lTKBool -lTKFeat -lTKFillet -lTKG2d -lTKG3d -lTKGeomAlgo -lTKGeomBase -lTKHLR -lTKMath -lTKMesh -lTKOffset -lTKPrim -lTKShHealing -lTKTopAlgo -lTKXMesh -lTKernel -lstdc++ -std=c++1y testmain.cpp -o test -lTKTopAlgo -lTKBRep -lTKernel -ltk -ltcl -lGL -ltbb -lXmu -lgl2ps -lm -Wl,-R/usr/local/lib

valgrind ./test ../model\ test\ files/new/90.IGS --track-origins=yes --leak-check=full

*/
void printType(GeomAbs_CurveType curvetype);
void printShapeType(TopAbs_ShapeEnum shapetype);
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
  std::string filename = argv[1];

  std::cout << "fffffffffffffffffffffffffffffffffffffffffffffffffffffff" << '\n';
  if (filename.find("igs") != std::string::npos || filename.find("iges") != std::string::npos) {
    std::cout << "igs OR iges" << '\n';
  } else if (filename.find("step") != std::string::npos || filename.find("stp") != std::string::npos) {
    std::cout << "step OR stp" << '\n';
  } else{
    std::cout << "Unknown format" << '\n';
  }

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
    TopoDS_Shape ashape = myIgesReader.Shape(i);

    // Heal the shape
    Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape;
    sfs->Init ( ashape );
    sfs->SetPrecision ( 0.0);
    sfs->SetMaxTolerance (0.0 );
    sfs->SetMinTolerance ( 0.0 );
    sfs->FixWireTool()->FixRemovePCurveMode() =1;
    sfs->Perform();

    //getting the result
    if(sfs->Status(ShapeExtend_DONE) ) {
      cout << "Shape was fixed" << endl;
      TopoDS_Shape resFace = sfs->Shape();
    }
    else if(sfs->Status(ShapeExtend_FAIL)) {
      cout<< "Shape could not be fixed" << endl;
    }
    else if(sfs->Status(ShapeExtend_OK)) {
      cout<< "Initial face is valid with specified precision ="<<endl;
    }
    // TopoDS_Shape shape = sfs->Shape();
    Handle(ShapeFix_Wireframe) SFWF = new ShapeFix_Wireframe(sfs->Shape());
    SFWF->ModeDropSmallEdges() = Standard_True;
    if (SFWF->StatusWireGaps(ShapeExtend_FAIL )) {
      cout<<"No gaps were found"<<endl;
    }

    // SFWF->SetPrecision ( 0.0);
    // SFWF->SetMaxTolerance (0.0 );
    // SFWF->SetMinTolerance ( 0.0 );
    SFWF->FixSmallEdges();
    SFWF->FixWireGaps();
    TopoDS_Shape shape = SFWF->Shape();
    // TopAbs_ShapeEnum shapetype = shape.ShapeType();
    // printShapeType(shape.ShapeType());
    TopoDS_Face& face = static_cast<TopoDS_Face&>(TopoDS::Face(shape));
    // TopoDS_Wire& wire = static_cast<TopoDS_Wire&>(TopoDS::Wire(SFWF));
    Handle(ShapeFix_Face) SFF= new ShapeFix_Face;
    Handle(ShapeBuild_ReShape) Context = new ShapeBuild_ReShape;
    Context->Apply(shape);
    //set a tool for rebuilding a shape in the tool for fixing
    SFF->SetContext(Context);
    //initialize the fixing tool by one face
    SFF->Init(face);
    SFF->FixAddNaturalBound();
    SFF->FixIntersectingWires();
    // SFF->FixLoopWires();
    SFF->FixMissingSeam();
    SFF->FixOrientation();
    SFF->FixSmallAreaWire(Standard_True);
    //fix the set face
    SFF->Perform();
    //get the result
    TopoDS_Shape  NewShape = SFF->Result(); //Context->Apply(shape);
    // Correcting a Wire
    // Standard_Real precision = 1e-04;
    // ShapeAnalysis_Wire saw (wire, face, precision);
    // ShapeFix_Wire sfw (wire, face, precision);

    // if (saw.CheckOrder()) {
    //   cout<<"Some edges in the wire need to be reordered"<<endl;
    //   // Two edges are incorrectly oriented
    //   sfw.FixReorder();
    //   cout<<"Reordering is done"<<endl;
    // }

    face = static_cast<TopoDS_Face&>(TopoDS::Face(NewShape));

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
      std::cout << "Face ID : "<< test->getFaceId() << '\n';
      test->setPlaneType(PlaneType::PLANAR);
      // std::cout << " PLANAR : ";
    }
    else
    {
      test = new Bend();
      ++p;
      test->setFaceId(p);
      std::cout << "BEND ID : "<< test->getFaceId() << '\n';
      test->setPlaneType(PlaneType::NON_PLANAR);
      // std::cout << " NON-PLANAR : ";
      // std::cout <<" Radius Curvature : "<<(1 / surfaceProps.MeanCurvature())<< "\n";
    }

    // std::cout << "Face_ID : "<< test->getFaceId() << '\n';
    test->setCurvature(surfaceProps.MeanCurvature());

    BRepLib_MakeWire wireMaker;
    int c = 0;
    for (TopExp_Explorer edgeEx(face, TopAbs_EDGE); edgeEx.More(); edgeEx.Next()){
      TopoDS_Shape shape = edgeEx.Current();
      TopoDS_Edge edge = TopoDS::Edge(shape);

      ShapeAnalysis_Edge sae;
      ShapeFix_Edge sfe;
      Standard_Real maxdev;
      if (sae.CheckSameParameter (edge, maxdev)) {
        cout<<"Incorrect SameParameter flag"<<endl;
        cout<<"Maximum deviation "<<maxdev<< ", tolerance "<<BRep_Tool::Tolerance(edge)<<endl;
        sfe.FixSameParameter(edge, BRep_Tool::Tolerance(edge));
        cout<<"New tolerance "<<BRep_Tool::Tolerance(edge)<<endl;
        sfe.FixReversed2d(edge, face);
      }
      wireMaker.Add(edge);
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
      edgex.setEdge(edge);
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

    TopoDS_Wire wire = wireMaker.Wire();
    Standard_Real precision = 1e-07;
    ShapeAnalysis_Wire saw (wire, face, precision);
    ShapeFix_Wire sfw (wire, face, precision);

    if (saw.CheckOrder()) {
      cout<<"Some edges in the wire need to be reordered"<<endl;
      // Two edges are incorrectly oriented
      sfw.FixReorder();
      cout<<"Reordering is done"<<endl;
    }

    if (saw.CheckSmall (precision)) {
      cout<<"Wire contains edge(s) shorter than "<<precision<<endl;
      // An edge that is shorter than the given tolerance is found.
      Standard_Boolean LockVertex = Standard_True;
      if (sfw.FixSmall (LockVertex, precision)) {
        cout<<"Edges shorter than “<<precision<<“ have been removed" <<endl;
        //The edge is removed
      }
    }
    if (saw.CheckSelfIntersection()) {
      cout<<"Wire has self-intersecting or intersecting adjacent edges"<<endl;
      // Two intersecting adjacent edges are found.
      if (sfw.FixSelfIntersection()) {
        cout<<"Wire was slightly self-intersecting. Repaired"<<endl;
        // The edges are cut at the intersection point so that they no longer intersect.
      }
    }

    Standard_Boolean LockVertex = Standard_True;
    if (sfw.FixSmall (LockVertex, precision)) {
      //Removes all edges which are shorter than the given precision and have the same vertex at both ends.
      std::cout << "FIX SMALL" << '\n';
    }
    if (sfw.FixSelfIntersection()) {
      //Fixes self-intersecting edges and intersecting adjacent edges.
      cout <<"Wire was slightly self-intersecting. Repaired"<<endl;
    }
    if ( sfw.FixLacking ( Standard_False ) ) {
      //Inserts edges to connect adjacent non-continuous edges.
      std::cout << "FIX LACKING" << '\n';
    }

    // std::cout <<"======>"<< c << '\n';
    Standard_Real umin, umax, vmin, vmax;
    BRepTools::UVBounds(face, umin, umax, vmin, vmax); // create surface
    Handle(Geom_Surface) surf=BRep_Tool::Surface(face); // get surface properties
    GeomLProp_SLProps props(surf, umin, vmin, 1, 0.01); // get surface normal
    gp_Dir norm=props.Normal(); // check orientation
    gp_Dir nm = surfaceProps.Normal();

    std::cout << "----------------x = "<<norm.X() << '\n';
    std::cout << "++++++++++++++++x = "<<nm.X() << '\n';
    std::cout << "----------------x = "<<norm.Y() << '\n';
    std::cout << "++++++++++++++++x = "<<nm.Y() << '\n';
    std::cout << "----------------x = "<<norm.Z() << '\n';
    std::cout << "++++++++++++++++x = "<< nm.Z() << '\n';
    test->setUnitNormal(norm);

    test->computeFaceNormal();
    test->computeFaceEquation();
    test->printUnitNormal();
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
