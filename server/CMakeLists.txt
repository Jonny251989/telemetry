set(target telemetry_server)

add_executable(${target}
    main.cpp
    udp_server.cpp
    data_validator.cpp
)

target_link_libraries(${target}
    common
    Boost::system
)

target_include_directories(${target} PRIVATE
    ${nlohmann_json_SOURCE_DIR}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/limits.json 
               ${CMAKE_CURRENT_BINARY_DIR}/limits.json COPYONLY)