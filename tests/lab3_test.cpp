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
    explicit TempFile(const std::string& fName) : filename(fName) {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Не удалось создать временный файл: " + filename);
        }
    }

    const std::string& getFilename() const {
        return filename;
    }

    ~TempFile() {
        if (std::filesystem::exists(filename)) {
            std::remove(filename.c_str());
        }
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
    const std::string output_file = std::filesystem::absolute("output.txt").string();

    const std::string parent_input = output_file + "\n10 20 30\n";
    std::istringstream input_stream(parent_input);

    ParentMain(input_stream);

    std::ifstream result_file(output_file);
    ASSERT_TRUE(result_file.is_open()) << "Failed to open output file";

    std::string content;
    std::getline(result_file, content);
    EXPECT_EQ(content, "60");

    result_file.close();
    std::filesystem::remove(output_file);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}