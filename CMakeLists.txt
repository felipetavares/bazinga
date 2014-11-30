# Compila a partir do CMake2.8
cmake_minimum_required(VERSION 2.8)

# Vamos compilar a Bazinga Engine
project (Bazinga)

# Adiciona bibliotecas Lua ao projeto
include_directories (lua/lua-5.2.2/src/)

# Adiciona bibliotecas SDL ao projeto
include_directories (sdl/include/SDL/)

# Adiciona lib lua
add_library(lualib STATIC IMPORTED)
set_target_properties(lualib PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lua/lua-5.2.2/src/liblua.a")

# Adiciona libs da SDL
add_library(libSDL STATIC IMPORTED)
set_target_properties(libSDL PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/sdl/windows/libSDL.dll.a")
add_library(libSDLmain STATIC IMPORTED)
set_target_properties(libSDLmain PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/sdl/windows/libSDLmain.a")

# Compila Lua 5.2.2
ADD_CUSTOM_TARGET(
  lua.o ALL
  COMMAND ${CMAKE_MAKE_PROGRAM} ansi
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/lua/lua-5.2.2
COMMENT "Compilando Lua 5.2.2 (com o Makefile original)")

# Compila o executável principal
add_executable (bazinga bazinga.cpp json.cpp filesystem.cpp)

# Linka com (lua, sdl)
target_link_libraries (bazinga mingw32)
target_link_libraries (bazinga libSDLmain)
target_link_libraries (bazinga libSDL)
target_link_libraries (bazinga lualib)

# Dependencias
ADD_DEPENDENCIES (lualib lua.o)
ADD_DEPENDENCIES (bazinga libSDLmain)
