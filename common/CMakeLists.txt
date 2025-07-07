set(target common)

add_library(${target}
    protocol.cpp
)

target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${target} PUBLIC Boost::boost Boost::system)