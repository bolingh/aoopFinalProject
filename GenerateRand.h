#include <cstdlib>
#include <random>
double get_rand(double left, double right) {
    static mt19937 mt(random_device{}());
    uniform_real_distribution<double> dis(left, right);
    return dis(mt);
}

double get_rand(double right = 1.0) {
    return get_rand(0.0, right);
}