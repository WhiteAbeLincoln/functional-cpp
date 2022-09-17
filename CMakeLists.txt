cmake_minimum_required(VERSION 3.12)

project("functional-cpp" VERSION 1.0.0
                         DESCRIPTION "A functional programming library for C++"
                         HOMEPAGE_URL "https://github.com/WhiteAbeLincoln/functional-cpp")

add_library(${PROJECT_NAME} INTERFACE)

set(detail_header_files)

set(header_files)

target_sources(${PROJECT_NAME} INTERFACE "$<BUILD_INTERFACE:${detail_header_files};${header_files}>")
target_include_directories(
    ${PROJECT_NAME}
    INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)

install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)

option(${PROJECT_NAME}_BUILD_TEST_EXAMPLE "build test and example" ON)
if(${${PROJECT_NAME}_BUILD_TEST_EXAMPLE} AND (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
    enable_testing()
    add_subdirectory(example/)
    # add_subdirectory(test/)
endif()
