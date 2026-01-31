function(add_unit_test source_file)
  get_filename_component(stem "${source_file}" NAME_WE)
  set(target "test_${stem}")

  add_executable(${target} "${source_file}")
  target_compile_options(${target} PRIVATE -UNDEBUG)
  target_link_libraries(${target} PRIVATE ${PROJECT_NAME})
add_test(NAME "unit_${stem}" COMMAND ${target})
endfunction()

add_unit_test(types/concepts.cpp)
