#ifndef GeneticAlgorithm_hpp
#define GeneticAlgorithm_hpp
#include <memory>
#include <ostream>
#include "TerminationCondition.hpp"
template<typename T>
class ObjectiveBase;

template<typename T>
class Population;

template<typename T>
class GeneticAlgorithm {
    public:
        ~GeneticAlgorithm() {}
        GeneticAlgorithm(std::vector<std::vector<typename T::value_type>>& chromosomes, std::unique_ptr<ObjectiveBase<typename T::value_type>> objective) 
        : objective(std::move(objective)) {
            this->population = std::unique_ptr<Population<T>>(new Population<T>(chromosomes, this->objective));
        }

        virtual void run() final {
            setup();
            while(!terminationManager.checkTermination()) {
                geneticAlgorithm();
            }
            std::cout << "Best solution score: " << (*population).getPopulationMember(0).getScore() << "\n";
            (*population).getPopulationMember(0).printChromosomeInline();
        }

        void addTerminationFlag(std::unique_ptr<TerminationFlagBase<T>> terminationFlag) {
            terminationManager.addTerminationFlag(std::move(terminationFlag));
        }

        /// @brief Get the population stored in the genetic algorithm.
        /// @return 
        std::unique_ptr<Population<T>> getPopulation() {
            return std::move(population);
        }

    protected:
        virtual void geneticAlgorithm() = 0;
        std::unique_ptr<Population<T>> population;
        std::unique_ptr<ObjectiveBase<typename T::value_type>> objective;
        TerminationManager<T> terminationManager = TerminationManager<T>();
    private:
        void setup() {
            if(terminationManager.size() < 1) {
                std::cerr << "At least one termination condition must be provided\nExiting program\n";
                exit(-1);
            }
            terminationManager.initialiseTerminationFlags(population, objective);

        }
};
#endif