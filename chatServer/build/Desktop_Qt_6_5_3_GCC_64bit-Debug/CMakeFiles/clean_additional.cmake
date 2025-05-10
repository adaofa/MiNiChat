# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/chatServer_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/chatServer_autogen.dir/ParseCache.txt"
  "chatServer_autogen"
  )
endif()
