cmake_minimum_required(VERSION 3.19)
project(OpenGLTests)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(OpenGLTests
        Boundary.cpp
        Boundary.h
        Graph.cpp
        Graph.h
        imconfig.h
        imgui.cpp
        imgui.h
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_impl_glfw.cpp
        imgui_impl_glfw.h
        imgui_impl_opengl3.cpp
        imgui_impl_opengl3.h
        imgui_impl_opengl3_loader.h
        imgui_internal.h
        imgui_tables.cpp
        imgui_widgets.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        main.cpp
        Mesh.cpp
        Mesh.h
        PlaneSweepTriangulation.cpp
        PlaneSweepTriangulation.h
        Points.cpp
        Points.h
        RandomizedIncrementalTriangulation.cpp
        RandomizedIncrementalTriangulation.h
        Shader.cpp
        Shader.h
        Triples.cpp
        Triples.h
        Window.cpp
        Window.h)

target_link_libraries(OpenGLTest -lOpenGL32 -lfreeGlut)