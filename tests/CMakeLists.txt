option(MAYO_TIDY_TESTS "Run clang-tidy for test targets" OFF)

function(add_unit_test source_file)
  get_filename_component(dir "${source_file}" DIRECTORY)
  get_filename_component(name "${source_file}" NAME_WE)
  if(dir)
    set(stem "${dir}/${name}")
  else()
    set(stem "${name}")
  endif()
  string(REPLACE "/" "_" stem "${stem}")
  set(target "test_${stem}")

  add_executable(${target} "${source_file}")
  target_compile_options(${target} PRIVATE -UNDEBUG)
  target_link_libraries(${target} PRIVATE ${PROJECT_NAME})
  set_target_properties(
    ${target}
    PROPERTIES CXX_STANDARD 26
               CXX_STANDARD_REQUIRED YES
               CXX_EXTENSIONS NO)

  if(CMAKE_CXX_CLANG_TIDY AND NOT MAYO_TIDY_TESTS)
    set_property(TARGET ${target} PROPERTY CXX_CLANG_TIDY "")
  endif()

  if(APPLE)
    set_property(TARGET ${target} PROPERTY OSX_ARCHITECTURES "arm64")
  endif()

  add_test(NAME "unit_${stem}" COMMAND ${target})
endfunction()

add_unit_test(types/concepts.cpp)
add_unit_test(types/core/option.cpp)
add_unit_test(types/core/ref.cpp)
