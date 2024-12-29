#include <iostream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <zmq.hpp>
#include "utils.h"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        _exit(1);
    }
    int node_id = std::stoi(argv[1]);
    int port    = std::stoi(argv[2]);
    int parent_id = std::stoi(argv[3]);

    zmq::context_t context(1);
    zmq::socket_t socket_rep(context, zmq::socket_type::rep);

    std::string address = "tcp://127.0.0.1:" + std::to_string(port);
    socket_rep.bind(address);

    while (true) {
        std::string msg;
        try {
            msg = receive_message(socket_rep);
        } catch (...) {
            continue;
        }
        std::istringstream iss(msg);
        std::string cmd;
        iss >> cmd;
        if (cmd == "pid") {
            send_message(socket_rep, std::to_string(getpid()));
        }
        else if (cmd == "ping") {
            send_message(socket_rep, "Ok: 1");
        }
        else if (cmd == "exec") {
            int n;
            iss >> n;
            long long sum = 0;
            for (int i = 0; i < n; i++) {
                long long val;
                iss >> val;
                sum += val;
            }
            std::string ans = "Ok:" + std::to_string(node_id) + ": " + std::to_string(sum);
            send_message(socket_rep, ans);
        }
        else if (cmd == "kill") {
            send_message(socket_rep, std::to_string(parent_id));
            break;
        }
        else if (cmd == "create") {
            send_message(socket_rep, "Error: not implemented in child");
        }
        else {
            send_message(socket_rep, "Error: unknown command");
        }
    }
    socket_rep.close();
    return 0;
}
