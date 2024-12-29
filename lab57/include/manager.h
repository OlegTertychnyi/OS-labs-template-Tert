#ifndef MANAGER_H
#define MANAGER_H

#include <map>
#include <set>
#include <unordered_map>
#include <string>
#include <vector>
#include <zmq.hpp>
#include <sys/types.h>

class Manager {
public:
    struct Node {
        int id;
        bool available;
        int port;
        pid_t pid;
        Node* next; 

        Node(int _id, int _port, pid_t _pid)
          : id(_id), available(true),
            port(_port), pid(_pid), next(nullptr)
        {}
    };

private:
    Node* managerNode;

    std::vector<Node*> managerChildren;

    std::set<int> ids;

    std::unordered_map<int, Node*> node_map;

    zmq::context_t context;

public:
    Manager();
    ~Manager();

    bool Exist(int id);

    std::string createNode(int child_id, int parent_id);
    std::string execCommand(int node_id, const std::vector<int>& numbers);
    std::string pingNode(int node_id);
    std::string killNode(int node_id);

private:
    void removeChain(Node* node);

    void clearAll(Node* node);

    Node* findNode(int id);

    Node* findParent(int node_id);

    Node* findParentAmongChildren(const std::vector<Node*>& children, int child_id);
};

#endif // MANAGER_H
