set(BINARY ${CMAKE_PROJECT_NAME}_test)

set(SOURCES test_lexer_iterator.cpp main.cpp test_lexer.cpp)

add_executable(${BINARY} ${SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)