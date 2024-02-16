#ifndef phenotype_hpp
#define phenotype_hpp
#include <memory>
class TSP_Points;
class Phenotype {
    public:
        Phenotype(std::vector<int> permutation, std::shared_ptr<TSP_Points> pointsPtr);
        Phenotype();
        static int objectiveFunctionCount;
        double getScore() const;
        std::shared_ptr<TSP_Points> getPointsPtr();
        std::vector<int> getPermutation() const;
        int getPermutationSize() const;
        void setPermutation(std::vector<int> newPermutation);
        bool operator<(const Phenotype& b) const;
        bool operator>(const Phenotype& b) const;
        bool operator==(const Phenotype& b) const;
        void printScoreInline() const;
        void printScoreNewLine() const;
        void printPermutationInline() const;
    private:
        int permutationSize;
        double score;
        std::vector<int> permutation;
        std::shared_ptr<TSP_Points> pointsPtr;
        double evaluateTSP(std::vector<int>& permutation);
};

template <>
struct std::hash<Phenotype> {
    std::size_t operator()(const Phenotype& a) const {
        std::vector<int> p = a.getPermutation();
        int zeroIdx = 0;
        while(p[zeroIdx] != 0) {
            zeroIdx++;
        }
    std::size_t seed = a.getPermutationSize();
    int size = a.getPermutationSize();
    for(int i = 0; i < size; i++, zeroIdx++) {
        zeroIdx %= size;
        p[zeroIdx] = ((p[zeroIdx] >> 16) ^ p[zeroIdx]) * 0x45d9f3b;
        p[zeroIdx] = ((p[zeroIdx] >> 16) ^ p[zeroIdx]) * 0x45d9f3b;
        p[zeroIdx] = (p[zeroIdx] >> 16) ^ p[zeroIdx];
        seed ^= p[zeroIdx] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
    }
};
#endif