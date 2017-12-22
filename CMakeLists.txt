cmake_minimum_required(VERSION 3.0)

project(game)

include_directories(
  "${CMAKE_SOURCE_DIR}/include"
)

link_directories(
  ${CMAKE_SOURCE_DIR}/lib
)

file(GLOB SOURCE_FILES
  "src/*.cpp"
)

add_executable(
  game
  ${SOURCE_FILES}
)

# Make release exe portable (not require VC++ Runtime)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
if(WIN32)
  set_target_properties(game PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif(WIN32)

# Necessary to find CMake modules
set(CMAKE_MODULE_PATH 
  "${CMAKE_SOURCE_DIR}/cmake_modules" 
  ${CMAKE_MODULE_PATH}
)

# Necessary to find packages
set(CMAKE_PREFIX_PATH
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_PREFIX_PATH}
)

set(SFML_STATIC_LIBRARIES TRUE)
find_package(
  SFML 2 REQUIRED network audio graphics window system
)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(game ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
  include_directories(${OPENGL_INCLUDE_DIRS})
  target_link_libraries(game ${OPENGL_LIBRARIES})
endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIRS})
  target_link_libraries(game ${GLEW_LIBRARIES})
endif()
