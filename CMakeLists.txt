cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

if (NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "Please set VCPKG_ROOT environment variable first.")
endif()

MESSAGE(STATUS "VCPKG_ROOT=$ENV{VCPKG_ROOT}")

SET(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain file")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(renderer LANGUAGES CXX C)

include(cmake/deps.cmake)
include(cmake/opts.cmake)

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME}
    ${dependencies}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 20
  LINKER_LANGUAGE CXX
)

target_compile_definitions(${PROJECT_NAME} PUBLIC ${defines})

target_compile_options(${PROJECT_NAME} PUBLIC ${compile_opts})

target_link_options(${PROJECT_NAME} PUBLIC ${link_opts})

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${include_dirs}
)
