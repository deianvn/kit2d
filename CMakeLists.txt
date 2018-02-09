cmake_minimum_required (VERSION 2.6)
project (kit2d)
set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

file(GLOB SOURCE_FILES
  "src/*.cpp"
)
add_library(kit2d ${SOURCE_FILES})
target_link_libraries(kit2d ${SDL2_LIBRARIES})

add_executable(example example/example.cpp)
target_link_libraries(example kit2d)
