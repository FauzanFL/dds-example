cmake_minimum_required(VERSION 3.16)

project(dds_gateway_example LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

list(APPEND CMAKE_PREFIX_PATH "/home/fauzanfl/.local")

find_package(CycloneDDS REQUIRED)
find_package(iceoryx_posh REQUIRED)
find_package(iceoryx_hoofs REQUIRED)

# Otomatisasi Build IDL menggunakan idlc
set(IDL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/idl)
set(IDL_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${IDL_OUT_DIR})

# Generate C++ Code dari IDL
add_custom_command(
    OUTPUT ${IDL_OUT_DIR}/Telemetry.cpp ${IDL_OUT_DIR}/Telemetry.hpp
           ${IDL_OUT_DIR}/RcwsCommand.cpp ${IDL_OUT_DIR}/RcwsCommand.hpp
    COMMAND idlc -l cxx -d ${IDL_OUT_DIR} ${IDL_DIR}/Telemetry.idl
    COMMAND idlc -l cxx -d ${IDL_OUT_DIR} ${IDL_DIR}/RcwsCommand.idl
    DEPENDS ${IDL_DIR}/Telemetry.idl ${IDL_DIR}/RcwsCommand.idl
)

add_executable(dds_gateway_example
  src/main.cpp
  ${IDL_OUT_DIR}/Telemetry.cpp
  ${IDL_OUT_DIR}/RcwsCommand.cpp
  idl/Telemetry.idl
  idl/RcwsCommand.idl
)

target_include_directories(dds_gateway_example PRIVATE src ${IDL_OUT_DIR})
target_link_libraries(dds_gateway_example PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  CycloneDDS::ddsc
  iceoryx_posh::iceoryx_posh
  iceoryx_hoofs::iceoryx_hoofs
)

include(GNUInstallDirs)
install(TARGETS dds_gateway_example
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
