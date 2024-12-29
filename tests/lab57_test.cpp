#include <gtest/gtest.h>
#include "../lab57/include/manager.h"

TEST(Lab57Test, CreateSingleNode) {
    Manager m;
    EXPECT_FALSE(m.Exist(10));

    std::string ans = m.createNode(10, -1);
    EXPECT_TRUE(m.Exist(10));
    EXPECT_NE(ans.find("Ok:"), std::string::npos);
    EXPECT_EQ("Ok: 1", m.pingNode(10));
}

TEST(Lab57Test, CreateAlreadyExists) {
    Manager m;
    m.createNode(10, -1);
    std::string ans = m.createNode(10, -1);
    EXPECT_EQ("Error: Already exists", ans);
}

TEST(Lab57Test, CreateParentNotFound) {
    Manager m;
    std::string ans = m.createNode(10, 123);
    EXPECT_EQ("Error: Parent not found", ans);
    EXPECT_FALSE(m.Exist(10));
}

TEST(Lab57Test, CreateParentUnavailable) {
    Manager m;
    m.createNode(10, -1);     
    m.killNode(10);

    std::string ans = m.createNode(20, 10);
    EXPECT_EQ("Error: Parent is unavailable", ans);
    EXPECT_FALSE(m.Exist(20));
}

TEST(Lab57Test, CreateParentAlreadyHasChild) {
    Manager m;
    std::string ans1 = m.createNode(10, -1);
    EXPECT_NE(ans1.find("Ok:"), std::string::npos);

    std::string ans2 = m.createNode(20, 10);
    EXPECT_NE(ans2.find("Ok:"), std::string::npos);

    std::string ans3 = m.createNode(30, 10);
    EXPECT_EQ("Error: Parent already has child", ans3);

    EXPECT_FALSE(m.Exist(30));
}


TEST(Lab57Test, ExecCommandOk) {
    Manager m;
    m.createNode(10, -1);
    std::vector<int> nums{1, 2, 3};
    std::string ans = m.execCommand(10, nums);
    EXPECT_NE(ans.find("Ok:10: 6"), std::string::npos);
}

TEST(Lab57Test, ExecNodeNotFound) {
    Manager m;
    std::vector<int> nums{10, 20};
    std::string ans = m.execCommand(50, nums);
    EXPECT_EQ("Error:50: Not found", ans);
}

TEST(Lab57Test, ExecNodeUnavailable) {
    Manager m;
    m.createNode(10, -1);
    m.killNode(10);

    std::vector<int> arr{10, 20};
    std::string ans = m.execCommand(10, arr);
    EXPECT_EQ("Error:10: Node is unavailable", ans);
}

TEST(Lab57Test, MultipleChain) {
    Manager m;
    EXPECT_EQ("Ok: ", m.createNode(10, -1).substr(0, 4)); 
    EXPECT_EQ("Ok: ", m.createNode(20, 10).substr(0, 4));
    EXPECT_EQ("Ok: ", m.createNode(30, 20).substr(0, 4));

    EXPECT_EQ("Ok: 1", m.pingNode(10));
    EXPECT_EQ("Ok: 1", m.pingNode(20));
    EXPECT_EQ("Ok: 1", m.pingNode(30));

    std::vector<int> numbers{5, 5, 5};
    std::string ans = m.execCommand(30, numbers);
    EXPECT_NE(ans.find("Ok:30: 15"), std::string::npos);
}

TEST(Lab57Test, KillIntermediateNode) {
    Manager m;
    m.createNode(10, -1); 
    m.createNode(20, 10);
    m.createNode(30, 20); 
    std::string ansKill = m.killNode(20);
    EXPECT_EQ("Ok", ansKill);

    EXPECT_EQ("Ok: 0", m.pingNode(20));
    EXPECT_EQ("Ok: 0", m.pingNode(30));
    EXPECT_EQ("Ok: 1", m.pingNode(10));
}

TEST(Lab57Test, KillNotFound) {
    Manager m;
    std::string ansKill = m.killNode(77);
    EXPECT_EQ("Error: Not found", ansKill);
}

TEST(Lab57Test, KillManagerNode) {
    Manager m;
    m.createNode(10, -1);
    m.createNode(20, 10);

    std::string ans = m.killNode(-1);
    EXPECT_EQ("Ok", ans);
    EXPECT_EQ("Ok: 0", m.pingNode(10));
    EXPECT_EQ("Ok: 0", m.pingNode(20));
}

