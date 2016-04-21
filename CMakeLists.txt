cmake_minimum_required(VERSION 3.5)
project(fakeasm)

find_package(BISON)
find_package(FLEX)

set(SOURCE_FILES main.c llist.c parse.c ast.c print.c)

BISON_TARGET(FAParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
FLEX_TARGET(FALexer lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
ADD_FLEX_BISON_DEPENDENCY(FALexer FAParser)


include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(fakeasm ${SOURCE_FILES} ${BISON_FAParser_OUTPUTS} ${FLEX_FALexer_OUTPUTS})