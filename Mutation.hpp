#ifndef MUTATION_HPP
#define MUTATION_HPP

template<typename T>
class Population;

template<typename T>
class ObjectiveBase;

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
        for(int p = 0; p < population.size(); p++) {
            if(terminationManager.checkTermination()) return;
            double mutationProbability = double(rand()) / double(RAND_MAX);
            if(mutationProbability > mutationRate) continue;
            int i, j, k;
            i = rand() % permutationSize;
            j = rand() % permutationSize;
            k = rand() % (permutationSize + 1);
            if(i == j) continue;
            
            std::vector<typename T::value_type> permutation = population[p].getChromosome();
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
            population.getPopulationMember(p).setChromosome(newPermutation, objective);
        }
    }
}
#endif