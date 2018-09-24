
cmake_minimum_required(VERSION 3.10.2)

project(inFINITE VERSION 1.0.0.1 DESCRIPTION "Finite Element Analysis Program")

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_INCLUDE_CURRENT_DIR ON)


# VTK directory
set(VTK_DIR "/opt/vtk8r/lib/cmake/vtk-8.1" CACHE PATH "VTK directory override" FORCE)
# QT directories
set(Qt5_DIR "/opt/qt-5.9.1/5.9.1/gcc_64/lib/cmake/Qt5")
set(Qt5Core_DIR "/opt/qt-5.9.1/5.9.1/gcc_64/lib/cmake/Qt5Core")
set(Qt5Gui_DIR "/opt/qt-5.9.1/5.9.1/gcc_64/lib/cmake/Qt5Gui")
set(Qt5Widgets_DIR "/opt/qt-5.9.1/5.9.1/gcc_64/lib/cmake/Qt5Widgets")
set(Qt5OpenGL_DIR "/opt/qt-5.9.1/5.9.1/gcc_64/lib/cmake/Qt5OpenGL")
# Expert libraries directories eg. Atlas, Magma, Cuda
set(E_LIB_DIR "/opt")
# Built libraries directory
set(B_LIB_DIR "~/Projects/library")

#include directories

# find libraries

# List source files & resources
file (GLOB_RECURSE sources "source/*.cpp")
file (GLOB_RECURSE headers "include/*.h")
file (GLOB_RECURSE resources "resources/*.qrc")
file (GLOB_RECURSE UIs "UIs/*.ui")
include_directories("./include")

# Find package VTK
find_package(VTK REQUIRED)

# Use VTK
include(${VTK_USE_FILE})
include_directories(${VTK_INCLUDE_DIRS})

# Find package Qt
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

# Use Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
qt5_add_resources(Resources ${resources})
set(CMAKE_AUTOUIC_SEARCH_PATHS "./UIs")
unset(QT_QMAKE_EXECUTABLE)

# Definitions
#add_definitions(-DFEA_LIBRARY)
#add_definitions(-DADD_)
#add_definitions(-DMTH_NUM_THREADS=8)
#add_definitions(-DQT_DEPRECATED_WARNINGS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Log library
include("~/Projects/library/log/FindLog.cmake")
include_directories("~/Projects/library/log/include")

# Mth library
include("~/Projects/library/mth/FindMth.cmake")
include_directories("~/Projects/library/mth/include")

# Fea library
include("~/Projects/library/fea/FindFea.cmake")
include_directories("~/Projects/library/fea/include")

# Gph library
include("~/Projects/library/gph/FindGph.cmake")
include_directories("~/Projects/library/gph/include")

# Build executable
add_executable(inFINITE ${sources} ${headers} ${resources} ${Resources} ${UIs})

# Link libraries
target_link_libraries(inFINITE ${VTK_LIBRARIES} fea gph mth)




