#ifndef GENETICALGORITHM_HPP
#define GENETICALGORITHM_HPP
#include <iostream>
#include <vector>
#include <memory>
#include "TerminationCondition.hpp"
#include "Population.hpp"
#include "Objective.hpp"
#include "phenotype.hpp"

class GeneticAlgorithm {
    public:
        ~GeneticAlgorithm() {}
        GeneticAlgorithm(std::shared_ptr<PhenotypeBase> emptyPhenotype, std::vector<std::unique_ptr<RepresentationBase>> representations, std::unique_ptr<ObjectiveBase> objective) 
        : objective(std::move(objective)) {
            this->population = std::make_unique<Population>(emptyPhenotype, std::move(representations), this->objective);
        }

        virtual void run() final {
            setup();
            std::shared_ptr<PhenotypeBase> best = population->getPopulationMember(0).deepCopy();
            while(!terminationManager.checkTermination()) {
                geneticAlgorithm();
                population->sort();
                if(population->getPopulationMember(0) < *best) {
                    best = population->getPopulationMember(0).deepCopy();
                }
                if(terminationManager.reportProgress()) {
                    std::cout << "Current best score: " << best->getScore() << "\n";
                }
            }
            std::cout << "Best solution score: " << best->getScore() << "\n";
            population->getPopulationMember(0).printRepresentation();
        }

        void addTerminationFlag(std::unique_ptr<TerminationFlagBase> terminationFlag) {
            terminationManager.addTerminationFlag(std::move(terminationFlag));
        }

        /// @brief Get the population stored in the genetic algorithm.
        /// @return 
        std::unique_ptr<Population> getPopulation() {
            return std::move(population);
        }

        void setProgressReportCount(int reportCount=-1) {this->reportCount = reportCount;}

    protected:
        virtual void geneticAlgorithm() = 0;
        std::unique_ptr<Population> population;
        std::unique_ptr<ObjectiveBase> objective;
        TerminationManager terminationManager = TerminationManager();
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