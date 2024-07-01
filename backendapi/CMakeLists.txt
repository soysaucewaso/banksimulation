project(backendapi)
#set(CMAKE_PREFIX_PATH )
set(CMAKE_PREFIX_PATH pistache/include)
set(CMAKE_CXX_STANDARD 17) 
#required pkg
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEVENT REQUIRED libevent)
#####################################
##      HEADERS SEARCH PATHS       ##
#####################################
#set(PROJECT_INCLUDE_DIR "src/include")
find_library(Pistache_LIBRARY pistache /opt/homebrew/lib)
include_directories("/Users/sawyer/banksimulation/backendapi/pistache/include")
include_directories(${LIBEVENT_INCLUDE_DIRS})
set(PISTACHE_INCLUDE_DIR "pistache/include/pistache/include")
set(HEADER_SEARCH_PATHS ${PISTACHE_INCLUDE_DIR})
add_executable(backendapi playing.cpp)
add_executable(testbackend pistache/examples/http_server.cc)
target_link_libraries(backendapi ${Pistache_LIBRARY} ${LIBEVENT_LIBRARIES}  )
target_link_libraries(testbackend ${Pistache_LIBRARY} ${LIBEVENT_LIBRARIES}  )
