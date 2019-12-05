#include <iostream>
#include <string>
#include <vector>
#include <omp.h>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <boost/program_options.hpp>

#include "factors.cpp"
#include "dataset.cpp"

using namespace std;
namespace po = boost::program_options;
typedef Eigen::SparseMatrix<double, Eigen::RowMajor> SparseMatrix;


SparseMatrix readDataset(Dataset& dataset){
    cout << "Reading ratings.." << endl;
    // Load ratings
    Rating rating;
    cout << "Users: " << dataset.users() << " "
         << "Items: " << dataset.items() << " "
         << endl;
    SparseMatrix ratings(dataset.users(), dataset.items());
    for(dataset.reset(); !dataset.end(); dataset.next()){
        rating = dataset.get();
        ratings.insert(rating.user, rating.item) = rating.rating;
    };
    std::cout << "Done." << std::endl;
    return ratings;
}


int main(int argc, char *argv[]){
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("ratings-file", po::value<string>(), "File with user ratings.")
        ("factors", po::value<unsigned int>()->default_value(30), "Number of latent factors.")
        ("lambda", po::value<double>()->default_value(0.1), "Regularization coefficient")
        ("iters", po::value<unsigned int>()->default_value(20), "Number of iterations.")
        ("nthreads", po::value<unsigned int>()->default_value(1), "number of threads to use")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }
    std::string path =  vm["ratings-file"].as<string>();
    unsigned int factors = vm["factors"].as<unsigned int>();
    double lambda = vm["lambda"].as<double>();
    unsigned int iters = vm["iters"].as<unsigned int>();
    unsigned int nthreads = vm["nthreads"].as<unsigned int>();

    Eigen::setNbThreads(1);
    omp_set_num_threads(nthreads);
    Dataset dataset(path, '\t');
    SparseMatrix ratings = readDataset(dataset);
    LFM model(ratings, factors, iters, lambda);
}