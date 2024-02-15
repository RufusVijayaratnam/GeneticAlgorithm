#include "Population.hpp"
#include "Reproduction.hpp"
#include <vector>
#include <algorithm>

void Reproduction::nElitism(Population& population, int n) {
    population.sort();
    population.resizePopulation(n);
}