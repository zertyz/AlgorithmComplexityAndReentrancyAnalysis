# test this CMakeLists.txt in a standalone fashion as:
#     TYPE="Release"; rm -fr $TYPE; mkdir $TYPE; cd $TYPE; cmake -G Ninja -DCMAKE_BUILD_TYPE=$TYPE ..; cmake --build .; ctest; cd ..; ls -l "$TYPE/AlgorithmComplexityAndReentrancyAnalysisSpikes"
# The rules here should be used when importing this library into other projects
cmake_minimum_required (VERSION 3.9)

project (AlgorithmComplexityAndReentrancyAnalysisSpikes VERSION 2018.10.09)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


add_executable(AlgorithmComplexityAndReentrancyAnalysisSpikes AlgorithmComplexityAndReentrancyAnalysisSpikes.cpp)
target_link_libraries(AlgorithmComplexityAndReentrancyAnalysisSpikes PRIVATE mutua::AlgorithmComplexityAndReentrancyAnalysis)

# imported mutua libraries
##########################
# mutua libraries have the inclues inside the "cpp/" directory
foreach (_referencedLib AlgorithmComplexityAndReentrancyAnalysis CppUtils SplitRun)
	message("Referencing library '${_referencedLib}' in '${${_referencedLib}_DIR}'.")
	if (NOT TARGET ${_referencedLib})
		find_package(${_referencedLib} CONFIG REQUIRED)
		add_library (${_referencedLib} STATIC IMPORTED GLOBAL)
	else()
		add_library(mutua::${_referencedLib} ALIAS ${_referencedLib})
	endif()
	include_directories("../../../${_referencedLib}/cpp/")
	target_link_libraries(AlgorithmComplexityAndReentrancyAnalysisSpikes PRIVATE mutua::${_referencedLib})
endforeach()

enable_testing()
add_test(NAME "${PROJECT_NAME}" COMMAND "./AlgorithmComplexityAndReentrancyAnalysisSpikes")
