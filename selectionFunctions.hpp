#ifndef selection_functions_hpp
#define selection_functions_hpp
#include <memory>
class Phenotype;
class TSP_Points;
class Population;
namespace Selection {
void linearRankingSelection(Population& population, int numToSelect);
void truncateSelection(Population& population, int numToSelect);
void setVerbosity(bool verbosity);
extern bool verbose;
}
#endif