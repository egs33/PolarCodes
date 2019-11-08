#include <stdexcept>
#include <algorithm>
#include <map>
#include <cmath>
#include <iostream>
#include "GnCosetCode.h"
#include "Channel.h"


GnCosetCode::GnCosetCode() = default;

GnCosetCode::GnCosetCode(unsigned int length, std::vector<int> informationSet, std::vector<int> frozenBits) :
        length(length), informationSet(informationSet), frozenBits(frozenBits) {
    if (length != informationSet.size() + frozenBits.size()) {
        throw std::invalid_argument("invalid length");
    }
    std::sort(this->informationSet.begin(), this->informationSet.end());
}

GnCosetCode::GnCosetCode(unsigned int length, const std::vector<int> &informationSet,
                         const std::vector<int> &frozenBits, const std::vector<std::pair<int, int>> &sendBits) :
        length(length), informationSet(informationSet), frozenBits(frozenBits), sendBits(sendBits) {
    if (length != informationSet.size() + frozenBits.size()) {
        throw std::invalid_argument("invalid length");
    }
    std::sort(this->informationSet.begin(), this->informationSet.end());
}

GnCosetCode::GnCosetCode(unsigned int length, std::vector<int> informationSet) :
        length(length), informationSet(informationSet) {
    if (length < informationSet.size()) {
        throw std::invalid_argument("invalid length");
    }
    std::sort(this->informationSet.begin(), this->informationSet.end());
    this->frozenBits = std::vector<int>(length - informationSet.size(), 0);
}

GnCosetCode::GnCosetCode(unsigned int length, const std::vector<int> &informationSet,
                         const std::vector<std::pair<int, int>> &sendBits) : length(length),
                                                                             informationSet(informationSet),
                                                                             sendBits(sendBits) {
    if (length < informationSet.size()) {
        throw std::invalid_argument("invalid length");
    }
    std::sort(this->informationSet.begin(), this->informationSet.end());
    this->frozenBits = std::vector<int>(length - informationSet.size(), 0);
}

GnCosetCode::GnCosetCode(const std::vector<std::pair<int, int>> &sendBits) : sendBits(sendBits) {}

std::vector<int> GnCosetCode::encode(const std::vector<int> &information) const {
    const std::vector<int> &u = createU(information);
    auto x = Channel::combine(u);
    for (auto &&sendBit : sendBits) {
        x.push_back(Channel::combinedAt(u, sendBit.first, sendBit.second));
    }
    return x;
}

std::vector<int> GnCosetCode::createU(const std::vector<int> &information) const {
    if (this->informationSet.size() != information.size()) {
        throw std::invalid_argument("invalid length");
    }
    std::vector<int> u(this->length);
    for (int i = 0, informationIndex = 0; i < this->length; ++i) {
        if (informationSet.size() > informationIndex && this->informationSet[informationIndex] == i) {
            u[i] = information[informationIndex];
            ++informationIndex;
        } else {
            u[i] = this->frozenBits[i - informationIndex];
        }
    }
    return u;
}

unsigned int GnCosetCode::getLength() const {
    return length;
}

const std::vector<int> &GnCosetCode::getInformationSet() const {
    return informationSet;
}

const std::vector<int> &GnCosetCode::getFrozenBits() const {
    return frozenBits;
}

std::vector<int> GnCosetCode::SuccessiveCancellationDecode(const std::vector<int> &y, const Channel &channel) const {
    std::vector<int> decoded, ret;
    std::vector<int> information(getInformationSet().size());
    for (int i = 0, informationIndex = 0; i < getLength(); ++i) {
        if (getInformationSet().size() > informationIndex && i == getInformationSet()[informationIndex]) {
            int decodedBit = channel.w(getLength(), y, decoded, 0) >= channel.w(getLength(), y, decoded, 1) ? 0 : 1;
            decoded.push_back(decodedBit);
            ret.push_back(decodedBit);
            ++informationIndex;
        } else {
            decoded.push_back(getFrozenBits()[i - informationIndex]);
        }
    }
    return ret;
}

std::vector<int> GnCosetCode::LlrSCDecode(const std::vector<int> &y, const Channel &channel) const {
    std::vector<int> decoded, ret;
    std::vector<int> information(getInformationSet().size());
    for (int i = 0, informationIndex = 0; i < getLength(); ++i) {
        if (getInformationSet().size() > informationIndex && i == getInformationSet()[informationIndex]) {
            int decodedBit = channel.logLikelihoodRatio(getLength(), y, decoded) > 0 ? 0 : 1;
            decoded.push_back(decodedBit);
            ret.push_back(decodedBit);
            ++informationIndex;
        } else {
            decoded.push_back(getFrozenBits()[i - informationIndex]);
        }
    }
    return ret;
}

double phi(double pre, double llr, int u) {
    return pre + std::log(1 + std::exp((u == 0 ? -1 : 1) * llr));
}

std::vector<int> GnCosetCode::SCLDecode(const std::vector<int> &received, const int L, const Channel &channel) const {
    const std::vector<int> y(received.begin(), received.begin() + getLength());
    std::map<std::pair<int, int>, int> interNodes;
    for (int i = 0; i < sendBits.size(); ++i) {
        interNodes[sendBits[i]] = received[getLength() + i];
    }
    std::vector<std::pair<std::vector<int> *, double>> list;
    list.emplace_back(std::make_pair(new std::vector<int>, 0));
    for (int i = 0, informationIndex = 0; i < getLength() - 1; ++i) {
        if (getInformationSet().size() <= informationIndex || i != getInformationSet()[informationIndex]) {
            // frozen bit
            for (auto &&path : list) {
                path.second = phi(path.second,
                                  channel.logLikelihoodRatioAt(getLength(), y, interNodes, *(path.first), 0, i),
                                  getFrozenBits()[i - informationIndex]);
                path.first->push_back(getFrozenBits()[i - informationIndex]);
            }
        } else {
            ++informationIndex;
            if (list.size() < L) {
                duplicatePath(list, y, interNodes, channel);
                continue;
            }
            std::vector<std::pair<std::vector<int> *, double>> candidateList;
            for (auto &&path : list) {
                auto v0 = new std::vector<int>(*(path.first));
                auto v1 = new std::vector<int>(*(path.first));

                candidateList.emplace_back(std::make_pair(v0, phi(
                        path.second, channel.logLikelihoodRatioAt(getLength(), y, interNodes, *v0, 0, i), 0)
                ));
                candidateList.emplace_back(std::make_pair(v1, phi(
                        path.second, channel.logLikelihoodRatioAt(getLength(), y, interNodes, *v1, 0, i), 1)
                ));
                v0->push_back(0);
                v1->push_back(1);
                delete path.first;
            }
            list.clear();
            std::sort(candidateList.begin(), candidateList.end(),
                      [](const std::pair<std::vector<int> *, double> &a, const std::pair<std::vector<int> *, double> &b) {
                          return a.second < b.second;
                      });
            for (int j = 0; j < L; ++j) {
                list.push_back(candidateList[j]);
            }
            for (int j = L; j < candidateList.size(); ++j) {
                delete candidateList[j].first;
            }
        }
    }
    duplicatePath(list, y, interNodes, channel);
    double minPathMetrics = list[0].second;
    std::vector<int> *min = list[0].first;
    for (auto &&path : list) {
        if (path.second < minPathMetrics) {
            minPathMetrics = path.second;
            min = path.first;
        }
    }
    std::vector<int> ret;
    for (auto &&i : getInformationSet()) {
        ret.push_back((*min)[i]);
    }
    for (auto &&path : list) {
        delete path.first;
    }
    return ret;
}

void GnCosetCode::duplicatePath(std::vector<std::pair<std::vector<int> *, double>> &list, const std::vector<int> &y,
                                const std::map<std::pair<int, int>, int> &interNodes, const Channel &channel) const {
    const int originalSize = list.size();
    for (int j = 0; j < originalSize; ++j) {
        const double originalPathMetrics = list[j].second;
        auto newCandidate = new std::vector<int>(*(list[j].first));
        list[j].second = phi(originalPathMetrics,
                             channel.logLikelihoodRatioAt(getLength(), y, interNodes, *(list[j].first), 0,
                                                          list[j].first->size()), 0);
        list.emplace_back(std::make_pair(
                newCandidate,
                phi(originalPathMetrics,
                    channel.logLikelihoodRatioAt(getLength(), y, interNodes, *newCandidate, 0, list[j].first->size()),
                    1)
        ));
        list[j].first->push_back(0);
        newCandidate->push_back(1);
    }
}

std::vector<std::vector<int>> GnCosetCode::SCLDecode2(const std::vector<int> &received, const int L, const Channel &channel) const {
    const std::vector<int> y(received.begin(), received.begin() + getLength());
    std::map<std::pair<int, int>, int> interNodes;
    for (int i = 0; i < sendBits.size(); ++i) {
        interNodes[sendBits[i]] = received[getLength() + i];
    }
    std::vector<std::pair<std::vector<int> *, double>> list;
    list.emplace_back(std::make_pair(new std::vector<int>, 0));
    for (int i = 0, informationIndex = 0; i < getLength(); ++i) {
        if (getInformationSet().size() <= informationIndex || i != getInformationSet()[informationIndex]) {
            // frozen bit
            for (auto &&path : list) {
                path.second = phi(path.second,
                                  channel.logLikelihoodRatioAt(getLength(), y, interNodes, *(path.first), 0, i),
                                  getFrozenBits()[i - informationIndex]);
                path.first->push_back(getFrozenBits()[i - informationIndex]);
            }
        } else {
            ++informationIndex;
            if (list.size() < L) {
                duplicatePath(list, y, interNodes, channel);
                continue;
            }
            std::vector<std::pair<std::vector<int> *, double>> candidateList;
            for (auto &&path : list) {
                auto v0 = new std::vector<int>(*(path.first));
                auto v1 = new std::vector<int>(*(path.first));

                candidateList.emplace_back(std::make_pair(v0, phi(
                        path.second, channel.logLikelihoodRatioAt(getLength(), y, interNodes, *v0, 0, i), 0)
                ));
                candidateList.emplace_back(std::make_pair(v1, phi(
                        path.second, channel.logLikelihoodRatioAt(getLength(), y, interNodes, *v1, 0, i), 1)
                ));
                v0->push_back(0);
                v1->push_back(1);
                delete path.first;
            }
            list.clear();
            std::sort(candidateList.begin(), candidateList.end(),
                      [](const std::pair<std::vector<int> *, double> &a, const std::pair<std::vector<int> *, double> &b) {
                          return a.second < b.second;
                      });
            for (int j = 0; j < L; ++j) {
                list.push_back(candidateList[j]);
            }
            for (int j = L; j < candidateList.size(); ++j) {
                delete candidateList[j].first;
            }
        }
    }
//    duplicatePath(list, y, interNodes, channel);
//    double minPathMetrics = list[0].second;
//    std::vector<int> *min = list[0].first;
//    for (auto &&path : list) {
//        if (path.second < minPathMetrics) {
//            minPathMetrics = path.second;
//            min = path.first;
//        }
//    }
//    std::vector<int> ret;
//    for (auto &&i : getInformationSet()) {
//        ret.push_back((*min)[i]);
//    }
//    for (auto &&path : list) {
//        delete path.first;
//    }
    std::vector<std::vector<int>> ret;
    for(auto l: list) {
        std::vector<int> temp;
        for (auto &&i : getInformationSet()) {
            temp.push_back((*l.first)[i]);
        }
        ret.emplace_back(temp);
    }
    return ret;
}