#include <cstdlib>
#include <random>
double get_rand(double left = 0.0, double right = 1.0) {
    static mt19937 mt(random_device{}());
    uniform_real_distribution<double> dis(left, right);
    return dis(mt);
}