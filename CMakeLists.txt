cmake_minimum_required (VERSION 3.0.2)
project (kit2d)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules/")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})

file(GLOB CORE_SOURCE_FILES
  "src/core/*.cpp"
)
add_library(kit2d ${CORE_SOURCE_FILES})
target_link_libraries(kit2d ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

file(GLOB SPRITE_SOURCE_FILES
  "src/sprite/*.cpp"
)
add_library(kit2d-sprite ${SPRITE_SOURCE_FILES})

add_executable(example example/example.cpp)
target_link_libraries(example kit2d kit2d-sprite)
