set(target unit_tests)


add_executable(${target}
    unit_tests.cpp
    main.cpp
)

target_include_directories(${target} PUBLIC .)

target_link_libraries(${target}
    common
    gtest
    gtest_main
    pthread
    Boost::system
)

add_test(NAME unit_tests COMMAND ${target})
