#ifndef selection_functions_hpp
#define selection_functions_hpp
#include <random>
#include <algorithm>
template<typename T>
class Phenotype;

template<typename T>
class Population;

namespace Selection {
    template<typename T>
    void linearRankingSelection(Population<T>& population, int numToSelect, bool verbose=false) {
        if(verbose) std::cout << "linearRankingSelection\n";
        population.clearSelected();
        int M = population.size();
        if(M < numToSelect) {
            std::cerr << "linearRankingSelection:\ncannot select more individuals than total population size\n";
            exit(-1);
        }

        double beta = 1.5;
        double alpha = 2 - beta;
        population.sort();
        if(verbose) {
            std::cout << "after sorting\n";
            population.printScoresInline();
            std::cout << "\n";
        }
        std::vector<int> ranks = std::vector<int>(population.size());
        std::vector<double> probabilities = std::vector<double>(population.size());
        std::iota(ranks.begin(), ranks.end(), 0);
        double sum = 0;
        if(verbose) std::cout << "Probabilities: ";
        for(int rank : ranks) {
            probabilities[double(M) - rank - 1] = double(alpha + (beta - alpha) * double(rank) / double(M - 1)) / double(M);
            sum += probabilities[rank];
            if(verbose) std::cout << probabilities[rank] << ", ";
        }
        if(verbose) std::cout << "\n";
        if(verbose) std::cout << "Sum of probabilities = " << sum << "\n";

        population.reserveSelected(numToSelect);
        std::default_random_engine generator;
        std::discrete_distribution<int> dist(probabilities.begin(), probabilities.end());
        for(int i = 0; i < numToSelect; i++) {
            int selected = dist(generator);
            if(verbose) std::cout << "selected member: " << selected << "\n";
            population.select(dist(generator));
        }
    }

    template<typename T>
    void truncateSelection(Population<T>& population, int numToSelect) {
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
}
#endif