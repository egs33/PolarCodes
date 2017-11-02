#include "Channel.h"
#include <random>
#include <ctime>

BEC::BEC(double p) : p(p) {}

std::vector<int> BEC::channel(std::vector<int> x) {
    std::vector<int> ret(x.size());
    std::mt19937 mt(static_cast<unsigned int>(time(nullptr)));
    std::uniform_real_distribution<double> random(0.0, 1.0);

    for (int i = 0; i < x.size(); ++i) {
        if (p > random(mt)) {
            ret[i] = 2;
        } else {
            ret[i] = x[i];
        }
    }
    return ret;
}

double BEC::w(int y, int x) {
    if (y == x) {
        return 1 - p;
    } else if (y == 2) {
        return p;
    }
    return 0;
}


BSC::BSC(double p) : p(p) {}

std::vector<int> BSC::channel(std::vector<int> x) {
    std::vector<int> ret(x.size());
    std::mt19937 mt(static_cast<unsigned int>(time(nullptr)));
    std::uniform_real_distribution<double> random(0.0, 1.0);
    for (int i = 0; i < x.size(); ++i) {
        if (p > random(mt)) {
            ret[i] = 1 - x[i];
        } else {
            ret[i] = x[i];
        }
    }
    return ret;
}

double BSC::w(int y, int x) {
    if (y == x) {
        return 1 - p;
    } else if (y == 1 - x) {
        return p;
    }
    return 0;
}
