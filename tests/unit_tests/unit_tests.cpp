#include <gtest/gtest.h>
#include "protocol.hpp"
#include <optional>

TEST(TelemetryProtocolTest, FullCyclePackingUnpacking) {
    TelemetryData original{};
    original.x = 31;
    original.y = 22;
    original.velocity = 150;
    original.mode = 2;
    original.state = 1;
    original.acceleration = 182;
    original.power = 90;
    original.reserved = 0xABCD;

    uint64_t packed = pack_data(original);
    auto unpacked = unpack_data(packed);

    ASSERT_TRUE(unpacked.has_value());
    EXPECT_EQ(original.x, unpacked->x);
    EXPECT_EQ(original.y, unpacked->y);
    EXPECT_EQ(original.velocity, unpacked->velocity);
    EXPECT_EQ(original.mode, unpacked->mode);
    EXPECT_EQ(original.state, unpacked->state);
    EXPECT_EQ(original.acceleration, unpacked->acceleration);
    EXPECT_EQ(original.power, unpacked->power);
    EXPECT_EQ(original.reserved, unpacked->reserved);
}

TEST(TelemetryProtocolTest, EdgeValues) {
    TelemetryData edge{};
    edge.x = 0;
    edge.y = 0;
    edge.velocity = 0;
    edge.mode = 0;
    edge.state = 0;
    edge.acceleration = 0;
    edge.power = 0;
    edge.reserved = 0xFFFF; // Максимальное 16-битное значение

    uint64_t packed = pack_data(edge);
    auto unpacked = unpack_data(packed);

    ASSERT_TRUE(unpacked.has_value());
    EXPECT_EQ(edge.x, unpacked->x);
    EXPECT_EQ(edge.y, unpacked->y);
    EXPECT_EQ(edge.velocity, unpacked->velocity);
    EXPECT_EQ(edge.mode, unpacked->mode);
    EXPECT_EQ(edge.state, unpacked->state);
    EXPECT_EQ(edge.acceleration, unpacked->acceleration);
    EXPECT_EQ(edge.power, unpacked->power);
    EXPECT_EQ(edge.reserved, unpacked->reserved);
}

TEST(TelemetryProtocolTest, OverflowProtection) {
    TelemetryData overflow{};
    overflow.x = 100;
    overflow.y = 100;
    overflow.velocity = 300;
    overflow.mode = 5;
    overflow.state = 4;
    overflow.acceleration = 300;
    overflow.power = 200;
    overflow.reserved = 0x12345;

    uint64_t packed = pack_data(overflow);
    auto unpacked = unpack_data(packed);

    ASSERT_TRUE(unpacked.has_value());
    EXPECT_EQ(36, unpacked->x);
    EXPECT_EQ(36, unpacked->y);
    EXPECT_EQ(44, unpacked->velocity);
    EXPECT_EQ(1, unpacked->mode);
    EXPECT_EQ(0, unpacked->state);
    EXPECT_EQ(44, unpacked->acceleration);
    EXPECT_EQ(200, unpacked->power);
    EXPECT_EQ(0x2345, unpacked->reserved);
}

TEST(TelemetryProtocolTest, ReservedFieldValidation) {
    TelemetryData data{};
    data.zero1 = 1; // Нарушение: должен быть 0
    
    uint64_t packed = pack_data(data);
    auto unpacked = unpack_data(packed);
    
    EXPECT_FALSE(unpacked.has_value());
}

TEST(TelemetryProtocolTest, NegativeY) {
    TelemetryData data{};
    data.y = 0;  // -32
    
    uint64_t packed = pack_data(data);
    auto unpacked = unpack_data(packed);
    
    ASSERT_TRUE(unpacked.has_value());
    EXPECT_EQ(0, unpacked->y);
}