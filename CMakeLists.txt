cmake_minimum_required(VERSION 3.0)

project(hello)

add_subdirectory(include)
add_subdirectory(src)

set(CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14 -lm -Wl,-R/usr/local/lib")
set(wt_INCLUDE_DIRS /usr/loca/include/wt)
set (OpenCASCADE_LIBRARIES TKernel;TKMath;TKG2d;TKG3d;TKGeomBase;TKBRep;TKGeomAlgo;TKTopAlgo;TKPrim;TKBO;TKShHealing;TKBool;TKHLR;TKFillet;TKOffset;TKFeat;TKMesh;TKXMesh;TKService;TKV3d;TKOpenGl;TKMeshVS;TKCDF;TKLCAF;TKCAF;TKBinL;TKXmlL;TKBin;TKXml;TKStdL;TKStd;TKTObj;TKBinTObj;TKXmlTObj;TKVCAF;TKXSBase;TKSTEPBase;TKSTEPAttr;TKSTEP209;TKSTEP;TKIGES;TKXCAF;TKXDEIGES;TKXDESTEP;TKSTL;TKVRML;TKXmlXCAF;TKBinXCAF;TKDraw;TKTopTest;TKViewerTest;TKXSDRAW;TKDCAF;TKXDEDRAW;TKTObjDRAW;TKQADraw)
set (OpenCASCADE_INCLUDE_DIR  "/usr/local/include/opencascade")

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Threads REQUIRED)
find_package(wt COMPONENTS wt dbo dbosqlite3 http REQUIRED)
find_package(OpenCASCADE REQUIRED)

add_executable (hello src/main.cpp ${Foo_SOURCES})
target_include_directories(hello PRIVATE ${Boost_INCLUDE_DIRS} ${wt_INCLUDE_DIRS} ${OpenCASCADE_INCLUDE_DIR})
target_link_libraries(hello config filedrop db fxtract ${Boost_LIBRARIES} Threads::Threads ${wt_LIBRARIES} -lwthttp -lwt -lwtdbo -lwtdbosqlite3 ${OpenCASCADE_LIBRARIES})
