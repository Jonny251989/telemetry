#pragma once

#include <boost/asio.hpp>
#include <boost/endian/conversion.hpp>

template <typename T>
T host_to_network(T&& value) {
    return boost::endian::native_to_big(std::forward<T>(value));
}

template <typename T>
T network_to_host(T&& value) {
    return boost::endian::big_to_native(std::forward<T>(value));
}