# Compila a partir do CMake2.8
cmake_minimum_required(VERSION 2.8)

# Vamos compilar a Bazinga Engine
project (Bazinga)

# Adiciona bibliotecas Lua ao projeto
include_directories (lua/lua-5.2.2/src/)

# Compila Lua 5.2.2
ADD_CUSTOM_TARGET(
lua.o ALL
COMMAND ${CMAKE_MAKE_PROGRAM} linux
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lua/lua-5.2.2
COMMENT "Original lua-5.2.2 makefile target")

# Adiciona lib lua
add_library(lualib SHARED IMPORTED)
set_target_properties(lualib PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lua/lua-5.2.2/src/liblua.a)

# Compila o executável principal
add_executable (bazinga bazinga.cpp)

# Linka com lua
target_link_libraries (bazinga LINK_PUBLIC lualib)
