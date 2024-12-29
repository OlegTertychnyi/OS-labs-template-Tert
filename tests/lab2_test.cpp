#include "gauss.h"
#include <gtest/gtest.h>
#include <cmath>
#include <chrono>
#include <random>
#include <vector>
#include <limits>

class GaussSolverTest : public ::testing::Test {
protected:
    GaussSolver solver{12};

    std::vector<std::vector<double>> generateRandomMatrix(int size) {
        std::vector<std::vector<double>> matrix(size, std::vector<double>(size));
        std::mt19937 gen(42);
        std::uniform_real_distribution<> dist(-100.0, 100.0);

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                matrix[i][j] = dist(gen);
            }
            matrix[i][i] += size * 10;
        }
        return matrix;
    }

    std::vector<double> generateRandomVector(int size) {
        std::vector<double> vec(size);
        std::mt19937 gen(42);
        std::uniform_real_distribution<> dist(-100.0, 100.0);
        for (int i = 0; i < size; ++i) {
            vec[i] = dist(gen);
        }
        return vec;
    }
};

TEST_F(GaussSolverTest, SingleThreadedCorrectness2x2) {
    std::vector<std::vector<double>> matrix = {
        {2.0, 4.0},
        {3.0, 5.0}
    };
    std::vector<double> rhs = {8.0, 11.0};

    std::vector<double> expected_result = {2.0, 1.0};
    auto result = solver.SolveSingleThreaded(matrix, rhs);

    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_NEAR(result[i], expected_result[i], 1e-9) << "Mismatch at index " << i;
    }
}

TEST_F(GaussSolverTest, SingleThreadedCorrectness) {
    std::vector<std::vector<double>> matrix = generateRandomMatrix(1000);
    std::vector<double> rhs = generateRandomVector(1000);

    std::vector<double> result = solver.SolveSingleThreaded(matrix, rhs);

    for (double value : result) {
        EXPECT_FALSE(std::isnan(value));
    }
}

TEST_F(GaussSolverTest, MultiThreadedCorrectness) {
    std::vector<std::vector<double>> matrix = generateRandomMatrix(1000);
    std::vector<double> rhs = generateRandomVector(1000);

    std::vector<double> result = solver.SolveMultiThreaded(matrix, rhs);

    for (double value : result) {
        EXPECT_FALSE(std::isnan(value));
    }
}

TEST_F(GaussSolverTest, MultiThreadSameResult) {
    std::vector<std::vector<double>> matrix = generateRandomMatrix(100);
    std::vector<double> rhs = generateRandomVector(100);

    std::vector<double> single_result = solver.SolveSingleThreaded(matrix, rhs);
    std::vector<double> multi_result = solver.SolveMultiThreaded(matrix, rhs);

    for (size_t i = 0; i < single_result.size(); ++i) {
        EXPECT_NEAR(single_result[i], multi_result[i], 1e-9) << "Mismatch at index " << i;
    }
}

TEST_F(GaussSolverTest, MultiThreadSameResultLarge) {
    std::vector<std::vector<double>> matrix = generateRandomMatrix(100);
    std::vector<double> rhs = generateRandomVector(100);

    std::vector<double> single_result = solver.SolveSingleThreaded(matrix, rhs);
    std::vector<double> multi_result = solver.SolveMultiThreaded(matrix, rhs);

    for (size_t i = 0; i < single_result.size(); ++i) {
        EXPECT_NEAR(single_result[i], multi_result[i], 1e-9) << "Mismatch at index " << i;
    }
}

TEST_F(GaussSolverTest, MultiThreadPerformanceLarge) {
    std::vector<std::vector<double>> matrix = generateRandomMatrix(1000);
    std::vector<double> rhs = generateRandomVector(1000);

    std::chrono::high_resolution_clock::time_point start_single = std::chrono::high_resolution_clock::now();
    solver.SolveSingleThreaded(matrix, rhs);
    std::chrono::high_resolution_clock::time_point end_single = std::chrono::high_resolution_clock::now();

    std::chrono::high_resolution_clock::time_point start_multi = std::chrono::high_resolution_clock::now();
    solver.SolveMultiThreaded(matrix, rhs);
    std::chrono::high_resolution_clock::time_point end_multi = std::chrono::high_resolution_clock::now();

    long long single_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_single - start_single).count();
    long long multi_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_multi - start_multi).count();

    EXPECT_LT(multi_time, single_time) << "Многопоточная версия не быстрее однопоточной на больших данных";
}
