cmake_minimum_required(VERSION 4.0)

project(
  mayo
  VERSION 0.1.0
  LANGUAGES CXX)

# 言語設定
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ライブラリ本体
add_library(${PROJECT_NAME} STATIC src/private/mayo.cpp)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/public>
         $<INSTALL_INTERFACE:include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/private)

# Apple Silicon対応
if(APPLE)
  set_property(TARGET ${PROJECT_NAME} PROPERTY OSX_ARCHITECTURES "arm64")
endif()

# テスト設定
include(CTest)
enable_testing()

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

# プロジェクトをインストール
include(GNUInstallDirs)
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY src/public/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(_pkg_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION "${_pkg_dir}")

# Config/Versionをインストール
include(CMakePackageConfigHelpers)

configure_package_config_file(
  cmake/PackageConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION "${_pkg_dir}")

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMinorVersion)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION "${_pkg_dir}")
