#include <Eigen/Dense>
#include <Eigen/Sparse>

using namespace std;
typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;
typedef Eigen::SparseMatrix<double, Eigen::RowMajor> SparseMatrix;


vector<unsigned int> non_zero_cols(SparseMatrix m, unsigned int k){
    vector<unsigned int> res;
    for (SparseMatrix::InnerIterator it(m, k); it; ++it)
        res.push_back(it.col());
    return res;
};

vector<unsigned int> non_zero_rows(SparseMatrix m, unsigned int k){
    vector<unsigned int> res;
    for(int i = 0; i < m.rows(); i++)
        if (m.coeff(i, k) != 0) res.push_back(i);
    return res;
};


Matrix get_rows(Matrix& m, vector<unsigned int>& rows){
    Matrix res(rows.size(), m.cols());
    for(int i = 0; i < rows.size(); i++)
        for (int j=0; j < m.cols(); j++)
            res(i, j) = m(rows[i], j);
    return res;
}


Matrix get_cols(Matrix& m, vector<unsigned int>& cols){
    Matrix res(m.rows(), cols.size());
    for(int i = 0; i < m.rows(); i++)
        for (int j = 0; j < cols.size(); j++)
            res(i, j) = m(i, cols[j]);
    return res;
}


Matrix get_rows(SparseMatrix& m, vector<unsigned int>& rows){
    Matrix res(rows.size(), m.cols());
    for(int i = 0; i < rows.size(); i++)
        for (int j=0; j < m.cols(); j++)
            res(i, j) = m.coeff(rows[i], j);
    return res;
}


Matrix get_cols(SparseMatrix& m, vector<unsigned int>& cols){
    Matrix res(m.rows(), cols.size());
    for(int i = 0; i < m.rows(); i++)
        for (int j = 0; j < cols.size(); j++)
            res(i, j) = m.coeff(i, cols[j]);
    return res;
}


Matrix get_submatrix(SparseMatrix& m, vector<unsigned int>& rows, vector<unsigned int>& cols){
    Matrix res(m.rows(), cols.size());
    for(int i = 0; i < rows.size(); i++)
        for (int j = 0; j < cols.size(); j++)
            res(i, j) = m.coeff(rows[i], cols[j]);
    return res;
}


Vector get_row(SparseMatrix& m, unsigned int row, vector<unsigned int>& cols){
    Vector res(cols.size());
        for (int j = 0; j < cols.size(); j++){
            res(j) = m.coeff(row, cols[j]);
        }
    return res;
}


Vector get_col(SparseMatrix& m, unsigned int col, vector<unsigned int>& rows){
    Vector res(rows.size());
        for (int i = 0; i < rows.size(); i++){
            res(i) = m.coeff(rows[i], col);
        }
    return res;
}
