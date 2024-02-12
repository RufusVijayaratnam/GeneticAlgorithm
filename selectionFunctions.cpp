#include <algorithm>
#include <random>
#include <iostream>
#include <time.h>
#include "phenotype.hpp"
#include "selectionFunctions.hpp"
#include "Population.hpp"

/// @brief Selects n individuals from the population for reproduction
/// @param population The population
/// @param n Number of individuals to select for variation
void Selection::linearRankingSelection(Population& population, int numToSelect) {
    using namespace std;
    population.clearSelected();
    int M = population.size();
    if(M < numToSelect) {
        cerr << "linearRankingSelection:\ncannot select more individuals than total population size\n";
        exit(-1);
    }
    static random_device rd;
    static mt19937 gen(rd);

    double beta = 1.5;
    double alpha = 2 - beta;
    population.sort();
    vector<int> ranks = vector<int>(population.size());
    vector<int> probabilities = vector<int>(population.size());
    iota(ranks.begin(), ranks.end(), 0);
    for(int rank : ranks) {
        probabilities[rank] = double(alpha + (beta - alpha) * double(rank) / double(M - 1)) / double(M);
    }

    population.reserveSelected(numToSelect);
    discrete_distribution distribution(probabilities.begin(), probabilities.end());
    for(int i = 0; i < numToSelect; i++) {
        population.select(distribution(gen));
    }
}

void Selection::truncateSelection(Population& population, int numToSelect) {
    population.clearSelected();
    if(population.size() < numToSelect) {
        std::cerr << "truncateSelection:\ncannot select more individuals than total population size\n";
        exit(-1);
    }
    population.sort();
    population.reserveSelected(numToSelect);
    for(int i = population.size() - 1; i > population.size() - numToSelect - 1; i--) {
        population.select(i);
    }
}

void Selection::setVerbosity(bool verbosity) {
    verbose = verbosity;
}



bool Selection::verbose = false;