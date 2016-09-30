
cmake_minimum_required(VERSION 2.8)


PROJECT(NewMarchingCubes)


find_package(VTK REQUIRED)

include(${VTK_USE_FILE})


add_executable(NewMarchingCubes MACOSX_BUNDLE  NewMarchingCubes.cxx                          vtkMarchingCubesSpace.h vtkMarchingCubesSpace.cxx
                         vtkMarchingCubesSpaceCases.h                          vtkMarchingCubesSpaceCases.cxx )



if(VTK_LIBRARIES)
 
  target_link_libraries(NewMarchingCubes ${VTK_LIBRARIES})

else()
  
  target_link_libraries(NewMarchingCubes vtkHybrid vtkWidgets)

endif()
