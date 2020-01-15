set(fxtract_source_files
   ModelApp.cpp
   ModelApp.h
   ModelMath.cpp
   ModelMath.h
   ModelFace.cpp
   ModelFace.h
   ModelUtils.cpp
   ModelUtils.h
   ModelReaderClass.cpp
   ModelReaderClass.h
   ModelEdge.cpp
   ModelEdge.h
   ModelVertex.cpp
   ModelVertex.h
   StepProcessor.cpp
   StepProcessor.h
   IgesProcessor.cpp
   IgesProcessor.h
)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(wt COMPONENTS wt wtdbo wtdbosqlite3 wthttp REQUIRED)
find_package(OpenCASCADE REQUIRED)

add_library(fxtract ${fxtract_source_files})
target_include_directories(fxtract PRIVATE ${Boost_INCLUDE_DIRS} ${wt_INCLUDE_DIRS} ${OpenCASCADE_INCLUDE_DIR})
target_link_libraries(fxtract ${Boost_LIBRARIES} Threads::Threads ${wt_LIBRARIES} wthttp wt wtdbo wtdbosqlite3 ${OpenCASCADE_LIBRARIES})
