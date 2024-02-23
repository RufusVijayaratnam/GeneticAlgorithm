#ifndef crossover_functions_hpp
#define crossover_functions_hpp

template<typename T>
class PhenotypeBase;

template<typename T>
class ObjectiveBase;

template<typename T>
class Population;

namespace Variation {
    template<typename T>
    void simpleCrossover(Population<T>& population, double crossoverRate=0.8, bool verbose=false) {
        static bool seeded = false;
        std::default_random_engine randomEngine;
        if(!seeded) {
            time_t currentTime = time(NULL);
            srand(currentTime);
            randomEngine.seed(currentTime);
            seeded = true;
        }
        int chromosomeSize = population[0].getChromosomeSize();
        std::vector<int> selected = population.getSelectedIndices();
        int m = selected.size();


        //Shuffle to get random pairs
        shuffle(selected.begin(), selected.end(), randomEngine);
        for(int i = 0; i < m; i += 2) {
            double r = double(rand()) / RAND_MAX;
            if(r > crossoverRate) continue;
            std::vector<typename T::value_type> child1Permutation = std::vector<typename T::value_type>(chromosomeSize);
            std::vector<typename T::value_type> child2Permutation = std::vector<typename T::value_type>(chromosomeSize);
            //Generate random number in range 1 - chromosomeSize - 2 inclusive
            //First and last elements of vector must remain unchanged, i.e always
            //start and end at the same city
            int k = rand() % (chromosomeSize - 2) + 1;
            int j;
            //Loops below will correctly set the first and last elements
            std::vector<typename T::value_type> parent1Permutation = population[i].getChromosome();
            std::vector<typename T::value_type> parent2Permutation = population[i + 1].getChromosome();
            for(j = 0; j <= k; j++) {
                child1Permutation[j] = parent1Permutation[j];
                child2Permutation[j] = parent2Permutation[j];
            }
            for(; j < chromosomeSize; j++) {
                child1Permutation[j] = parent2Permutation[j];
                child2Permutation[j] = parent1Permutation[j];
            }

            const std::unique_ptr<ObjectiveBase<typename T::value_type>>& objective = population.getObjective();
            std::shared_ptr<T> child1 = std::shared_ptr<T>(new T(child1Permutation, objective));
            std::shared_ptr<T> child2 = std::shared_ptr<T>(new T(child2Permutation, objective));

            //Append new phenotypes to solutions
            population.addPopulationMember(child1);
            population.addPopulationMember(child2);
        }

    }
    
    template<typename T>
    void orderedCrossover(Population<T>& population, double crossoverRate=0.8, bool verbose=false) {
        std::vector<int> selected = population.getSelectedIndices();
        int numSelected = selected.size();
        if(numSelected < 2) {
            std::cerr << "Variation::orderedCrossover\nCan't have n < 2 for ordered crossover\n";
        }

        int chromosomeSize = population[0].getChromosomeSize();
        
        static bool seeded = false;
        if(!seeded) {
            time_t ct = time(NULL);
            srand(ct);
            seeded = true;
        }

        for(int p = 0; p < numSelected - 1; p += 2) {
            double r = double(rand()) / RAND_MAX;
            if(r > crossoverRate) continue;
            //Generate random number between 1 and n - 2 inclusive
            int a = rand() % chromosomeSize;
            int b = rand() % (chromosomeSize - a) + a;
            int j1, j2, k;
            j1 = b + 1; j2 = b + 1; k = b + 1;
            int p1Idx = selected[p];
            int p2Idx = selected[p + 1];
            const std::vector<typename T::value_type>& parent1 = population[p1Idx].getChromosome();
            std::vector<typename T::value_type> child1 = std::vector<typename T::value_type>(chromosomeSize, 0);
            const std::vector<typename T::value_type>& parent2 = population[p2Idx].getChromosome();
            std::vector<typename T::value_type> child2 = std::vector<typename T::value_type>(chromosomeSize, 0);
            std::unordered_set<typename T::value_type> parent1MidRange;
            std::unordered_set<typename T::value_type> parent2MidRange;
    
            for(int m = a; m <= b; m++) {
                parent1MidRange.insert(parent1[m]);
                child1[m] = parent1[m];
                parent2MidRange.insert(parent2[m]);
                child2[m] = parent2[m];
            }

            for(int i = 0; i < chromosomeSize; i++) {
                if(parent1MidRange.find(parent2[k % chromosomeSize]) == parent1MidRange.end()) {
                    child1[j1 % chromosomeSize] = parent2[k % chromosomeSize];
                    j1++;
                }
                if(parent2MidRange.find(parent1[k % chromosomeSize]) == parent2MidRange.end()) {
                    child2[j2 % chromosomeSize] = parent1[k % chromosomeSize];
                    j2++;
                }
                k++;
            }

            const std::unique_ptr<ObjectiveBase<typename T::value_type>>& objective = population.getObjective();
            std::shared_ptr<T> child1Pheno = std::shared_ptr<T>(new T(child1, objective));
            std::shared_ptr<T> child2Pheno = std::shared_ptr<T>(new T(child2, objective));
            population.addPopulationMember(child1Pheno);
            population.addPopulationMember(child2Pheno);

            if(verbose) {
                std::cout << "a = " << a << ", b = " << b << "\n";

                std::cout << std::setw(10) << "indicies:";
                for(int i = 0; i < chromosomeSize; i++) {
                    std::cout << std::setw(4) << i << ",";
                }
                std::cout << "\n";
                std::cout << std::setw(10) << "parent1:";
                population[p].printChromosomeInline();
                std::cout << std::setw(10) << "parent2:";
                population[p + 1].printChromosomeInline();
                std::cout << std::setw(10) << "midrange1:";
                for(int i = 0; i < chromosomeSize; i++) {
                    if(i >= a && i <= b) {
                        std::cout << std::setw(4) << parent1[i] << ",";
                    } else {
                        std::cout << std::setw(4) << "--" << ",";
                    }
                }
                std::cout << "\n";
                std::cout << std::setw(10) << "midrange2:";
                for(int i = 0; i < chromosomeSize; i++) {
                    if(i >= a && i <= b) {
                        std::cout << std::setw(4) << parent2[i] << ",";
                    } else {
                        std::cout << std::setw(4) << "--" << ",";
                    }
                }
                std::cout << "\n";
                std::cout << std::setw(10) << "child1:";
                child1Pheno->printChromosomeInline();
                std::cout << std::setw(10) << "child2:";
                child2Pheno->printChromosomeInline();
            }

        }
    }
}
#endif