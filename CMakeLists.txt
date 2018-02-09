cmake_minimum_required (VERSION 2.6)
project (kit2d)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

file(GLOB SOURCE_FILES
  "src/*.cpp"
)

add_library(kit2d STATIC ${SOURCE_FILES})
target_link_libraries(kit2d ${SDL2_LIBRARY})

set_property(TARGET kit2d PROPERTY CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
