# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\life_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\life_autogen.dir\\ParseCache.txt"
  "life_autogen"
  )
endif()
