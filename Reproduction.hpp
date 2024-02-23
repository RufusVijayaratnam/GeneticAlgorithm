#ifndef Reproduction_hpp
#define Reproduction_hpp
template<typename T>
class Population;

namespace Reproduction {
    /// @brief 
    /// @param population Population object
    /// @param n number of population that survive
    template<typename T>
    void nElitism(Population<T>& population, int n, TerminationManager<T>& terminationManager) {
        if(terminationManager.checkTermination()) return;
        population.sort();
        population.resizePopulation(std::min(population.size(), n));
    }
}
#endif