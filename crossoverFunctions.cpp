#include <vector>
#include <time.h>
#include <random>
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <iomanip>
#include "phenotype.hpp"
#include "crossoverFunctions.hpp"
#include "Population.hpp"

void Variation::simpleCrossover(std::vector<Phenotype>& solutions) {
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

void Variation::orderedCrossover(Population& population) {
    std::vector<int> selected = population.getSelected();
    int numSelected = selected.size();
    if(numSelected < 2) {
        std::cerr << "Variation::orderedCrossover\nCan't have n < 2 for ordered crossover\n";
    }

    int permutationSize = population.getPopulationMember(0).getPermutationSize();
    
    static bool seeded = false;
    if(!seeded) {
        time_t ct = time(NULL);
        srand(ct);
        seeded = true;
    }

    for(int p = 0; p < numSelected - 1; p += 2) {
        //Generate random number between 1 and n - 2 inclusive
        int a = rand() % permutationSize;
        int b = rand() % (permutationSize - a) + a;
        int j1, j2, k;
        j1 = b + 1; j2 = b + 1; k = b + 1;

        std::vector<int> parent1 = population.getPopulationMember(p).getPermutation();
        std::vector<int> child1 = std::vector<int>(permutationSize, 0);
        std::vector<int> parent2 = population.getPopulationMember(p + 1).getPermutation();
        std::vector<int> child2 = std::vector<int>(permutationSize, 0);
        std::unordered_set<int> parent1MidRange;
        std::unordered_set<int> parent2MidRange;
   
        for(int m = a; m <= b; m++) {
            parent1MidRange.insert(parent1[m]);
            child1[m] = parent1[m];
            parent2MidRange.insert(parent2[m]);
            child2[m] = parent2[m];
        }

        for(int i = 0; i < permutationSize; i++) {
            if(parent1MidRange.find(parent2[k % permutationSize]) == parent1MidRange.end()) {
                child1[j1 % permutationSize] = parent2[k % permutationSize];
                j1++;
            }
            if(parent2MidRange.find(parent1[k % permutationSize]) == parent2MidRange.end()) {
                child2[j2 % permutationSize] = parent1[k % permutationSize];
                j2++;
            }
            k++;
        }
        Phenotype child1Pheno = Phenotype(child1, population.getPopulationMember(0).getPointsPtr());
        Phenotype child2Pheno = Phenotype(child2, population.getPopulationMember(0).getPointsPtr());
        population.addPopulationMember(child1Pheno);
        population.addPopulationMember(child2Pheno);


        if(verbose) {
            std::cout << "a = " << a << ", b = " << b << "\n";

            std::cout << std::setw(10) << "indicies:";
            for(int i = 0; i < permutationSize; i++) {
                std::cout << std::setw(4) << i << ",";
            }
            std::cout << "\n";
            std::cout << std::setw(10) << "parent1:";
            population.getPopulationMember(p).printPermutationInline();
            std::cout << std::setw(10) << "parent2:";
            population.getPopulationMember(p + 1).printPermutationInline();
            std::cout << std::setw(10) << "midrange1:";
            for(int i = 0; i < permutationSize; i++) {
                if(i >= a && i <= b) {
                    std::cout << std::setw(4) << parent1[i] << ",";
                } else {
                    std::cout << std::setw(4) << "--" << ",";
                }
            }
            std::cout << "\n";
            std::cout << std::setw(10) << "midrange2:";
            for(int i = 0; i < permutationSize; i++) {
                if(i >= a && i <= b) {
                    std::cout << std::setw(4) << parent2[i] << ",";
                } else {
                    std::cout << std::setw(4) << "--" << ",";
                }
            }
            std::cout << "\n";
            std::cout << std::setw(10) << "child1:";
            child1Pheno.printPermutationInline();
            std::cout << std::setw(10) << "child2:";
            child2Pheno.printPermutationInline();
        }

    }
}

bool Variation::verbose = false;
void Variation::setVerbosity(bool verbosity) {
    verbose = verbosity;
}