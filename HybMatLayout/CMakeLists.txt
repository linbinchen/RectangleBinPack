cmake_minimum_required (VERSION 3.0)
add_executable (hybmatlayout HybMatLayout.cpp)

## Compile Config ##
find_package(boost REQUIRED)
target_include_directories (hybmatlayout
    PUBLIC
        /opt/X11/include
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_compile_features (hybmatlayout PUBLIC cxx_std_11)


## Link Config ##
set_target_properties (hybmatlayout PROPERTIES LINK_FLAGS -L/opt/X11/lib)
target_link_libraries (hybmatlayout mrbp plotter Xaw Xt SM ICE Xext X11 png z m
    boost_program_options )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
