cmake_minimum_required(VERSION 3.31)

PROJECT(mh_p1 CXX)

file(GLOB C_SOURCES
  "src/*.cpp"
  "src/P1/*.cpp"
  "src/P2/*.cpp"
  "src/P3/*.cpp"
)
INCLUDE_DIRECTORIES("common" "inc" "inc/P1" "inc/P2" "inc/P3")

ADD_EXECUTABLE(main "main.cpp" ${C_SOURCES})
