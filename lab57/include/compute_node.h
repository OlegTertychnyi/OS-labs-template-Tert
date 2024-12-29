#ifndef COMPUTE_NODE_H
#define COMPUTE_NODE_H

#include <zmq.hpp>
#include <string>
#include "utils.h"

class ComputeNode {
private:
    int node_id;
    int parent_id;
    int port;
    zmq::context_t context;
    zmq::socket_t socket_rep;

public:
    ComputeNode(int _node_id, int _port, int _parent_id);
    void run();
};

#endif // COMPUTE_NODE_H
