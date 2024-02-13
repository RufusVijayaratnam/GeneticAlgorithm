#ifndef crossover_functions_hpp
#define crossover_functions_hpp
class Phenotype;
class TSP_Points;
class Population;
namespace Variation {
    void simpleCrossover(std::vector<Phenotype>& solutions);
    void orderedCrossover(Population& population);
    void setVerbosity(bool verbosity);
    extern bool verbose;
}
#endif