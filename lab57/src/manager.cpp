#include "manager.h"
#include "utils.h"
#include <unistd.h>
#include <csignal>
#include <cstdlib> 
#include <atomic>
#include <sstream>

#ifndef COMPUTE_NODE_PATH
#define COMPUTE_NODE_PATH "./compute_node"
#endif

static std::atomic<int> port_counter{20000};

Manager::Manager() : managerNode(nullptr), context(1)
{
    pid_t manager_pid = getpid();
    managerNode = new Node(-1, -1, manager_pid);
    managerNode->available = true;

    node_map[-1] = managerNode;
    ids.insert(-1);
}

Manager::~Manager()
{
    for (Node* c : managerChildren) {
        clearAll(c);
    }
    managerChildren.clear();

    node_map.erase(-1);
    delete managerNode;
    managerNode = nullptr;

    node_map.clear();
}

void Manager::clearAll(Node* node)
{
    if (!node) return;
    clearAll(node->next);
    node_map.erase(node->id);
    delete node;
}

bool Manager::Exist(int id)
{
    return (ids.find(id) != ids.end());
}

std::string Manager::createNode(int child_id, int parent_id)
{
    if (Exist(child_id)) {
        return "Error: Already exists";
    }

    Node* parent = findNode(parent_id);
    if (!parent) {
        return "Error: Parent not found";
    }
    if (!parent->available) {
        return "Error: Parent is unavailable";
    }

    if (parent->id == -1) {
    } else {
        if (parent->next != nullptr) {
            return "Error: Parent already has child";
        }
    }

    int port = port_counter.fetch_add(1);

    pid_t fork_id = fork();
    if (fork_id < 0) {
        return "Error: fork() failed";
    }
    if (fork_id == 0) {
        if (execl(COMPUTE_NODE_PATH,
                  "compute_node",
                  std::to_string(child_id).c_str(),
                  std::to_string(port).c_str(),
                  std::to_string(parent_id).c_str(),
                  (char*)NULL) == -1)
        {
            _exit(1);
        }
    }
    else {
        Node* newNode = new Node(child_id, port, fork_id);
        newNode->available = true;

        ids.insert(child_id);
        node_map[child_id] = newNode;

        if (parent->id == -1) {
            managerChildren.push_back(newNode);
        } else {
            parent->next = newNode;
        }
        return "Ok: " + std::to_string(fork_id);
    }
    return "Error: unreachable";
}

std::string Manager::execCommand(int node_id, const std::vector<int>& numbers)
{
    Node* node = findNode(node_id);
    if (!node) {
        return "Error:" + std::to_string(node_id) + ": Not found";
    }
    if (!node->available) {
        return "Error:" + std::to_string(node_id) + ": Node is unavailable";
    }

    long long sum = 0;
    for (auto x : numbers) sum += x;

    return "Ok:" + std::to_string(node_id) + ": " + std::to_string(sum);
}

std::string Manager::pingNode(int node_id)
{
    Node* node = findNode(node_id);
    if (!node) {
        return "Ok: 0";
    }
    return node->available ? "Ok: 1" : "Ok: 0";
}

std::string Manager::killNode(int node_id)
{
    if (node_id == -1) {
        for (Node* c : managerChildren) {
            removeChain(c);
        }
        managerChildren.clear();
        return "Ok";
    }

    for (auto it = managerChildren.begin(); it != managerChildren.end(); ++it) {
        if ((*it)->id == node_id) {
            Node* child = *it;
            managerChildren.erase(it);
            removeChain(child);
            return "Ok";
        }
    }

    Node* parent = findParent(node_id);
    if (!parent) {
        return "Error: Not found";
    }
    if (!parent->next) {
        return "Error: Not found";
    }
    if (parent->next->id == node_id) {
        Node* doomed = parent->next;
        parent->next = nullptr;
        removeChain(doomed);
        return "Ok";
    }
    return "Error: Not found";
}

void Manager::removeChain(Node* node)
{
    if (!node) return;
    removeChain(node->next);
    node->next = nullptr;

    if (node->available && node->pid > 0) {
        ::kill(node->pid, SIGKILL);
    }
    node->available = false;
}

Manager::Node* Manager::findNode(int id)
{
    auto it = node_map.find(id);
    if (it == node_map.end()) return nullptr;
    return it->second;
}

Manager::Node* Manager::findParent(int node_id)
{

    for (auto& kv : node_map) {
        int pid = kv.first;
        if (pid == -1) continue;
        Node* p = kv.second;
        if (p && p->next && p->next->id == node_id) {
            return p;
        }
    }
    return nullptr;
}
