#ifndef Reproduction_hpp
#define Reproduction_hpp
class Population;
namespace Reproduction {
    /// @brief 
    /// @param population Population object
    /// @param n number of population that survive
    void nElitism(Population& population, int n);
}
#endif