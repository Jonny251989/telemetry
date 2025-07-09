#pragma once

#include <cstdint>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <optional>
#include <memory>

#pragma pack(push, 1)

struct TelemetryData {
    uint64_t reserved : 16;  
    uint64_t acceleration : 8; 
    uint64_t power : 8;
    uint64_t velocity : 8;
    uint64_t mode : 2;
    uint64_t zero1 : 2;
    uint64_t state : 2;
    uint64_t zero2 : 2;
    uint64_t x : 6;
    uint64_t zero3 : 2;
    uint64_t y : 6;
    uint64_t zero4 : 2;
};

#pragma pack(pop)

uint64_t pack_data(const TelemetryData& data);
std::optional<TelemetryData> unpack_data(uint64_t packet);