double get_uniary_rand(int range = (int)1e7) {
    mt19937 mt(time(NULL));
    uniform_int_distribution<int> dis(0, range);
    return (double)dis(mt) / range;
}
double get_rand(double l = 0.0, double r = 1.0) {
    
}