cmake_minimum_required(VERSION 3.2)
project(PRScal VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
execute_process(COMMAND date OUTPUT_VARIABLE DATE OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND whoami OUTPUT_VARIABLE USER OUTPUT_STRIP_TRAILING_WHITESPACE)

add_definitions(-DVERSION="${PROJECT_VERSION}" -DUSER="${USER}" -DDATE="${DATE}")

find_package(ZLIB REQUIRED)
find_library(STATGEN_LIBRARY StatGen)
add_executable(PRScal
	src/Main.cpp
        src/UserVariables.h src/Analysis.cpp src/Analysis.h src/DosageFile.cpp src/DosageFile.h)
target_link_libraries(PRScal ${STATGEN_LIBRARY} ${ZLIB_LIBRARIES})

install(TARGETS PRScal RUNTIME DESTINATION bin)
