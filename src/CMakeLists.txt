set(main_source_files
 main.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(Wt REQUIRED Wt HTTP)

add_library(main ${main_source_files})
target_link_libraries(main config filedrop db ${Boost_LIBRARIES} Threads::Threads  Wt::Wt Wt::HTTP ${OpenCASCADE_LIBRARIES})
