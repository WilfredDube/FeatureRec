set(filedrop_source_files
  FileDropApplication.C
  FileDropApplication.h
)

set (OpenCASCADE_LIBRARIES TKernel;TKMath;TKG2d;TKG3d;TKGeomBase;TKBRep;TKGeomAlgo;TKTopAlgo;TKPrim;TKBO;TKShHealing;TKBool;TKHLR;TKFillet;TKOffset;TKFeat;TKMesh;TKXMesh;TKService;TKV3d;TKOpenGl;TKMeshVS;TKCDF;TKLCAF;TKCAF;TKBinL;TKXmlL;TKBin;TKXml;TKStdL;TKStd;TKTObj;TKBinTObj;TKXmlTObj;TKVCAF;TKXSBase;TKSTEPBase;TKSTEPAttr;TKSTEP209;TKSTEP;TKIGES;TKXCAF;TKXDEIGES;TKXDESTEP;TKSTL;TKVRML;TKXmlXCAF;TKBinXCAF;TKDraw;TKTopTest;TKViewerTest;TKXSDRAW;TKDCAF;TKXDEDRAW;TKTObjDRAW;TKQADraw)
set (OpenCASCADE_INCLUDE_DIR  "/usr/local/include/opencascade")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(wt COMPONENTS wt wtdbo wtdbosqlite3 wthttp REQUIRED)
find_package(OpenCASCADE REQUIRED)

add_library(filedrop ${filedrop_source_files})
target_include_directories(filedrop PRIVATE ${Boost_INCLUDE_DIRS} ${wt_INCLUDE_DIRS} ${OpenCASCADE_INCLUDE_DIR})
target_link_libraries(filedrop fxtract ${Boost_LIBRARIES} Threads::Threads ${wt_LIBRARIES} wthttp wt wtdbo wtdbosqlite3 ${OpenCASCADE_LIBRARIES})
