#include "Population.hpp"
#include "Reproduction.hpp"
#include <vector>
#include <algorithm>
#include "GeneticAlgorithm.hpp"

void Reproduction::nElitism(Population& population, int n) {
    population.sort();
    population.resizePopulation(n);
}