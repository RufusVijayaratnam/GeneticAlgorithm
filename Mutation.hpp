#ifndef Mutation_hpp
#define Mutation_hpp
template<typename T>
class Population;

namespace Variation {
    template<typename T>
    void Variation::rotationToRight(Population<T>& population) {
        if(!mutationRateSet) {
            std::cerr << "rotationToRight\nMutation rate must be set to use this function\n";
            exit(-1);
        }
        static bool seeded = false;
        if(!seeded) {
            time_t ct = time(NULL);
            srand(ct);
            seeded = true;
        }
        int permutationSize = population[0].getPermutationSize();
        for(int p = 0; p < population.size() && !shouldTerminateGA; p++) {
            double mutationProbability = double(rand()) / double(RAND_MAX);
            if(mutationProbability > mutationRate) continue;
            int i, j, k;
            i = rand() % permutationSize;
            j = rand() % permutationSize;
            k = rand() % (permutationSize + 1);
            if(i == j) continue;
            
            std::vector<int> permutation = population.getPopulationMember(p).getPermutation();
            std::vector<int> newPermutation = std::vector<int>(permutationSize, -1);
            std::vector<int> partial;
            if(i <= j) {
                int partialSize = abs(j - i) + 1;
                if(k == partialSize) continue;
                partial = std::vector<int>(partialSize, -1);
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
            population.getPopulationMember(p).setPermutation(newPermutation);
        }
    }
}
#endif