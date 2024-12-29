#include "gauss.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

GaussSolver::GaussSolver(size_t maxThreads) : maxThreads(maxThreads) {
    pthread_mutex_init(&mutex, nullptr);
    pthread_barrier_init(&barrier, nullptr, maxThreads);
}

GaussSolver::~GaussSolver() {
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex);
}

void GaussSolver::ForwardEliminationSingleThreaded() {
    size_t n = matrix.size();
    for (size_t pivot = 0; pivot < n - 1; ++pivot) {
        if (std::fabs(matrix[pivot][pivot]) < 1e-10) {
            throw std::runtime_error("Zero pivot encountered.");
        }

        for (size_t row = pivot + 1; row < n; ++row) {
            double factor = matrix[row][pivot] / matrix[pivot][pivot];
            for (size_t col = pivot; col < n; ++col) {
                matrix[row][col] -= factor * matrix[pivot][col];
            }
            rhs[row] -= factor * rhs[pivot];
        }
    }
}

void GaussSolver::ForwardEliminationMultiThreaded(size_t threadId) {
    size_t n = matrix.size();
    for (size_t pivot = 0; pivot < n - 1; ++pivot) {
        if (std::fabs(matrix[pivot][pivot]) < 1e-10) {
            throw std::runtime_error("Zero pivot encountered.");
        }
        pthread_barrier_wait(&barrier);

        for (size_t row = pivot + 1 + threadId; row < n; row += maxThreads) {
            double factor = matrix[row][pivot] / matrix[pivot][pivot];
            for (size_t col = pivot; col < n; ++col) {
                matrix[row][col] -= factor * matrix[pivot][col];
            }
            rhs[row] -= factor * rhs[pivot];
        }

        pthread_barrier_wait(&barrier);
    }
}

void* GaussSolver::ThreadTask(void* args) {
    auto* data = static_cast<ThreadData*>(args);
    GaussSolver* solver = data->solver;
    size_t threadId = data->threadId;

    solver->ForwardEliminationMultiThreaded(threadId);

    pthread_barrier_wait(&solver->barrier);

    if (threadId == 0) {
        solver->BackwardSubstitution(solver->result);
    }

    delete data;
    return nullptr;
}

std::vector<double> GaussSolver::SolveSingleThreaded(const std::vector<std::vector<double>>& inputMatrix,
                                                     const std::vector<double>& inputRhs) {
    matrix = inputMatrix;
    rhs = inputRhs;
    size_t n = matrix.size();
    result.resize(n, 0.0);

    ForwardEliminationSingleThreaded();
    BackwardSubstitution(result);

    return result;
}

std::vector<double> GaussSolver::SolveMultiThreaded(const std::vector<std::vector<double>>& inputMatrix,
                                                    const std::vector<double>& inputRhs) {
    matrix = inputMatrix;
    rhs = inputRhs;
    size_t n = matrix.size();
    result.resize(n, 0.0);

    pthread_barrier_init(&barrier, nullptr, maxThreads);

    std::vector<pthread_t> threads(maxThreads);

    for (size_t i = 0; i < maxThreads; ++i) {
        auto* data = new ThreadData{this, i};
        pthread_create(&threads[i], nullptr, ThreadTask, data);
    }

    for (size_t i = 0; i < maxThreads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    pthread_barrier_destroy(&barrier);

    return result;
}


void GaussSolver::BackwardSubstitution(std::vector<double>& result) {
    size_t n = matrix.size();
    for (int i = n - 1; i >= 0; --i) {
        result[i] = rhs[i];
        for (size_t j = i + 1; j < n; ++j) {
            result[i] -= matrix[i][j] * result[j];
        }
        result[i] /= matrix[i][i];
    }
}
