project(FeactureRec)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -lm -Wl,-R/usr/local/lib")
set(INCLUDE_DIR ./include)
#set(INCLUDE_LIBS /usr/local/include/opencascade/)
set(SRC_DIR ./src)

include_directories(/usr/local/include/opencascade/ /usr/local/lib/)
include_directories(
  ${INCLUDE_DIR}
  ${INCLUDE_DIR}/*
  )

add_executable(${PROJECT_NAME} ${SRC_LIST} ${SRC_DIR} ${INCLUDE_DIR} ${INCLUDE_LIBS})

find_package(opencascade REQUIRED COMPONENTS lTKIGES lTKSTEP lTKXSBase lTKBO lTKBRep
    lTKBool lTKFeat lTKFillet lTKG2d lTKG3d lTKGeomAlgo
    lTKGeomBase lTKHLR lTKMath lTKMesh lTKOffset lTKPrim
    lTKShHealing lTKTopAlgo lTKXMesh lTKernel
    lTKTopAlgo lTKBRep lTKernel ltk ltcl lGL ltbb lXmu lgl2ps)

target_link_libraries(${PROJECT_NAME} opencascade)
