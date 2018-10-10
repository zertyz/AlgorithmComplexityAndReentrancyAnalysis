# Usage:
#	TYPE="Debug";   rm -fr $TYPE; mkdir $TYPE; cd $TYPE; cmake -G Ninja -DCMAKE_BUILD_TYPE=$TYPE ..; cmake --build .; ctest; cd ..; ls -l "$TYPE/tests/cpp/AlgorithmComplexityAndReentrancyAnalysisSpikes"
#	TYPE="Release"; rm -fr $TYPE; mkdir $TYPE; cd $TYPE; cmake -G Ninja -DCMAKE_BUILD_TYPE=$TYPE ..; cmake --build .; ctest; cd ..; ls -l "$TYPE/tests/cpp/AlgorithmComplexityAndReentrancyAnalysisSpikes"

cmake_minimum_required (VERSION 3.9)
project (AlgorithmComplexityAndReentrancyAnalysis VERSION 2018.10.09)

set(CMAKE_CXX_STANDARD 17)


# imported mutua libraries
##########################
# mutua libraries have the inclues inside the "cpp/" directory
set(LIBs_H_FILES, "")
foreach (_referencedLib CppUtils SplitRun)
	message("Referencing library '${_referencedLib}' in '${${_referencedLib}_DIR}'.")
	find_package(${_referencedLib} CONFIG REQUIRED)
	add_library (${_referencedLib} STATIC IMPORTED GLOBAL)
	#target_include_directories(${_referencedLib} INTERFACE )
	include_directories("../${_referencedLib}/cpp/")
	#file(GLOB_RECURSE THIS_LIB_H_FILES ../${_referencedLib}/cpp/*.h)
	#message(" --> this lib .h: ${THIS_LIB_H_FILES}")
	#set(LIBs_H_FILES "${LIBs_H_FILES} ${THIS_LIB_H_FILES}")
endforeach()

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

# this lib
##########

file(GLOB_RECURSE CPP_FILES cpp/*.cpp)
file(GLOB_RECURSE H_FILES   cpp/*.h)
message("Building '${PROJECT_NAME}' library with:")
message("      .h files: [${H_FILES}]")
message("    .cpp files: [${CPP_FILES}]")
#message("other .h files: [${LIBs_H_FILES}]")
#add_library(${PROJECT_NAME} ${LIBs_H_FILES} ${H_FILES} ${CPP_FILES})
add_library(${PROJECT_NAME} ${H_FILES} ${CPP_FILES})

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PUBLIC "${PROJECT_NAME}_DEBUG=$<CONFIG:Debug>")
#target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} INTERFACE
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpp/>  
                           $<INSTALL_INTERFACE:cpp/>)                             # <prefix>/cpp/*.h

install(TARGETS ${PROJECT_NAME}  EXPORT      ${PROJECT_NAME}Targets
        LIBRARY                  DESTINATION lib
        ARCHIVE                  DESTINATION lib
        RUNTIME                  DESTINATION bin
        INCLUDES                 DESTINATION include)
install(FILES   ${H_FILES}       DESTINATION include COMPONENT Devel)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${Upstream_VERSION}
                                 COMPATIBILITY AnyNewerVersion
)

export(EXPORT ${PROJECT_NAME}Targets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Targets.cmake"
       NAMESPACE mutua::)

configure_file(cmake/${PROJECT_NAME}Config.cmake
               "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake"
               COPYONLY)

set(ConfigPackageLocation lib/cmake/${PROJECT_NAME})
install(EXPORT      ${PROJECT_NAME}Targets
        FILE        ${PROJECT_NAME}Targets.cmake
        NAMESPACE   mutua::
        DESTINATION ${ConfigPackageLocation})
install(FILES       cmake/${PROJECT_NAME}Config.cmake "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${ConfigPackageLocation}
        COMPONENT   Devel)

# export to user registry at ~/.cmake, so all other projects can refer to it with "find_package"
export(PACKAGE ${PROJECT_NAME})

enable_testing()
add_subdirectory("tests/cpp")

#add_executable(AlgorithmComplexityAndReentrancyAnalysisSpikes AlgorithmComplexityAndReentrancyAnalysisSpikes.cpp ../../../../CppUtils/cpp/TimeMeasurements.h ../../../../CppUtils/cpp/TimeMeasurements.cpp ../../cpp/AlgorithmComplexityAndReentrancyAnalysis.h ../../cpp/AlgorithmComplexityAndReentrancyAnalysis.cpp ../../../SplitRun/cpp/SplitRun.h ../../../SplitRun/cpp/SplitRun.cpp)

