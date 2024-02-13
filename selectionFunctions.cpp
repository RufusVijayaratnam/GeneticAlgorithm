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
    if(verbose) cout << "linearRankingSelection\n";
    population.clearSelected();
    int M = population.size();
    if(M < numToSelect) {
        cerr << "linearRankingSelection:\ncannot select more individuals than total population size\n";
        exit(-1);
    }

    double beta = 1.5;
    double alpha = 2 - beta;
    population.sort();
    if(verbose) {
        cout << "after sorting\n";
        population.printScoresInline();
        cout << "\n";
    }
    vector<int> ranks = vector<int>(population.size());
    vector<double> probabilities = vector<double>(population.size());
    iota(ranks.begin(), ranks.end(), 0);
    double sum = 0;
    if(verbose) cout << "Probabilities: ";
    for(int rank : ranks) {
        probabilities[double(M) - rank - 1] = double(alpha + (beta - alpha) * double(rank) / double(M - 1)) / double(M);
        sum += probabilities[rank];
        if(verbose) cout << probabilities[rank] << ", ";
    }
    if(verbose) cout << "\n";
    if(verbose) cout << "Sum of probabilities = " << sum << "\n";

    population.reserveSelected(numToSelect);
    default_random_engine generator;
    discrete_distribution<int> dist(probabilities.begin(), probabilities.end());
    for(int i = 0; i < numToSelect; i++) {
        int selected = dist(generator);
        if(verbose) cout << "selected member: " << selected << "\n";
        population.select(dist(generator));
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