# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\GeometryVisualizer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\GeometryVisualizer_autogen.dir\\ParseCache.txt"
  "GeometryVisualizer_autogen"
  )
endif()
