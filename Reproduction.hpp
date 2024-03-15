#ifndef REPRODUCTION_HPP
#define REPRODUCTION_HPP
#include "Population.hpp"
#include "TerminationCondition.hpp"

namespace Reproduction {
    /// @brief 
    /// @param population Population object
    /// @param n number of population that survive
    void nElitism(Population& population, int n, TerminationManager& terminationManager) {
        if(terminationManager.checkTermination()) return;
        population.sort();
        population.resizePopulation(std::min(population.size(), n));
    }
}
#endif