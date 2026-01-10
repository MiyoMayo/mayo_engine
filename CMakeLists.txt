cmake_minimum_required(VERSION 4.0)

project(
  mayo
  VERSION 0.1.0
  LANGUAGES CXX)

# 言語設定
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# テスト設定
include(CTest)
enable_testing()

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

# テスト
if(BUILD_TESTING)
  add_executable(tests tests/main.cpp)
  target_link_libraries(tests PRIVATE ${PROJECT_NAME})
  add_test(NAME unit COMMAND tests)
endif()

# install
include(GNUInstallDirs)
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY src/public/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
