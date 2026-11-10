cmake_minimum_required(VERSION 3.10)
project(kit2d)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules/")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(SDL2 2.0.18 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)

include_directories(${CMAKE_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS})

# SDL2_image's INTERFACE_INCLUDE_DIRECTORIES points to the SDL2/ subdir;
# we need its parent so that #include <SDL2/SDL_image.h> resolves correctly.
get_target_property(_sdl2_image_incdirs SDL2_image::SDL2_image INTERFACE_INCLUDE_DIRECTORIES)
foreach(_dir ${_sdl2_image_incdirs})
    get_filename_component(_parent "${_dir}" DIRECTORY)
    include_directories(${_parent})
endforeach()

file(GLOB CORE_SOURCE_FILES "src/core/*.cpp")
add_library(kit2d ${CORE_SOURCE_FILES})
target_link_libraries(kit2d SDL2::SDL2 SDL2_image::SDL2_image)

file(GLOB SPRITE_SOURCE_FILES "src/sprite/*.cpp")
add_library(kit2d-sprite ${SPRITE_SOURCE_FILES})

file(GLOB SCENE_SOURCE_FILES "src/scene/*.cpp")
add_library(kit2d-scene ${SCENE_SOURCE_FILES})

file(GLOB EXAMPLE_SOURCE_FILES "example/*.cpp")
add_executable(bricks ${EXAMPLE_SOURCE_FILES})
target_link_libraries(bricks kit2d kit2d-sprite kit2d-scene)

file(COPY ${CMAKE_SOURCE_DIR}/example/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include(CTest)
if(BUILD_TESTING)
    add_executable(sprite-tests tests/SpriteTests.cpp)
    target_link_libraries(sprite-tests kit2d-sprite kit2d)
    add_test(NAME sprite-animation COMMAND sprite-tests)
    add_executable(sprite-batch-tests tests/SpriteBatchTests.cpp)
    target_link_libraries(sprite-batch-tests kit2d-sprite kit2d)
    add_test(NAME sprite-batching COMMAND sprite-batch-tests)
    add_executable(actor-tests tests/ActorTests.cpp)
    target_link_libraries(actor-tests kit2d-scene kit2d-sprite kit2d)
    add_test(NAME actor-behavior COMMAND actor-tests)
    add_executable(bricks-tests tests/BricksTests.cpp example/Board.cpp example/Brick.cpp)
    target_link_libraries(bricks-tests kit2d-scene kit2d-sprite kit2d)
    add_test(NAME bricks-gameplay COMMAND bricks-tests)
endif()
