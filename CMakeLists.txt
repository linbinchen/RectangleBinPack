cmake_minimum_required (VERSION 3.0)
project (RectBinPack)
add_subdirectory (src lib)
add_subdirectory (HybMatLayout bin)
## Enable Generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
