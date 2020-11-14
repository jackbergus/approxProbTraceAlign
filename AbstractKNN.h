//
// Created by giacomo on 14/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_ABSTRACTKNN_H
#define FUZZYSTRINGMATCHING2_ABSTRACTKNN_H



#include <knn/brute_force.h>
#include <knn/kdtree_minkowski.h>
#include <Eigen/SparseQR>

typedef Eigen::MatrixXd Matrix;
typedef knn::Matrixi Matrixi;

class AbstractKNN {
    size_t dim, dataset_size;
    Matrix dataPoints;
    std::vector<Eigen::Triplet<double>> bulkInsertion;

public:
    AbstractKNN(size_t dim, size_t dataset_size) : dim{0}, dataset_size{0}, dataPoints(1, 1)  {


    }

    void insertVector(const std::vector<double>& vector) {
        size_t N = vector.size();
        dim = std::max(dim, N);
        for (size_t i = 0; i<N; i++) {
            const auto& arg = vector[i];
            if (arg >= std::numeric_limits<double>::epsilon()) bulkInsertion.emplace_back(i, dataset_size, arg);
        }
        dataset_size++;
    }

    void bulkInsertDataPoints() {
        Eigen::SparseMatrix<double, Eigen::RowMajor> matrix(dim, dataset_size);
        matrix.setFromTriplets(bulkInsertion.begin(), bulkInsertion.end());
        bulkInsertion.clear();
        dataPoints = Matrix(matrix);
    }

    Matrix generateQueryRepresentation(const std::vector<double>& vector) {
        Matrix query(dim, 1);
        assert(dim == vector.size());
        for (size_t i = 0; i<dim; i++) {
            const auto& arg = vector[i];
            if (arg >= std::numeric_limits<double>::epsilon()) query(i, 0) = arg;
        }
        return query;
    }

};


#endif //FUZZYSTRINGMATCHING2_ABSTRACTKNN_H
