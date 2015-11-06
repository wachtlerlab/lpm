cmake_minimum_required(VERSION 2.8.4)
project(lpm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

########################################
# Boost

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.49.0 REQUIRED program_options)

include_directories(${Boost_INCLUDE_DIR})
set(LINK_LIBS ${LINK_LIBS} ${Boost_LIBRARIES})

########################################
# OpenGL + Co
find_package(OpenGL REQUIRED)
#find_package(GLM REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(FREETYPE REQUIRED freetype2)

include_directories(${GLFW_INCLUDE_DIRS}
                    ${GLM_INCLUDE_DIRS}
                    ${OpenGL_INCLUDE_DIRS}
                    ${FREETYPE_INCLUDE_DIRS})

link_directories(${GLFW_LIBRARY_DIRS}
                 ${GLM_LIBRARY_DIRS}
                 ${FREETYPE_LIBRARY_DIRS}
                 ${OpenGL_LIBRARY_DIRS})

set(LINK_LIBS ${LINK_LIBS} ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} ${FREETYPE_LIBRARIES})

#######################################
# YAML CPP

pkg_check_modules(YAMLCPP REQUIRED yaml-cpp)

include_directories(${YAMLCPP_INCLUDE_DIRS})
link_directories(${YAMLCPP_LIBRARY_DIRS})
set(LINK_LIBS ${LINK_LIBS} ${YAMLCPP_LIBRARIES})

########################################
# IRIS

find_package(IRIS REQUIRED)
include_directories (${IRIS_INCLUDE_DIRS})
set (LINK_LIBS ${LINK_LIBS} ${IRIS_LIBRARIES})

#########################################
# Linux

if(NOT APPLE)
  pkg_search_module(GLEW REQUIRED glew)
  include_directories(${GLEW_INCLUDE_DIRS})
  link_directories(${GLEW_LIBRARY_DIRS})
  set(LINK_LIBS ${LINK_LIBS} ${GLEW_LIBRARIES})

  set(LINK_LIBS ${LINK_LIBS} -lstdc++ -lm)
endif()

#########################################
set(lpm_SOURCES lpm.cc)
add_executable(lpm ${lpm_SOURCES})
target_link_libraries(lpm iris)

set(LEDPhotoSpectrum_SOURCES LEDPhotoSpectrum.cc)
add_executable(lpm-LEDPhotoSpectrum ${LEDPhotoSpectrum_SOURCES})
target_link_libraries(lpm-LEDPhotoSpectrum iris)

set(ledPWMthresholder_SOURCES ledPWMthresholder.cc)
add_executable(lpm-ledPWMthresholder ${ledPWMthresholder_SOURCES})
target_link_libraries(lpm-ledPWMthresholder iris)

#########################################
# installation

install(TARGETS lpm lpm-LEDPhotoSpectrum lpm-ledPWMthresholder
        RUNTIME DESTINATION bin
        COMPONENT applications)