function(_fcpp_example name)
    add_executable(fcpp_example_${name} ${name}.cpp)
    target_link_libraries(fcpp_example_${name} PUBLIC functional-cpp)
    set_property(TARGET fcpp_example_${name} PROPERTY CXX_STANDARD 20)
endfunction()

_fcpp_example(basic)
# _fcpp_example(json)
