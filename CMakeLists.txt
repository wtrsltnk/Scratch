cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include(cmake/CPM.cmake)
CPMAddPackage(
  NAME glfw
  GITHUB_REPOSITORY glfw/glfw
  GIT_TAG 3.3.2
  OPTIONS
    "CXXOPTS_BUILD_EXAMPLES Off"
    "CXXOPTS_BUILD_TESTS Off"
)

project(Scratch)

add_executable(Scratch
    visibility.cpp
    main.cpp
    Tracer.cpp
    Camera.cpp
    #Json.cpp
    Shader.cpp
    uiFont.cpp
    Hl1BspData.cpp
    stb_truetype.cpp
    Hl1DataManager.cpp
    SpacePartitioner.cpp
    vector3.cpp
    lightmap.cpp
    StaticGeometry.cpp
    SunShadows.cpp
    texture.cpp
    glad.c
)


target_link_libraries(Scratch glfw)

target_include_directories(Scratch
    PUBLIC include
)
