#include <algorithm>
#include <iostream>
#include "PolarCode.h"

bool AscSecondElement(const std::pair<int, double> &left, const std::pair<int, double> &right) {
    return left.second > right.second;
}

PolarCode::PolarCode(unsigned int length, unsigned int codeDimension, Channel &channel) : channel(channel) {
    this->frozenBits = std::vector<int>(length - codeDimension);
    this->length = length;
    std::vector<std::pair<int, double >> symmetricCapacities(length);
    for (int i = 0; i < length; ++i) {
        symmetricCapacities[i] = std::make_pair(i, channel.symmetricCapacity(length, i));
    }
    std::sort(symmetricCapacities.begin(), symmetricCapacities.end(), AscSecondElement);

    this->informationSet = std::vector<int>(codeDimension);
    for (int i = 0; i < codeDimension; ++i) {
        this->informationSet[i] = symmetricCapacities[i].first;
    }
    std::sort(this->informationSet.begin(), this->informationSet.end());
}

std::vector<int> PolarCode::SuccessiveCancellationDecode(const std::vector<int> &y) const {
    return GnCosetCode::SuccessiveCancellationDecode(y, channel);
}

std::vector<int> PolarCode::LlrSCDecode(const std::vector<int> &y) const {
    return GnCosetCode::LlrSCDecode(y, channel);
}
