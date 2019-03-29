cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

#set(CMAKE_C_COMPILER "/usr/local/gcc/5.2.0/bin/gcc")
#set(CMAKE_CXX_COMPILER "/usr/local/gcc/5.2.0/bin/g++")

set(name ComptonCamera)

project(${name})

#set(Geant4_DIR ${Geant4_DIR} "/afs/in2p3.fr/system/amd64_sl6/usr/local/geant4/geant4.10.02.p02/lib64/Geant4-10.2.2")


option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})

execute_process(COMMAND root-config --libs OUTPUT_VARIABLE ROOTLIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

include_directories(${PROJECT_SOURCE_DIR}/include)

# We presume the existence of three directories
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB_RECURSE macros RELATIVE ${PROJECT_SOURCE_DIR} macros/*.mac)

# Enable macros for out-of-source build
foreach(_file ${macros})
  configure_file(
    ${_file}
    ${PROJECT_BINARY_DIR}/${_file}
    COPYONLY
    )
endforeach()

IF(UNIX)
	IF(CMAKE_COMPILER_IS_GNUCC)
		add_definitions(-Wall)				### ALL WARNINGS PRINTED
		add_definitions(-g)				### COMPILE WITH DEBUGGING SYMBOLS
		add_definitions(`root-config --cflags`)		### ROOT CFLAGS
		add_definitions(-DG4VIS_NONE)			### TO DEACTIVATE VISUALIZATION
	ENDIF(CMAKE_COMPILER_IS_GNUCC)
ENDIF(UNIX)

add_executable(${name} ComptonCamera.cc ${sources} ${headers})
target_link_libraries(${name}  ${ROOTLIBS} ${Geant4_LIBRARIES})
 

#set_property(TARGET ${name} PROPERTY CXX_STANDARD 11)
#set_property(TARGET ${name} PROPERTY CXX_STANDARD_REQUIRED ON)

install(TARGETS ${name} DESTINATION bin)
