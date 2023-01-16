# CMakeList.txt : CMake project for word_count, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (word_count "WordCounter.cpp" "WordCounter.h" "FileReader.cpp" "FileReader.h"  
						   "TextProcessor.h" "TextProcessor.cpp" "JobsRunner.h" "JobsRunner.cpp" 
						   "MultiThreadWordCounter.h" "MultiThreadWordCounter.cpp"
						    "SingleThreadWordCounter.cpp" "SingleThreadWordCounter.h" "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET word_count PROPERTY CXX_STANDARD 20)
endif()