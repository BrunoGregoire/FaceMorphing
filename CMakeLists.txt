cmake_minimum_required(VERSION 2.8)
 
PROJECT(FaceMorphing)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

#find qt
FIND_PACKAGE(Qt5Widgets REQUIRED)
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDES})
ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})
SET(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

#find vtk
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

#find dlib
include_directories("C:/dlib")
#include(C:/dlib/dlib/cmake)

#find opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

#include folder
FILE(GLOB_RECURSE INC_ALL "Include/*.h")
include_directories("Include")

#add src
add_executable(FaceMorphing MACOSX_BUNDLE Main UserInterface vtkModel vtkMeshModel vtkGlyphModel vtkLineModel vtkVectorTextModel Content Initializer InteractorStyle ICP TPST Texturer Raycaster FaceDetector ${INC_ALL})
 
#link with qt
TARGET_LINK_LIBRARIES(FaceMorphing ${Qt5Widgets_LIBRARIES})

#link with vtk
if(VTK_LIBRARIES)
  target_link_libraries(FaceMorphing ${VTK_LIBRARIES})
else()
  target_link_libraries(FaceMorphing vtkHybrid vtkWidgets)
endif()

#link with opencv
TARGET_LINK_LIBRARIES(FaceMorphing ${OpenCV_LIBS} )

#link with dlib
TARGET_LINK_LIBRARIES(FaceMorphing C:/dlib/dlib/bin/Release/dlib.lib)
