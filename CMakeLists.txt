# OPTIMISATION FLAGS :
# In cmake, we define variables with set(VAR Value [Value]*)
# set(MYVAR "val1") assigns value val1 to MYVAR
# set(MYVAR "val1" "val2") creates a list

set(OPT_FLAGS
    ""
)

# END OPTIMIZATION FLAGS
set(SRC_ENGINE
    "src/option_parsing.cpp"
)

# Directories containing headers must be specified in the include_directories()
include_directories(
    "src/"
)

# END SOURCES


# SETUP
cmake_minimum_required(VERSION 3.12)
project(FFTOcean)

# BUILD OPTIONS
set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3 -fsanitize=address")

# LIBRARIES
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost REQUIRED COMPONENTS system program_options)
set(LIBRARIES Boost::system Boost::program_options ${CMAKE_DL_LIBS})

# CHESSENGINE
add_executable(fftocean)
target_sources(fftocean PRIVATE "main.cpp" ${SRC_ENGINE})
target_link_libraries(fftocean PRIVATE ${LIBRARIES})

# STATIC TARGET
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(fftocean-static)
    target_sources(fftocean-static PRIVATE "main.cpp" ${SRC_ENGINE})
    target_link_libraries(fftocean-static PRIVATE ${LIBRARIES} -static)
endif()
