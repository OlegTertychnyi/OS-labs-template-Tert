#include <iostream>
#include <vector>
#include <chrono>
#include "gauss.h"

int main(int argc, char* argv[]) {
    int max_threads = 4; 
    if (argc > 1) {
        max_threads = std::stoi(argv[1]);
    }

    GaussSolver solver(max_threads);

    int size = 100;
    std::vector<std::vector<double>> matrix(size, std::vector<double>(size));
    std::vector<double> rhs(size);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = rand() % 100 + 1;
        }
        rhs[i] = rand() % 100 + 1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<double> solution = solver.SolveMultiThreaded(matrix, rhs);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;
    std::cout << "Время многопоточного решения: " << diff.count() << " секунд\n";

    start = std::chrono::high_resolution_clock::now();
    solution = solver.SolveSingleThreaded(matrix, rhs);
    end = std::chrono::high_resolution_clock::now();

    diff = end - start;
    std::cout << "Время однопоточного решения: " << diff.count() << " секунд\n";

    return 0;
}
