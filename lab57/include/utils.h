#ifndef UTILS_H
#define UTILS_H

#include <zmq.hpp>
#include <string>
#include <stdexcept>

inline void send_message(zmq::socket_t &socket, const std::string &msg) {
    zmq::message_t message(msg.size());
    memcpy(message.data(), msg.c_str(), msg.size());
    socket.send(message, zmq::send_flags::none);
}

inline std::string receive_message(zmq::socket_t &socket) {
    zmq::message_t message;
    if (!socket.recv(message, zmq::recv_flags::none)) {
        throw std::runtime_error("Failed to receive");
    }
    return std::string(static_cast<char*>(message.data()), message.size());
}

#endif // UTILS_H
