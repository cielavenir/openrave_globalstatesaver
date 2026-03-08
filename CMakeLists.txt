cmake_minimum_required(VERSION 3.0)
project(OpenRAVEGlobalStateSaver)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(OpenRAVE REQUIRED)

add_definitions(-fvisibility=hidden)

### set up some flags to configure ABI ###
# -DOPENRAVE_LOG4CXX=1 is set from openrave/config.h

# set up -DHAVE_BOOST_FILESYSTEM
find_package(Boost REQUIRED COMPONENTS filesystem)
add_definitions(-DHAVE_BOOST_FILESYSTEM)

# set up -DUSE_CRLIBM
set(OPT_ACCURATEMATH ON)  # for not assume OPT_ACCURATEMATH=OFF is not used
if( OPT_ACCURATEMATH AND NOT MSVC AND NOT ${CMAKE_SYSTEM_PROCESSOR} MATCHES aarch64 )
include(CheckTypeSize)
add_subdirectory(crlibm-1.0beta4)
endif()

### set up flags end ###

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
execute_process(
  COMMAND ${Python3_EXECUTABLE} -c "import distutils.sysconfig as sysconf; import os; \
    print(os.path.relpath(sysconf.get_python_lib(1, prefix='${CMAKE_INSTALL_PREFIX}'), '${CMAKE_INSTALL_PREFIX}'))"
    OUTPUT_VARIABLE PYTHON_INSTALL_DIR OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
find_file(OPENRAVEPY_INT_SO_FILE openravepy_int.so PATHS ${CMAKE_INSTALL_PREFIX}/${PYTHON_INSTALL_DIR}/openravepy/_openravepy_)

set(METADATA "${CMAKE_CURRENT_BINARY_DIR}/METADATA")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/dist-info/METADATA.in" "${METADATA}" IMMEDIATE @ONLY)

add_library(openrave_globalstatesaver SHARED
  openrave_globalstatesaver.cpp libopenrave.cpp plugindatabase.cpp plugindatabase_virtual.cpp
)

if(OpenRAVE_USE_PYBIND11_PYTHON3_BINDINGS)
  find_package(pybind11 2.9.2 REQUIRED)
  target_link_libraries(openrave_globalstatesaver PUBLIC pybind11::module)
else()
  find_package(Boost REQUIRED COMPONENTS python thread)
  target_link_libraries(openrave_globalstatesaver PUBLIC Boost::python Boost::thread)
endif()

target_link_libraries(openrave_globalstatesaver PUBLIC Python3::Python crlibm ${OPENRAVEPY_INT_SO_FILE} ${OpenRAVE_LIBRARIES})
target_include_directories(openrave_globalstatesaver PRIVATE ${OpenRAVE_INCLUDE_DIRS} ${OpenRAVE_PYTHON3_INCLUDE_DIRS})
target_include_directories(openrave_globalstatesaver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/crlibm-1.0beta4/)
set_target_properties(openrave_globalstatesaver PROPERTIES PREFIX "" INSTALL_RPATH "$ORIGIN/openravepy/_openravepy_")
install(TARGETS openrave_globalstatesaver DESTINATION ${PYTHON_INSTALL_DIR})
install(FILES ${METADATA} "${CMAKE_CURRENT_SOURCE_DIR}/dist-info/top_level.txt" DESTINATION ${PYTHON_INSTALL_DIR}/openrave_globalstatesaver.dist-info/)
