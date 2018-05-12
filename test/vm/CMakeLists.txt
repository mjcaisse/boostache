macro(add_boost_test name source)
   add_executable( ${ARGV} )

   # link setup
   target_link_libraries(${name} ${Boost_LIBRARIES})

   # unit test configuration
   add_test(NAME ${name} COMMAND ${name})
   set_tests_properties( ${name} PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.. )
endmacro(add_boost_test)

add_boost_test( foreach foreach.cpp )
