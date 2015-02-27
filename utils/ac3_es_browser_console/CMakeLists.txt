project(ac3esbrowser_console)

set(SOURCES
	src/main.cpp
	src/AC3InfoWriter.cpp
)

find_package(Boost COMPONENTS program_options system REQUIRED)

include_directories(
	${Boost_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/../../ac3parser/include/
)

ADD_DEFINITIONS(-std=c++0x) 

set(LIBS
	ac3parser
	${Boost_LIBRARIES}
)

add_executable(AC3ESBrowser_console ${SOURCES}) 
target_link_libraries(AC3ESBrowser_console ${LIBS})
add_dependencies(AC3ESBrowser_console ac3parser)
