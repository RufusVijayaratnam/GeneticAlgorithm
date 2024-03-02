#ifndef MUTATION_HPP
#define MUTATION_HPP
#include <random>

template<typename T>
class Population;

template<typename T>
class ObjectiveBase;

template<typename T>
class TerminationManager;

namespace Variation {
    template<typename T>
    void rotationToRight(Population<T>& population, TerminationManager<T>& terminationManager, double mutationRate=0.3) {
        if(mutationRate == 0) return;
        if(mutationRate < 0 || mutationRate > 1) {
            std::cerr << "rotationToRight\nMutation rate must be between [0,1], not " << mutationRate << "\n";
            exit(-1);
        }
        static bool seeded = false;
        if(!seeded) {
            time_t ct = time(NULL);
            srand(ct);
            seeded = true;
        }
        int permutationSize = population[0].getChromosomeSize();
        const std::vector<int> selected = population.getSelected();
        population.clearSelected();

        for(int sel : selected) {
            if(terminationManager.checkTermination()) return;
            double mutationProbability = double(rand()) / double(RAND_MAX);
            if(mutationRate < mutationProbability) continue;
            int i, j, k;
            i = rand() % permutationSize;
            j = rand() % permutationSize;
            k = rand() % (permutationSize + 1);
            if(i == j) continue;
            
            const std::vector<typename T::value_type>& permutation = population[sel].getChromosome();
            std::vector<typename T::value_type> newPermutation = std::vector<typename T::value_type>(permutationSize, -1);
            std::vector<typename T::value_type> partial;
            if(i <= j) {
                int partialSize = abs(j - i) + 1;
                if(k == partialSize) continue;
                partial = std::vector<typename T::value_type>(partialSize, -1);
                //Normal rotation
                for(int n = 0; n < i; n++) {
                    newPermutation[n] = permutation[n];
                }
                for(int n = i; n <= j; n++) {
                    partial[n - i] = permutation[n];
                }
                for(int n = j + 1; n < permutationSize; n++) {
                    newPermutation[n] = permutation[n];
                }
                std::rotate(partial.rbegin(), partial.rbegin() + (k % partialSize), partial.rend());
                for(int n = i; n <= j; n++) {
                    newPermutation[n] = partial[n - i];
                }
            } else {
                int partialSize = permutationSize - abs(j - i + 1);
                if(k == partialSize) continue;
                partial = std::vector<int>(partialSize, -1);
                //Rotating outer part of array
                for(int n = i; n <= j + permutationSize; n++) {
                    partial[n - i] = permutation[n % permutationSize];
                }
                //Copy inside part of array to new permutation
                for(int n = j + 1; n < i; n++) {
                    newPermutation[n] = permutation[n];
                }
                std::rotate(partial.rbegin(), partial.rbegin() + (k % partialSize), partial.rend());
                for(int n = i; n < i + partialSize; n++) {
                    newPermutation[n % permutationSize] = partial[n - i];
                }
            }
            const std::unique_ptr<ObjectiveBase<typename T::value_type>>& objective = population.getObjective();
            population.getPopulationMember(sel).setChromosome(newPermutation, objective);
        }
    }

    template<typename T>
    void twoOptSwap(Population<T>& population, TerminationManager<T>& terminationManager, double mutationRate=0.3) {
        if(mutationRate == 0) return;
        if(mutationRate < 0 || mutationRate > 1) {
            std::cerr << "rotationToRight\nMutation rate must be between [0,1], not " << mutationRate << "\n";
            exit(-1);
        }

        int chromosomeSize = population[0].getChromosomeSize();
        const std::vector<int> selected = population.getSelectedIndices();

        static std::random_device rd;
        static std::mt19937 mt(rd());
        static std::uniform_int_distribution<int> intDist(0, chromosomeSize - 1);
        static std::uniform_real_distribution<double> realDist(0.0, 1.0);


        for(int sel : selected) {
            if(terminationManager.checkTermination()) return;
            double mutationProbability = realDist(mt);
            if(mutationRate < mutationProbability) continue;

            const std::vector<typename T::value_type>& permutation = population[sel].getChromosome();
            std::vector<typename T::value_type> newPermutation = std::vector<typename T::value_type>(chromosomeSize);

            int v1, v2;
            do {
                v1 = intDist(mt);
                v2 = intDist(mt);
            } while(v1 == v2);
            if(v1 > v2) {
                int t = v1;
                v1 = v2;
                v2 = t;
            }

            //Fill first part of vector forwards
            for(int i = 0; i < v1; i++) {
                newPermutation[i] = permutation[i];
            }
            //Reverse [v1, v2]
            for(int i = v1; i <= v2; i++) {
                newPermutation[i] = permutation[v2 - (i - v1)];
            }
            //Fill in end
            for(int i = v2 + 1; i < chromosomeSize; i++) {
                newPermutation[i] = permutation[i];
            }
            
            const std::unique_ptr<ObjectiveBase<typename T::value_type>>& objective = population.getObjective();
            population.getPopulationMember(sel).setChromosome(newPermutation, objective);

        }


    }
}
#endif