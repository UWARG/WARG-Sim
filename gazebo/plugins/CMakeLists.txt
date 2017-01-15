cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

#Adds a custom FIND<MODULE>.CMAKE folder to the project
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Qt4 4.4.3 REQUIRED QtGui)
include (${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

message(status " QT Library: " ${QT_LIBRARIES})

find_package(gazebo REQUIRED)
find_package(LibSerial REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS} "/usr/include/qt4"
${LIBSERIAL_INCLUDE_DIR})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

set(plugins
    CessnaGUIPlugin
    CessnaPlugin
    LiftDragPlugin
    hello_model
    )

foreach (src ${plugins})
    add_library(${src} SHARED ${src}.cc)
    target_link_libraries(${src} ${GAZEBO_LIBRARIES} Qt4::QtGui )
endforeach()

# Compile WARG Simulator
add_executable(WargSimulator WargSimulator.cc ArduinoComms.cc)
target_link_libraries(WargSimulator ${GAZEBO_LIBRARIES} pthread ${LIBSERIAL_LIBRARY})

