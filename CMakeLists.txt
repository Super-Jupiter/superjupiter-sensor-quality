cmake_minimum_required(VERSION 3.16)
project(SuperJupiterSensorQuality VERSION 0.2.0 LANGUAGES CXX)

set(_superjupiter_sensor_quality_top_level OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(_superjupiter_sensor_quality_top_level ON)
endif()
option(SUPERJUPITER_SENSOR_QUALITY_BUILD_TESTS "Build SensorQuality tests" ${_superjupiter_sensor_quality_top_level})
option(SUPERJUPITER_SENSOR_QUALITY_BUILD_EXAMPLES "Build SensorQuality host example" ${_superjupiter_sensor_quality_top_level})
option(SUPERJUPITER_SENSOR_QUALITY_INSTALL "Generate SensorQuality install rules" ${_superjupiter_sensor_quality_top_level})

add_library(SuperJupiterSensorQuality INTERFACE)
add_library(SuperJupiter::SensorQuality ALIAS SuperJupiterSensorQuality)
set_target_properties(SuperJupiterSensorQuality PROPERTIES EXPORT_NAME SensorQuality)
target_include_directories(SuperJupiterSensorQuality INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>)
target_compile_features(SuperJupiterSensorQuality INTERFACE cxx_std_11)

if(SUPERJUPITER_SENSOR_QUALITY_BUILD_TESTS)
  enable_testing()
  add_executable(superjupiter-sensor-quality-tests tests/test_sensor_quality.cpp)
  target_link_libraries(superjupiter-sensor-quality-tests PRIVATE SuperJupiter::SensorQuality)
  add_test(NAME superjupiter-sensor-quality-tests COMMAND superjupiter-sensor-quality-tests)
  add_executable(superjupiter-sensor-quality-namespace-test tests/namespace_isolation.cpp)
  target_link_libraries(superjupiter-sensor-quality-namespace-test PRIVATE SuperJupiter::SensorQuality)
  add_test(NAME superjupiter-sensor-quality-namespace-test COMMAND superjupiter-sensor-quality-namespace-test)
endif()
if(SUPERJUPITER_SENSOR_QUALITY_BUILD_EXAMPLES)
  add_executable(superjupiter-sensor-quality-replay tools/replay.cpp)
  target_link_libraries(superjupiter-sensor-quality-replay PRIVATE SuperJupiter::SensorQuality)
endif()
if(SUPERJUPITER_SENSOR_QUALITY_INSTALL)
  include(CMakePackageConfigHelpers)
  set(_superjupiter_sensor_quality_package_dir lib/cmake/SuperJupiterSensorQuality)
  install(TARGETS SuperJupiterSensorQuality EXPORT SuperJupiterSensorQualityTargets)
  install(FILES src/SuperJupiterSensorQuality.h DESTINATION include)
  install(EXPORT SuperJupiterSensorQualityTargets NAMESPACE SuperJupiter::
    DESTINATION ${_superjupiter_sensor_quality_package_dir})
  configure_package_config_file(cmake/SuperJupiterSensorQualityConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterSensorQualityConfig.cmake
    INSTALL_DESTINATION ${_superjupiter_sensor_quality_package_dir})
  write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterSensorQualityConfigVersion.cmake
    VERSION ${PROJECT_VERSION} COMPATIBILITY SameMinorVersion ARCH_INDEPENDENT)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterSensorQualityConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterSensorQualityConfigVersion.cmake
    DESTINATION ${_superjupiter_sensor_quality_package_dir})
  install(FILES LICENSE DESTINATION share/licenses/SuperJupiterSensorQuality)
endif()
