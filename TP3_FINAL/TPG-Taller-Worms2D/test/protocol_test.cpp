#include <limits>
#include <random>

#include <gtest/gtest.h>

#include "../client/cprotocol.h"
#include "../common/States.h"
#include "../server/sprotocol.h"


class ProtocolTest: public ::testing::Test {
protected:
    Socket skt;
    ClientSide::Protocol client;
    ServerSide::Protocol server;
    uint8_t expected_uint8;
    float expected_float;
    bool expected_bool;
    std::string expected_str;


    inline ProtocolTest():
            skt("8080"),
            client("127.0.0.1", "8080"),
            server(skt.accept()),
            expected_uint8(0),
            expected_float(0.0f),
            expected_bool(false),
            expected_str("") {}

    void SetUp() override {
        auto rng = std::random_device();
        std::mt19937 gen(rng());

        std::uniform_int_distribution<uint8_t> random_uint8(0, 255);
        expected_uint8 = random_uint8(gen);

        std::uniform_real_distribution<float> random_float(std::numeric_limits<float>::min(),
                                                           std::numeric_limits<float>::max());
        expected_float = random_float(gen);

        std::uniform_int_distribution<uint8_t> random_bool(0, 1);
        expected_bool = random_bool(gen);

        std::uniform_int_distribution<char> random_char(97, 122);  // a - z
        expected_str = std::string(
                {random_char(gen), random_char(gen), random_char(gen), random_char(gen)});
    }
};

TEST_F(ProtocolTest, ServerRecvString64Char) {
    client.sendString64(expected_str);

    std::string received;
    server.recvString64(received);

    ASSERT_STREQ(expected_str.data(), received.data());
}


TEST_F(ProtocolTest, ServerRecvFloat) {
    client.sendFloat(expected_float);
    float received = server.recvFloat();

    ASSERT_FLOAT_EQ(expected_float, received);
}

TEST_F(ProtocolTest, ServerRecvUint8) {
    client.send(&expected_uint8, sizeof(uint8_t));
    uint8_t received = server.recvUint8();

    ASSERT_EQ(expected_uint8, received);
}

TEST_F(ProtocolTest, ClientRecvUint8) {
    server.send(&expected_uint8, sizeof(uint8_t));
    uint8_t received = client.recvUint8();

    ASSERT_EQ(expected_uint8, received);
}


TEST_F(ProtocolTest, ClientRecvFloat) {
    server.sendFloat(expected_float);
    float received = client.recvFloat();

    ASSERT_FLOAT_EQ(expected_float, received);
}

TEST_F(ProtocolTest, ClientRecvBool) {
    server.send(&expected_bool, sizeof(uint8_t));
    bool received = client.recvBool();

    ASSERT_EQ(expected_bool, received);
}

TEST_F(ProtocolTest, ClientRecvString64Char) {
    server.sendString64(expected_str);

    std::string received;
    client.recvString64(received);

    ASSERT_STREQ(expected_str.data(), received.data());
}
