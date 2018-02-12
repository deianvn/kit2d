cmake_minimum_required (VERSION 3.0.2)
project (kit2d)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})

file(GLOB SOURCE_FILES
  "src/*.cpp"
)
add_library(kit2d ${SOURCE_FILES})
target_link_libraries(kit2d ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

add_executable(example example/example.cpp)
target_link_libraries(example kit2d)
