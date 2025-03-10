#include <gtest/gtest.h>
#include <vector>
#include "utils.h"
#include "parent.h"
#include <array>
#include <filesystem>
#include <fstream>
#include <memory>


class TempFile {
public:
    TempFile(const std::string& fName) : filename(fName) {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Не удалось создать временный файл: " + filename);
        }
    }

    const std::string& getFilename() const {
        return filename;
    }

    ~TempFile() {
        if (std::filesystem::exists(filename)){
            std::remove(filename.c_str());
        };
    }

private:
    std::string filename;
};


std::string ReadFileContent(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

TEST(UtilsTest, NumberSumTest) {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    int sum = NumberSum(numbers);
    EXPECT_EQ(sum, 15);
}

TEST(UtilsTest, ReadNumbersTest) {
    std::istringstream input("1 2 3\n");
    std::cin.rdbuf(input.rdbuf());

    std::vector<int> expected = {1, 2, 3};
    EXPECT_EQ(ReadNumbers(), expected);
}

TEST(ProcessTest, ParentProcessTest) {
    int pipefd[2];
    ASSERT_EQ(pipe(pipefd), 0) << "Pipe creation failed";
    TempFile filename("output.txt");

    std::istringstream input_stream("/home/optert/OS-labs-template-Tert/build/output.txt\n10 5 15\n");    
    // std::string filename = "output.txt";

    ParentMain(input_stream);

    std::string content = ReadFileContent(filename.getFilename());
    EXPECT_EQ(content, "");
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}