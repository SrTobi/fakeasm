cmake_minimum_required(VERSION 3.5)
project(fakeasm)

find_package(BISON)
find_package(FLEX)

set(LIBFIRM_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/extra/libfirm/include ${CMAKE_CURRENT_SOURCE_DIR}/extra/libfirm/build/gen/include/libfirm)
set(LIBFIRM_LIB ${CMAKE_CURRENT_SOURCE_DIR}/extra/libfirm/build/debug/libfirm.a)

set(SOURCE_FILES main.c llist.c parse.c ast.c print.c predef.c gen.c strmap.c)

BISON_TARGET(FAParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
FLEX_TARGET(FALexer lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
ADD_FLEX_BISON_DEPENDENCY(FALexer FAParser)


include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${LIBFIRM_INCLUDES})
add_executable(fakeasm ${SOURCE_FILES} ${BISON_FAParser_OUTPUTS} ${FLEX_FALexer_OUTPUTS})
target_link_libraries (fakeasm firm m)