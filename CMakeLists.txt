cmake_minimum_required(VERSION 3.16)

project(dds_gateway_example LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

find_package(CycloneDDS REQUIRED)

# Otomatisasi Build IDL menggunakan idlc
set(IDL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/idl)
set(IDL_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${IDL_OUT_DIR})

# Generate C++ Code dari IDL
add_custom_command(
    OUTPUT ${IDL_OUT_DIR}/Telemetry.c ${IDL_OUT_DIR}/Telemetry.h
           ${IDL_OUT_DIR}/RcwsCommand.c ${IDL_OUT_DIR}/RcwsCommand.h
    COMMAND idlc -o ${IDL_OUT_DIR} ${IDL_DIR}/Telemetry.idl
    COMMAND idlc -o ${IDL_OUT_DIR} ${IDL_DIR}/RcwsCommand.idl
    DEPENDS ${IDL_DIR}/Telemetry.idl ${IDL_DIR}/RcwsCommand.idl
)

add_executable(dds_gateway_example
  src/main.cpp
  ${IDL_OUT_DIR}/Telemetry.c
  ${IDL_OUT_DIR}/RcwsCommand.c
  idl/Telemetry.idl
  idl/RcwsCommand.idl
  src/comms/telemetrypublisher.h src/comms/telemetrypublisher.cpp
  src/comms/rcwssubscriber.h src/comms/rcwssubscriber.cpp
  src/app/appcontroller.h src/app/appcontroller.cpp
)

target_include_directories(dds_gateway_example PRIVATE src ${IDL_OUT_DIR})
target_link_libraries(dds_gateway_example PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  CycloneDDS::ddsc
)

include(GNUInstallDirs)
install(TARGETS dds_gateway_example
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
