#include <random>
#include "BSC.h"

BSC::BSC(double p) : p(p) {}

int BSC::channel(int x) const {
    static std::mt19937 mt(static_cast<unsigned int>(time(nullptr)));
    static std::uniform_real_distribution<double> random(0.0, 1.0);

    if (p > random(mt)) {
        return 1 - x;
    }
    return x;
}

double BSC::w(int y, int x) const {
    if (y == x) {
        return 1 - p;
    } else if (y == 1 - x) {
        return p;
    }
    return 0;
}

double BSC::symmetricCapacity(int n, int i) const {
    // Unimplemented
    return 0;
}
