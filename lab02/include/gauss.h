#ifndef GAUSS_H
#define GAUSS_H

#include <vector>
#include <pthread.h>

class GaussSolver {
public:
    explicit GaussSolver(size_t maxThreads);
    ~GaussSolver();
    
    std::vector<double> SolveSingleThreaded(const std::vector<std::vector<double>>& matrix, const std::vector<double>& rhs);
    std::vector<double> SolveMultiThreaded(const std::vector<std::vector<double>>& matrix, const std::vector<double>& rhs);

private:
    size_t maxThreads;
    pthread_mutex_t mutex;
    pthread_barrier_t barrier;
    std::vector<pthread_t> threads;
    std::vector<std::vector<double>> matrix;
    std::vector<double> rhs;
    std::vector<double> result;

    struct ThreadData {
        GaussSolver* solver;
        size_t threadId;
    };
    
    void ForwardEliminationSingleThreaded();
    void ForwardEliminationMultiThreaded(size_t threadId);
    void BackwardSubstitution(std::vector<double>& result);
    static void* ThreadTask(void* args);
};

#endif
