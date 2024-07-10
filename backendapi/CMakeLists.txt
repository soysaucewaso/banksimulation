cmake_minimum_required(VERSION 3.10)

# Project name
project(backendapi)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set CMake prefix path for Pistache and libpqxx
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/pistache/include")

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEVENT REQUIRED libevent)
pkg_check_modules(PQXX REQUIRED libpqxx)

# Include directories
include_directories(${PQXX_INCLUDE_DIRS})
include_directories(${LIBEVENT_INCLUDE_DIRS})
include_directories(${CMAKE_PREFIX_PATH})

# Link directories
link_directories(${PQXX_LIBRARY_DIRS})
link_directories(${LIBEVENT_LIBRARY_DIRS})
link_directories(${PISTACHE_LIBRARY_DIRS})

# Set PostgreSQL paths
set(PostgreSQL_LIBRARY /opt/homebrew/lib)
set(PostgreSQL_INCLUDE_DIR /opt/homebrew/include)

# Find Pistache library
find_library(Pistache_LIBRARY pistache /opt/homebrew/lib)

# Add executable targets
add_executable(backendapi bankapiserver.cpp bankrequests.cpp ../sharedabstractclasses/EndpointTemplate.cpp)

# Link libraries to the executable targets
target_link_libraries(backendapi ${Pistache_LIBRARY} ${LIBEVENT_LIBRARIES} ${PQXX_LIBRARIES})
