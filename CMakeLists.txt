cmake_minimum_required(VERSION 3.16)

project(dds_gateway_example LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test)
find_package(CycloneDDS-CXX REQUIRED)

idlcxx_generate(
    TARGET generated
    FILES
      idl/Telemetry.idl
      idl/RCWSCommand.idl
      idl/RCWSStatus.idl
      idl/MissionRoute.idl
      idl/TargetTrack.idl
)

add_executable(dds_gateway_example
  src/main.cpp
  src/comms/telemetrypublisher.h src/comms/telemetrypublisher.cpp
  src/comms/rcwssubscriber.h src/comms/rcwssubscriber.cpp

  src/config/qos_profiles.h
  src/lib/jsonconverter.h src/lib/jsonconverter.cpp
)

target_include_directories(dds_gateway_example PRIVATE src ${IDL_OUT_DIR})
target_link_libraries(dds_gateway_example PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  CycloneDDS-CXX::ddscxx
  generated
)

enable_testing()

function(add_qt_unit_test TEST_NAME)
  add_executable(${TEST_NAME} ${ARGN})

  target_include_directories(${TEST_NAME} PRIVATE src ${IDL_OUT_DIR})

  target_link_libraries(${TEST_NAME} PRIVATE
          Qt${QT_VERSION_MAJOR}::Core
          Qt${QT_VERSION_MAJOR}::Test
          CycloneDDS-CXX::ddscxx
          generated
      )

  target_compile_options(${TEST_NAME} PRIVATE --coverage -O0)
  target_link_options(${TEST_NAME} PRIVATE --coverage)

  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

endfunction()

add_qt_unit_test(test_serialization
    tests/TestSerialization.cpp
    src/lib/jsonconverter.h src/lib/jsonconverter.cpp
)

include(GNUInstallDirs)
install(TARGETS dds_gateway_example
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
