#include <vector>
#include <time.h>
#include <random>
#include <algorithm>
#include "phenotype.hpp"
#include "crossoverFunctions.hpp"

void simpleCrossover(std::vector<Phenotype>& solutions) {
    using namespace std;
    static bool seeded = false;
    default_random_engine randomEngine;
    if(!seeded) {
        time_t currentTime = time(NULL);
        srand(currentTime);
        randomEngine.seed(currentTime);
        seeded = true;
    }
    int permutationSize = solutions[0].getPermutationSize();
    shared_ptr<TSP_Points> pointsPtr = solutions[0].getPointsPtr();
    int m = solutions.size();

    //Shuffle to get random pairs
    shuffle(solutions.begin(), solutions.end(), randomEngine);
    for(int i = 0; i < m; i += 2) {
        vector<int> child1Permutation = vector<int>(permutationSize);
        vector<int> child2Permutation = vector<int>(permutationSize);
        //Generate random number in range 1 - permutationSize - 2 inclusive
        //First and last elements of vector must remain unchanged, i.e always
        //start and end at the same city
        int r = rand() % (permutationSize - 2) + 1;
        int j;
        //Loops below will correctly set the first and last elements
        vector<int> parent1Permutation = solutions[i].getPermutation();
        vector<int> parent2Permutation = solutions[i + 1].getPermutation();
        for(j = 0; j <= r; j++) {
            child1Permutation[j] = parent1Permutation[j];
            child2Permutation[j] = parent2Permutation[j];
        }
        for(; j < permutationSize; j++) {
            child1Permutation[j] = parent2Permutation[j];
            child2Permutation[j] = parent1Permutation[j];
        }

        //Construct new phenotypes
        Phenotype child1 = Phenotype(child1Permutation, pointsPtr);
        Phenotype child2 = Phenotype(child2Permutation, pointsPtr);

        //Append new phenotypes to solutions
        solutions.push_back(child1);
        solutions.push_back(child2);
    }

}