cmake_minimum_required (VERSION 3.0)
add_library (mrbp Rect.cpp
                  MaxRectsBinPack.cpp
)
target_include_directories (mrbp
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
