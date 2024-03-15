#ifndef CROSSOVER_HPP
#define CROSSOVER_HPP

class PhenotypeBase;

class ObjectiveBase;

class Population;

class TerminationManager;

namespace Variation {
    void simpleCrossover(Population& population, TerminationManager& terminationManager, double crossoverRate=0.8, bool verbose=false) {
        static bool seeded = false;
        std::default_random_engine randomEngine;
        if(!seeded) {
            time_t currentTime = time(NULL);
            srand(currentTime);
            randomEngine.seed(currentTime);
            seeded = true;
        }
        int representationSize = population[0].getRepresentationSize();
        std::vector<int> selected = population.getSelectedIndices();
        //Clear selected to use in mutation
        population.clearSelected();
        int m = selected.size();


        //Shuffle to get random pairs
        shuffle(selected.begin(), selected.end(), randomEngine);
        for(int i = 0; i < m; i += 2) {
            if(terminationManager.checkTermination()) return;
            double r = double(rand()) / RAND_MAX;
            if(r > crossoverRate) continue;
            std::vector<int> child1Permutation = std::vector<int>(representationSize);
            std::vector<int> child2Permutation = std::vector<int>(representationSize);
            //Generate random number in range 1 - representationSize - 2 inclusive
            //First and last elements of vector must remain unchanged, i.e always
            //start and end at the same city
            int k = rand() % (representationSize - 2) + 1;
            int j;
            //Loops below will correctly set the first and last elements
            const RepresentationBase& parent1Representation = population[i].getRepresentation();
            const RepresentationBase& parent2Representation = population[i + 1].getRepresentation();
            std::vector<int> parent1Permutation = parent1Representation.getIntegerVectorRepresentation();
            std::vector<int> parent2Permutation = parent2Representation.getIntegerVectorRepresentation();
            for(j = 0; j <= k; j++) {
                child1Permutation[j] = parent1Permutation[j];
                child2Permutation[j] = parent2Permutation[j];
            }
            for(; j < representationSize; j++) {
                child1Permutation[j] = parent2Permutation[j];
                child2Permutation[j] = parent1Permutation[j];
            }

            const std::unique_ptr<ObjectiveBase>& objective = population.getObjective();
            std::unique_ptr<RepresentationBase> child1Representation = parent1Representation.emptyCopy();
            std::unique_ptr<RepresentationBase> child2Representation = parent2Representation.emptyCopy();
            child1Representation->setIntegerVectorRepresentation(child1Permutation);            
            child2Representation->setIntegerVectorRepresentation(child2Permutation);            
            std::shared_ptr<PhenotypeBase> child1 = population[i].emptyCopy();
            std::shared_ptr<PhenotypeBase> child2 = population[i + 1].emptyCopy();
            child1->setRepresentation(std::move(child1Representation), objective);
            child2->setRepresentation(std::move(child2Representation), objective);

            //Append new phenotypes to solutions
            population.addPopulationMember(child1);
            population.select(population.size() - 1);
            population.addPopulationMember(child2);
            population.select(population.size() - 1);
        }

    }
    
    void orderedCrossover(Population& population, TerminationManager& terminationManager, double crossoverRate=0.8, bool verbose=false) {
        std::vector<int> selected = population.getSelectedIndices();
        population.clearSelected();
        if(crossoverRate < 0.000001) return;
        int numSelected = selected.size();
        if(numSelected < 2) {
            std::cerr << "Variation::orderedCrossover\nCan't have n < 2 for ordered crossover\nExiting Program\n";
            exit(-1);
        }

        int representationSize = population[0].getRepresentationSize();
        
        static std::random_device rd;
        static std::mt19937 mt(rd());
        static std::uniform_int_distribution<int> intDist(0, representationSize - 1);
        static std::uniform_real_distribution<double> realDist(0.0, 1.0);

        for(int p = 0; p < numSelected - 1; p += 2) {
            if(terminationManager.checkTermination()) return;
            double crossoverProbability = realDist(mt);
            if(crossoverRate < crossoverProbability) continue;
            //Generate random number between 1 and n - 2 inclusive
            int a = intDist(mt);
            int b = intDist(mt) % (representationSize - a) + a;
            int j1, j2, k;
            j1 = b + 1; j2 = b + 1; k = b + 1;
            int p1Idx = selected[p];
            int p2Idx = selected[p + 1];

            const RepresentationBase& parent1Representation = population[p1Idx].getRepresentation();
            const RepresentationBase& parent2Representation = population[p2Idx].getRepresentation();
            std::vector<int> parent1Permutation = parent1Representation.getIntegerVectorRepresentation();
            std::vector<int> parent2Permutation = parent2Representation.getIntegerVectorRepresentation();

            std::vector<int> child1Permutation = std::vector<int>(representationSize, 0);
            std::vector<int> child2Permutation = std::vector<int>(representationSize, 0);
            std::unordered_set<int> parent1MidRange;
            std::unordered_set<int> parent2MidRange;
    
            for(int m = a; m <= b; m++) {
                parent1MidRange.insert(parent1Permutation[m]);
                child1Permutation[m] = parent1Permutation[m];
                parent2MidRange.insert(parent2Permutation[m]);
                child2Permutation[m] = parent2Permutation[m];
            }

            for(int i = 0; i < representationSize; i++) {
                if(parent1MidRange.find(parent2Permutation[k % representationSize]) == parent1MidRange.end()) {
                    child1Permutation[j1 % representationSize] = parent2Permutation[k % representationSize];
                    j1++;
                }
                if(parent2MidRange.find(parent1Permutation[k % representationSize]) == parent2MidRange.end()) {
                    child2Permutation[j2 % representationSize] = parent1Permutation[k % representationSize];
                    j2++;
                }
                k++;
            }

            const std::unique_ptr<ObjectiveBase>& objective = population.getObjective();
            std::unique_ptr<RepresentationBase> child1Representation = parent1Representation.emptyCopy();
            std::unique_ptr<RepresentationBase> child2Representation = parent2Representation.emptyCopy();
            child1Representation->setIntegerVectorRepresentation(child1Permutation);            
            child2Representation->setIntegerVectorRepresentation(child2Permutation);            
            std::shared_ptr<PhenotypeBase> child1 = population[p1Idx].emptyCopy();
            std::shared_ptr<PhenotypeBase> child2 = population[p2Idx].emptyCopy();
            child1->setRepresentation(std::move(child1Representation), objective);
            child2->setRepresentation(std::move(child2Representation), objective);

            population.addPopulationMember(child1);
            population.select(population.size() - 1);
            population.addPopulationMember(child2);
            population.select(population.size() - 1);

            if(verbose) {
                std::cout << "a = " << a << ", b = " << b << "\n";

                std::cout << std::setw(10) << "indicies:";
                for(int i = 0; i < representationSize; i++) {
                    std::cout << std::setw(4) << i << ",";
                }
                std::cout << "\n";
                std::cout << std::setw(10) << "parent1Permutation:";
                population[p].printRepresentation();
                std::cout << std::setw(10) << "parent2Permutation:";
                population[p + 1].printRepresentation();
                std::cout << std::setw(10) << "midrange1:";
                for(int i = 0; i < representationSize; i++) {
                    if(i >= a && i <= b) {
                        std::cout << std::setw(4) << parent1Permutation[i] << ",";
                    } else {
                        std::cout << std::setw(4) << "--" << ",";
                    }
                }
                std::cout << "\n";
                std::cout << std::setw(10) << "midrange2:";
                for(int i = 0; i < representationSize; i++) {
                    if(i >= a && i <= b) {
                        std::cout << std::setw(4) << parent2Permutation[i] << ",";
                    } else {
                        std::cout << std::setw(4) << "--" << ",";
                    }
                }
                std::cout << "\n";
                std::cout << std::setw(10) << "child1:";
                child1->printRepresentation();
                std::cout << std::setw(10) << "child2:";
                child2->printRepresentation();
            }

        }
    }
}
#endif