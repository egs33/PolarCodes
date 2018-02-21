#include <cmath>
#include <random>
#include <ctime>
#include "BEC.h"

BEC::BEC(double p) : p(p) {

}

double BEC::w(int y, int x) const {
    if (y == x) {
        return 1 - p;
    } else if (y == 2) {
        return p;
    }
    return 0;
}

double BEC::symmetricCapacity(int n, int i) const {
    if (n == 1) {
        return 1 - p;
    }
    if (i % 2 == 0) {
        return std::pow(symmetricCapacity(n / 2, i / 2), 2);
    }
    double temp = symmetricCapacity(n / 2, (i - 1) / 2);
    return 2 * temp - std::pow(temp, 2);
}

int BEC::channel(int x) const {
    static std::mt19937 mt(static_cast<unsigned int>(time(nullptr)));
    static std::uniform_real_distribution<double> random(0.0, 1.0);

    if (p > random(mt)) {
        return 2;
    }
    return x;
}


