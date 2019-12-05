/*
 * Latent Factor Model
 * 
 * This model asumes that the target/observed matrix R
 * can be expresed as the product of two lower rank matrices
 * U and V, that is:
 *  R ~= U @ V 
 * 
 * The implementation is based on the Altentating Least Squares
 * algorithm proposed in [1].
 *
 * References:
 * [1] Large-scale Parallel Collaborative Filtering for the Netflix Prize
 */
#include <math.h>
#include <chrono>
#include <vector>
// #include <Eigen/Dense>
// #include <Eigen/Sparse>
#include "utils.cpp"


typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;
typedef Eigen::SparseMatrix<double, Eigen::RowMajor> SparseMatrix;
typedef std::chrono::high_resolution_clock Clock;


class LFM{
private:
    unsigned int niters;
    unsigned int m, n, k; /* Latent Factors Dimension */
    double lambda;        /* Regularization parameter */
    Matrix U, Vt;         /* Latent Factors */

    void updateU(SparseMatrix& target); /* ALS U step */
    void updateV(SparseMatrix& target); /* ALS V step */

public:
    LFM(SparseMatrix& target, unsigned int k, unsigned int niters,
        double lbda=0.1);
    double predict(unsigned int i, unsigned int j);
    double MSE(SparseMatrix& target);
    double mean_prediction(SparseMatrix& target);
    ~LFM();
};

LFM::LFM(SparseMatrix& target, unsigned int k, unsigned int niters,
         double lbda){
    this->m = target.rows();
    this->n = target.cols();
    this->k = k;
    this->lambda = lbda;
    this->niters = niters;
    U = Matrix::Constant(m, k, 0.1);
    Vt = Matrix::Constant(n, k, 0.1);
    auto start = Clock::now();
    auto end = Clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    for(int i = 0; i < niters; i++){
        time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        start = Clock::now();
        std::cout << "Iteration: " << i
                  << " | MSE: " << MSE(target)
                  << " | Mean predition: " << mean_prediction(target) 
                  << " | Time: " << time << std::endl;
        updateU(target);
        updateV(target);
        end = Clock::now();
    }
};


void LFM::updateU(SparseMatrix& target){
    int i;
    std::vector<unsigned int> I;
    Matrix M, A;
    Vector b;
    #pragma omp parallel for \
        private(i, I, M, A, b) \
        shared(target, Vt, U, lambda, n, m, k) \
        schedule(guided)
    for (i = 0; i < m; i++){
        I = non_zero_cols(target, i);
        M = get_rows(Vt, I);
        A = M.transpose() * M + lambda * Matrix::Identity(k, k);
        b = M.transpose() * get_row(target, i, I);
        U.row(i) = A.ldlt().solve(b);
    }
}

void LFM::updateV(SparseMatrix& target){
    int i;
    std::vector<unsigned int> I;
    Matrix M, A;
    Vector b;
    #pragma omp parallel for \
        private(i, I, M, A, b) \
        shared(target, Vt, U, lambda, n, m, k) \
        schedule(guided)
    for (int i = 0; i < n; i++){
        std::vector<unsigned int> I = non_zero_rows(target, i);
        Matrix M = get_rows(U, I);
        Matrix A = M.transpose() * M + lambda * Matrix::Identity(k, k);
        Vector b = M.transpose() * get_col(target, i, I);
        Vt.row(i) = A.ldlt().solve(b);
    }
}

double LFM::predict(unsigned int i, unsigned int j){
    double p = 0;
    for(size_t r = 0; r < k; ++r) p += U(i, r) * Vt(j, r);
    return p;
};

double LFM::MSE(SparseMatrix& target){
    double mse = 0;
    for (int k = 0; k < target.outerSize(); ++k)
        for (SparseMatrix::InnerIterator it(target, k); it; ++it)
            mse += pow(it.value() - predict(it.row(), it.col()), 2);
    mse /= target.nonZeros();
    return mse;
}

double LFM::mean_prediction(SparseMatrix& target){
    double mp = 0;
    for (int k = 0; k < target.outerSize(); ++k)
        for (SparseMatrix::InnerIterator it(target, k); it; ++it)
            mp += predict(it.row(), it.col());
    mp /= target.nonZeros();
    return mp;
}

LFM::~LFM(){};