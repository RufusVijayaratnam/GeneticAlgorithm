#ifndef phenotype_hpp
#define phenotype_hpp
#include <memory>
class TSP_Points;
class Phenotype {
    public:
        Phenotype(std::vector<int> permutation, std::shared_ptr<TSP_Points> pointsPtr);
        Phenotype();
        double getScore() const;
        std::shared_ptr<TSP_Points> getPointsPtr();
        std::vector<int> getPermutation();
        int getPermutationSize();
        bool operator<(const Phenotype& b) const;
        bool operator>(const Phenotype& b) const;
        void printScoreInline() const;
        void printScoreNewLine() const;
    private:
        int permutationSize;
        double score;
        std::vector<int> permutation;
        std::shared_ptr<TSP_Points> pointsPtr;
        double evaluateTSP(std::vector<int>& permutation);
};
#endif