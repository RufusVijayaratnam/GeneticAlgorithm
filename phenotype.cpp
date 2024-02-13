#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include "phenotype.hpp"
#include "points.hpp"

Phenotype::Phenotype(std::vector<int> permutation, std::shared_ptr<TSP_Points> pointsPtr) : permutation(permutation), pointsPtr(pointsPtr), permutationSize(permutation.size()) {
    score = evaluateTSP(permutation);
}

Phenotype::Phenotype() {}

double Phenotype::evaluateTSP(std::vector<int>& permutation) {
    int m = pointsPtr->points.size();
    if(permutationSize != m) {
        std::cout << "vector permutation.size() must equal to pointsPtr->points.size() but it is not\n";
        exit(1);
    }
    double pathLength = 0;
    int cx, cy;
    cx = pointsPtr->points[0].first;
    cy = pointsPtr->points[0].second;
    for(int i = 1; i < permutationSize; i++) {
        int idx = permutation[i];
        int nx, ny;
        nx = pointsPtr->points[idx].first;
        ny = pointsPtr->points[idx].second;
        pathLength += sqrt(pow(double(nx - cx), 2.0) + pow(double(ny - cy), 2.0));
        cx = nx;
        cy = ny;
    }
    int nx, ny;
    nx = pointsPtr->points[0].first;
    ny = pointsPtr->points[0].second;
    pathLength += sqrt(pow(double(nx - cx), 2.0) + pow(double(ny - cy), 2.0));
    return pathLength;
}

double Phenotype::getScore() const {
    return score;
}

std::shared_ptr<TSP_Points> Phenotype::getPointsPtr() {
    return pointsPtr;
}

std::vector<int> Phenotype::getPermutation() {
    return permutation;
}

int Phenotype::getPermutationSize() const {
    return permutationSize;
}

bool Phenotype::operator<(const Phenotype& b) const {
    return score < b.getScore();
}

bool Phenotype::operator>(const Phenotype& b) const {
    return score > b.getScore();
}

void Phenotype::printScoreInline() const {
    std::cout << score << ", ";
}

void Phenotype::printScoreNewLine() const {
    std::cout << score << "\n";
}

void Phenotype::printPermutationInline() const {
    for(auto p : permutation) {
        std::cout << std::setw(4) << p << ",";
    }
    std::cout << "\n";
}