
include_directories("../../pb_shared/src")
include_directories("../../pb_wrap/src")

include_directories("./")

add_executable(pbsam
  ../../pb_shared/src/BaseSys.cpp
  BDSAM.cpp
  ../../pb_shared/src/BesselCalc.cpp
  ../../pb_shared/src/Constants.cpp
  ElectrostaticsSAM.cpp
  Gradsolvmat.cpp  
  main.cpp
  PBSAM.cpp 
  PhysCalcSAM.cpp
  ../../pb_shared/src/ReExpCalc.cpp
  ../../pb_shared/src/setup.cpp
  ../../pb_shared/src/SHCalc.cpp
  Solver.cpp
  Solvmat.cpp
  SystemSAM.cpp
  TMatrix.cpp
  )

################################################
################################################
##### For APBS build of PBSAM
################################################
option(ENABLE_PBSAM_APBS "pbsam APBS" OFF)
if(ENABLE_PBSAM_APBS)
  add_definitions(-DPBSAM_APBS)
  include_directories(
                      ../../../../src/ 
                      ../../../../tools/windows/configuration\ headers/
                      ../../../../externals/fetk/maloc/src/base
                      ../../../../externals/fetk/maloc/src/vsys
                      ../../../../externals/fetk/maloc/src/vsh
                      ../../../../externals/fetk/maloc/src/psh
                      )

  #  build pbsam library
  add_library(PBSAMLib 
              ../../pb_shared/src/BaseSys.cpp
			  BDSAM.cpp
              ../../pb_shared/src/BesselCalc.cpp
              ../../pb_shared/src/Constants.cpp
              ElectrostaticsSAM.cpp
              Gradsolvmat.cpp  
              PBSAM.cpp 
              PhysCalcSAM.cpp
              ../../pb_wrap/src/PBSAMWrap.cpp
              ../../pb_shared/src/ReExpCalc.cpp
              ../../pb_shared/src/setup.cpp
              ../../pb_shared/src/SHCalc.cpp
              Solver.cpp
              Solvmat.cpp
              SystemSAM.cpp
              TMatrix.cpp
              )

  MESSAGE( STATUS "linkers: " ${APBS_LIBS})
  list(APPEND APBS_LIBS ${PBSAM_LINKER_LIBS})
  MESSAGE( STATUS "linkers: " ${APBS_LIBS})

endif()

################################################
TARGET_LINK_LIBRARIES( pbsam ${PBSAM_LINKER_LIBS})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
install(TARGETS pbsam RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin)
