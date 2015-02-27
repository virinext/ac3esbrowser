project(ac3parser)

set(SOURCES
  src/AC3Parser.cpp
  src/AC3ParserImpl.cpp
  src/BitstreamReader.cpp
)

include_directories(
	include/
)

ADD_DEFINITIONS(-std=c++0x) 

add_library(ac3parser ${SOURCES}) 


find_package(Boost COMPONENTS system unit_test_framework REQUIRED)

set(TEST_SOURCES
  test/main.cpp
  test/Params.cpp
  test/Parsing.cpp
)

ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK) 


add_executable(ac3parser_test ${TEST_SOURCES}) 
target_link_libraries(ac3parser_test ac3parser ${Boost_LIBRARIES})
add_dependencies(ac3parser_test ac3parser)

add_test(ac3parser_test ${BUILD_DIR}/ac3parser_test)
enable_testing()
