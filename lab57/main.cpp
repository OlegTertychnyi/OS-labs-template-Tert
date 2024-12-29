#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "manager.h"

int main() {
    Manager manager;
    std::cout << "Commands:\n"
              << "  create <id> [parent]\n"
              << "  exec <id> <n> <k1..kn>\n"
              << "  ping <id>\n"
              << "  kill <id>\n"
              << "  exit\n";

    while (true) {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line)) {
            break;
        }
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "create") {
            int child_id, parent_id = -1;
            iss >> child_id;
            if (!(iss >> parent_id)) {
            }
            std::string ans = manager.createNode(child_id, parent_id);
            std::cout << ans << std::endl;

        } else if (cmd == "exec") {
            int node_id, n;
            iss >> node_id >> n;
            std::vector<int> nums(n);
            for (int i = 0; i < n; i++) {
                iss >> nums[i];
            }
            std::string ans = manager.execCommand(node_id, nums);
            std::cout << ans << std::endl;

        } else if (cmd == "ping") {
            int node_id;
            iss >> node_id;
            std::string ans = manager.pingNode(node_id);
            std::cout << ans << std::endl;

        } else if (cmd == "kill") {
            int node_id;
            iss >> node_id;
            std::string ans = manager.killNode(node_id);
            std::cout << ans << std::endl;

        } else if (cmd == "exit") {
            break;
        } else {
            std::cout << "Unknown command\n";
        }
    }

    return 0;
}
