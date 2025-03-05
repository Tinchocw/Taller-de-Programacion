#include <iostream>
#include <iostream>

class Socket {
public:
    Socket() {}

    Socket(const Socket& other) = delete;
    Socket& operator=(const Socket& other) = delete;

    Socket(Socket&& other) { std::cout << "Move constructor\n"; }
    Socket& operator=(Socket&& other)  { std::cout << "Move assign\n"; return *this; }
};

class ProtocolA {
    Socket skt;
public:
    ProtocolA(Socket skt) : skt(std::move(skt)) {}
};

class ProtocolB {
    Socket skt;
public:
    ProtocolB(Socket&& skt) : skt(std::move(skt)) {}
};

int main() {
    Socket skA;
    ProtocolA protoA(std::move(skA));
    std::cout << "--------\n";
    Socket skB;
    ProtocolB protoB(std::move(skB));
    return 0;
}
