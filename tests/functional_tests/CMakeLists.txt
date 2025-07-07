set(target functional_tests)


add_executable(${target}
    functional_tests.cpp
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
