project(${CMAKE_PROJECT_NAME}_lib)

set(SOURCES lexer.cpp)

set(HEADERS headers/lexer.h)

add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCES} ${HEADERS})