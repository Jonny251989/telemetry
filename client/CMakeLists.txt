set(target telemetry_client)

add_executable(${target}
    main.cpp
    udp_client.cpp
)

target_link_libraries(${target}
    common
    Boost::boost Boost::system
)