#ifndef selection_functions_hpp
#define selection_functions_hpp
#include <memory>
class Phenotype;
class TSP_Points;
namespace Selection {
void linearRankingSelection(std::vector<Phenotype>& solutions);
bool linearRankingShouldReject(int M, int gamma, double alpha, double beta);
void truncateSelection(std::vector<Phenotype>& solutions, int n);
void setVerbosity(bool verbosity);
extern bool verbose;
}
#endif