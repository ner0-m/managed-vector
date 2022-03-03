CPMAddPackage("gh:onqtam/doctest#2.4.5")

set(CMAKE_MODULE_PATH "${doctest_SOURCE_DIR}/scripts/cmake" ${CMAKE_MODULE_PATH})

include(CTest)
include(doctest)

function(add_unit_test name)
    set(target_name test_${name})
    add_executable(${target_name} EXCLUDE_FROM_ALL ${target_name}.cpp test_main.cpp)
    target_link_libraries(${target_name} PUBLIC mem::mem doctest)
    set_target_properties(${target_name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests/"
        )
    doctest_discover_tests(${target_name})
endfunction()

add_unit_test(simple)
add_unit_test(normal_iterator)
add_unit_test(contiguous_storage)
add_unit_test(base_vector)
add_unit_test(overlapped_copy)
