cmake_minimum_required(VERSION 3.16)

project(OBSBOT_Linux_GUI)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED true)

# Find GTK3
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

set(OBSBOT_Target "OBSBOT_Linux_GUI")

# Set SDK path
set(SDK_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/libdev_v2.1.0_7/include)

if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    set(SDK_LINK_PATH ${PROJECT_SOURCE_DIR}/libdev_v2.1.0_7/linux\(beta\)/x86_64-release)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    set(SDK_LINK_PATH ${PROJECT_SOURCE_DIR}/libdev_v2.1.0_7/linux\(beta\)/arm64-release)
else()
    message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

add_executable(${OBSBOT_Target} main.cpp)

# Include directories
include_directories(${SDK_INCLUDE_PATH})
include_directories(${GTK3_INCLUDE_DIRS})

# Link directories
target_link_directories(${OBSBOT_Target} PRIVATE "${SDK_LINK_PATH}")
link_directories(${GTK3_LIBRARY_DIRS})

# Add GTK3 definitions
add_definitions(${GTK3_CFLAGS_OTHER})

# Link libraries
target_link_libraries(${OBSBOT_Target} PRIVATE
    dev
    ${GTK3_LIBRARIES}
    pthread
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations -Wno-switch")

# Install
set(PROJECT_RUN_DIR ${CMAKE_BINARY_DIR}/rundir)
install(TARGETS ${OBSBOT_Target} DESTINATION ${PROJECT_RUN_DIR})
install(DIRECTORY ${SDK_LINK_PATH}/ DESTINATION ${PROJECT_RUN_DIR})
