# OPTIMISATION FLAGS :
# In cmake, we define variables with set(VAR Value [Value]*)
# set(MYVAR "val1") assigns value val1 to MYVAR
# set(MYVAR "val1" "val2") creates a list

set(OPT_FLAGS
    ""
)

# END OPTIMIZATION FLAGS
set(SRC_ENGINE
    "src/camera/camera.cpp"
    "src/ocean/fft/fast_fourier_transform.cpp"
    "src/ocean/height/height_spectrum.cpp"
    "src/ocean/ocean.cpp"
    "src/ocean/philipps/philipps_spectrum.cpp"
    "src/render/gl_rendering.cpp"
    "src/tools/gl_utils.cpp"
    "src/tools/matrix4.cpp"
    "src/tools/option_parsing.cpp"
    "src/tools/program.cpp"
)

# Directories containing headers must be specified in the include_directories()
include_directories(
    "src/"
)

# END SOURCES


# SETUP
cmake_minimum_required(VERSION 3.23)
project(fftocean)

# BUILD OPTIONS
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS} -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3 -fsanitize=address -g")

# LIBRARIES
find_package(Boost REQUIRED COMPONENTS system program_options)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

# FFTOCEAN
add_executable(fftocean main.cpp)
target_sources(fftocean PRIVATE ${SRC_ENGINE})
target_link_libraries(fftocean PRIVATE Boost::system Boost::program_options ${CMAKE_DL_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})
