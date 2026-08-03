cmake_minimum_required(VERSION 3.8)
project(studica_drivers)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)

if(BUILD_TESTING)
  option(STUDICA_STRICT_LINT "Enable full strict lint checks" OFF)
  find_package(ament_lint_auto REQUIRED)
  find_package(ament_lint_common REQUIRED)
  if(NOT STUDICA_STRICT_LINT)
    # Legacy codebase: enable lint infrastructure first, then tighten checks incrementally.
    set(AMENT_LINT_AUTO_EXCLUDE
      ament_cmake_copyright
      ament_cmake_cpplint
      ament_cmake_uncrustify
    )
  endif()
  ament_lint_auto_find_test_dependencies()
endif()

find_path(VMXPI_INCLUDE_DIR
  NAMES VMXPi.h
  HINTS /usr/local/include/vmxpi
)
find_library(VMXPI_HAL_CPP
  NAMES vmxpi_hal_cpp
  HINTS /usr/local/lib/vmxpi /usr/local/lib
)

set(DRIVER_SOURCES
  analog_input.cpp
  cobra.cpp
  dio.cpp
  duty_cycle_encoder.cpp
  encoder.cpp
  i2c.cpp
  imu.cpp
  light_tower.cpp
  parsec.cpp
  parsec_usb.cpp
  pwm.cpp
  servo.cpp
  sharp.cpp
  titan.cpp
  ultrasonic.cpp
)

set(DRIVER_HPP_HEADERS
  analog_input.hpp
  cobra.hpp
  dio.hpp
  duty_cycle_encoder.hpp
  encoder.hpp
  i2c.hpp
  imu.hpp
  light_tower.hpp
  parsec.hpp
  parsec_usb.hpp
  pwm.hpp
  servo.hpp
  sharp.hpp
  titan.hpp
  ultrasonic.hpp
)

set(DRIVER_COMPAT_HEADERS
  analog_input.h
  cobra.h
  dio.h
  duty_cycle_encoder.h
  encoder.h
  i2c.h
  imu.h
  light_tower.h
  parsec.h
  parsec_usb.h
  pwm.h
  servo.h
  sharp.h
  titan.h
  ultrasonic.h
)

set(DRIVER_PUBLIC_HEADERS
  ${DRIVER_HPP_HEADERS}
  ${DRIVER_COMPAT_HEADERS}
)

if(NOT VMXPI_INCLUDE_DIR OR NOT VMXPI_HAL_CPP)
  message(WARNING "VMXPi headers or library not found. Building stub studica_drivers (no hardware support).")
  add_library(studica_drivers INTERFACE)
  add_library(studica_drivers::studica_drivers ALIAS studica_drivers)

  install(TARGETS studica_drivers
    EXPORT export_studica_drivers
  )
  install(FILES ${DRIVER_PUBLIC_HEADERS}
    DESTINATION include/studica_drivers
  )

  ament_export_targets(export_studica_drivers HAS_LIBRARY_TARGET)
  ament_export_include_directories(include/studica_drivers)
  ament_package()
  return()
endif()

add_library(studica_drivers SHARED ${DRIVER_SOURCES})
target_compile_features(studica_drivers PUBLIC cxx_std_17)
target_include_directories(studica_drivers PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/studica_drivers>
  ${VMXPI_INCLUDE_DIR}
)
target_link_libraries(studica_drivers PUBLIC ${VMXPI_HAL_CPP} rt pthread)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 7)
  target_link_libraries(studica_drivers PUBLIC atomic)
endif()

install(TARGETS studica_drivers
  EXPORT export_studica_drivers
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(FILES ${DRIVER_PUBLIC_HEADERS}
  DESTINATION include/studica_drivers
)

ament_export_targets(export_studica_drivers HAS_LIBRARY_TARGET)
ament_export_include_directories(include/studica_drivers)
ament_package()
