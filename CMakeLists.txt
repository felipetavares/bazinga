# Compila a partir do CMake2.8
cmake_minimum_required(VERSION 2.8)

ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    # Other flags
)

# Detecta a plataforma na qual vamos compilar
# TODO
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	SET(OS "linux")
ELSE ()
	SET(OS "windows")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Vamos compilar a Bazinga Engine
project (Bazinga)

# Adiciona bibliotecas Lua ao projeto
include_directories (${CMAKE_SOURCE_DIR}/lua/lua-5.2.2/src/)

# Adiciona bibliotecas SDL ao projeto
include_directories (${CMAKE_SOURCE_DIR}/sdl/include/SDL/)

# Adiciona bibliotecas DevIL ao projeto
include_directories (${CMAKE_SOURCE_DIR}/devil/include/IL/)

# Adiciona lib lua
add_library(lualib STATIC IMPORTED)
set_target_properties(lualib PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lua/lua-5.2.2/src/liblua.a")

# Adiciona libs da SDL (se estiver no windows)
IF (${OS} EQUAL "windows")
add_library(libSDL STATIC IMPORTED)
set_target_properties(libSDL PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/sdl/windows/libSDL.dll.a")
add_library(libSDLmain STATIC IMPORTED)
set_target_properties(libSDLmain PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/sdl/windows/libSDLmain.a")
# Adiciona libs da DevIL
add_library(devil STATIC IMPORTED)
set_target_properties(devil PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/build/DevIL.lib")
ENDIF(${OS} EQUAL "windows")

# Compila Lua 5.2.2
ADD_CUSTOM_TARGET(
  lua.o ALL
  COMMAND ${CMAKE_MAKE_PROGRAM} posix
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/lua/lua-5.2.2
COMMENT "Compilando Lua 5.2.2 (com o Makefile original)")

# Compila o executável principal
add_executable (bazinga bazinga.cpp json.cpp filesystem.cpp object.cpp map.cpp video.cpp cache.cpp)

# Linka com (opengl, lua, sdl, devil)
IF (${OS} EQUAL "windows")
target_link_libraries (bazinga glu32 opengl32)
target_link_libraries (bazinga mingw32)
target_link_libraries (bazinga libSDLmain)
target_link_libraries (bazinga libSDL)
ENDIF (${OS} EQUAL "windows")

target_link_libraries (bazinga lualib)

IF (${OS} EQUAL "windows")
target_link_libraries (bazinga devil)
ELSE ()
target_link_libraries (bazinga IL GL SDL)
ENDIF ()
# Dependencias
ADD_DEPENDENCIES (lualib lua.o)
