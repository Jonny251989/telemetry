#include "protocol.hpp"

uint64_t pack_data(const TelemetryData& data) {
    uint64_t packet = 0;
    memcpy(&packet, &data, sizeof(packet));
    
    return packet;
}

std::optional<TelemetryData> unpack_data(uint64_t packet) {

    if (sizeof(packet) != 8) {
        std::cout << "Invalid packet size: expected 8 bytes, got "
                  << sizeof(packet) << " bytes\n";
        return std::nullopt;
    }

    TelemetryData data;
    memcpy(&data, &packet, sizeof(data));

    if (data.zero1 != 0 ||
        data.zero2 != 0 || 
        data.zero3 != 0 || 
        data.zero4 != 0) {

        std::cout << "Invalid packet: reserved zero bits are non-zero\n";
        return std::nullopt;
    }
    
    return data;
}