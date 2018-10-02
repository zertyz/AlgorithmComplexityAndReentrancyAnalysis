cmake_minimum_required (VERSION 2.6)

project (AlgorithmComplexityAndReentrancyAnalysisSpikes)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_CXX_FLAGS -g)
set(CMAKE_CXX_FLAGS -O3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(AlgorithmComplexityAndReentrancyAnalysisSpikes AlgorithmComplexityAndReentrancyAnalysisSpikes.cpp ../../../../CppUtils/cpp/TimeMeasurements.h ../../../../CppUtils/cpp/TimeMeasurements.cpp ../../cpp/AlgorithmComplexityAndReentrancyAnalysis.h ../../cpp/AlgorithmComplexityAndReentrancyAnalysis.cpp ../../../SplitRun/cpp/SplitRun.h ../../../SplitRun/cpp/SplitRun.cpp)
