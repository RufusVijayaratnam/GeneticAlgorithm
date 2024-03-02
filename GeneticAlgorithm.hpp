#ifndef GENETICALGORITHM_HPP
#define GENETICALGORITHM_HPP
#include <iostream>
#include <vector>
#include <memory>

template<typename T>
class ObjectiveBase;

template<typename T>
class Population;

template<typename T>
class TerminationManager;

template<typename T>
class TerminationFlagBase;

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
            T best = (*population).getPopulationMember(0);
            while(!terminationManager.checkTermination()) {
                geneticAlgorithm();
                population->sort();
                if(population->getPopulationMember(0) < best) {
                    best = population->getPopulationMember(0);
                }
                if(terminationManager.reportProgress()) {
                    std::cout << "Current best score: " << best.getScore() << "\n";
                }
            }
            std::cout << "Best solution score: " << best.getScore() << "\n";
            population->getPopulationMember(0).printChromosomeInline();
        }

        void addTerminationFlag(std::unique_ptr<TerminationFlagBase<T>> terminationFlag) {
            terminationManager.addTerminationFlag(std::move(terminationFlag));
        }

        /// @brief Get the population stored in the genetic algorithm.
        /// @return 
        std::unique_ptr<Population<T>> getPopulation() {
            return std::move(population);
        }

        void setProgressReportCount(int reportCount=-1) {this->reportCount = reportCount;}

    protected:
        virtual void geneticAlgorithm() = 0;
        std::unique_ptr<Population<T>> population;
        std::unique_ptr<ObjectiveBase<typename T::value_type>> objective;
        TerminationManager<T> terminationManager = TerminationManager<T>();
        int reportCount = -1;
    private:
        void setup() {
            terminationManager.setProgressReportCount(reportCount);
            if(terminationManager.size() < 1) {
                std::cerr << "At least one termination condition must be provided\nExiting program\n";
                exit(-1);
            }
            terminationManager.checkHasHardstopFlag();
            terminationManager.initialiseTerminationFlags(population, objective);
        }
};
#endif