cmake_minimum_required(VERSION 3.22.1)

PROJECT(mh_p3 CXX)

file(GLOB C_SOURCES
  "src/*.cpp"
)
INCLUDE_DIRECTORIES("common" "inc")

ADD_EXECUTABLE(main "main.cpp" ${C_SOURCES})

ADD_EXECUTABLE(savetofile "savetofile.cpp" ${C_SOURCES})