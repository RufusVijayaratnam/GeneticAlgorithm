#ifndef TERMINATIONCONDITION_HPP
#define TERMINATIONCONDITION_HPP
#include <chrono>
#include <memory>
#include "Objective.hpp"
#include "Population.hpp"

class TerminationFlagBase {
    public:
        bool alreadyPrinted = false;
        virtual ~TerminationFlagBase() = default;
        virtual bool checkTermination() = 0;
        virtual void setPopulation(const std::unique_ptr<Population>& population) {return;}
        virtual void setObjective(const std::unique_ptr<ObjectiveBase>& objective) {return;}
        virtual bool isHardstopFlag() const {return false;}
        virtual double checkProgress() const {return -1;}
        virtual void reportProgress() const {return;}
};

class TimeTerminationFlag : public TerminationFlagBase {
    private:
        std::chrono::steady_clock::time_point startTime;
        std::chrono::seconds timeLimit;

    public:
        TimeTerminationFlag(int limit) : timeLimit(limit) {
            startTime = std::chrono::steady_clock::now();
        }

        virtual bool checkTermination() override {
            auto now = std::chrono::steady_clock::now();
            if(std::chrono::duration_cast<std::chrono::seconds>(now - startTime) >= timeLimit) {

                if(!this->alreadyPrinted) std::cout << "TimeTerminationFlag terminating: time = " << std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count() << " seconds\n";
                this->alreadyPrinted = true;
                return true;
            }
            return false;
        }

        virtual bool isHardstopFlag() const override {return true;}

        virtual double checkProgress() const override {
            auto now = std::chrono::steady_clock::now();
            return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count()) / static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(timeLimit).count());
        }

        virtual void reportProgress() const override {
            auto now = std::chrono::steady_clock::now();
            std::cout << "Time: " << static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count()) / 1000 << " : " << std::chrono::duration_cast<std::chrono::seconds>(timeLimit).count() << " s\n";
        }
};

class IterationTerminationFlag : public TerminationFlagBase {
    private:
        int iterationLimit;
        int iterationCount = 0;
    
    public:
        virtual bool checkTermination() override {
            iterationCount++;
            if(iterationCount >= iterationLimit) {

                if(!this->alreadyPrinted) std::cout << "IterationTerminationFlag terminating: iteration = " << iterationCount << "\n";
                this->alreadyPrinted = true;
                return true;
            }
            return false;
        }

        virtual bool isHardstopFlag() const override {return true;}

        virtual double checkProgress() const override {return static_cast<double>(iterationCount) / static_cast<double>(iterationLimit);}

        virtual void reportProgress() const override {std::cout << "iteration: " << iterationCount << " : " << iterationLimit << "\n";}
};

class FitnessFunctionCallTerminationFlag : public TerminationFlagBase {
    private:
        ObjectiveBase* objective;
        int callCountLimit;

    public:
        FitnessFunctionCallTerminationFlag(int callCountLimit) : callCountLimit(callCountLimit) {}
        
        virtual bool checkTermination() override {
            if(!objective) {
                std::cerr << "Objective pointer not initialised, the population must be maintained for lifetime of GeneticAlgorithm\nExiting Program\n";
                exit(-1);
            }
            if(objective->getCallCount() >= callCountLimit) {

                if(!this->alreadyPrinted) std::cout << "FitnessFunctionCallTerminationFlag terminating: objective function call count = " << objective->getCallCount() << "\n";
                this->alreadyPrinted = true;
                return true;
            }
            return false;
        }

        virtual void setObjective(const std::unique_ptr<ObjectiveBase>& objective) override {this->objective = objective.get();}

        virtual bool isHardstopFlag() const override {return true;}

        virtual double checkProgress() const override {return double(objective->getCallCount()) / callCountLimit;}

        virtual void reportProgress() const override {std::cout << "fitness function calls: " << objective->getCallCount() << " : " << callCountLimit << "\n";}
};

class MinimumPopulationTerminationFlag : public TerminationFlagBase {
    private:
        Population* population;
        int minimumPopulation;
        
    public:
    MinimumPopulationTerminationFlag(int minimumPopulation) : minimumPopulation(minimumPopulation) {}
    
    virtual bool checkTermination() override {
        if(!population) {
                std::cerr << "Population pointer not initialised, the population must be maintained for lifetime of GeneticAlgorithm\nExiting Program\n";
                exit(-1);
        }
        if(population->size() < minimumPopulation) {
            if(!this->alreadyPrinted) std::cout << "MinimumPopulationTerminationFlag terminating: population size = " << population->size() << "\n";
            this->alreadyPrinted = true;
        }
        return false;
    }

    virtual void setPopulation(const std::unique_ptr<Population>& population) override {this->population = population.get();}
};

class MaximumPopulationTerminationFlag : public TerminationFlagBase {
    private:
        Population* population;
        int minimumPopulation;

    public:
    MaximumPopulationTerminationFlag(int minimumPopulation) : minimumPopulation(minimumPopulation) {}
    
    virtual bool checkTermination() override {
        if(!population) {
                std::cerr << "Population pointer not initialised, the population must be maintained for lifetime of GeneticAlgorithm\nExiting Program\n";
                exit(-1);
        }
        if(population->size() > minimumPopulation) {
            if(!this->alreadyPrinted) std::cout << "MaximumPopulationTerminationFlag terminating: population size = " << population->size() << "\n";
            this->alreadyPrinted = true;
        }
        return false;
    }

    virtual void setPopulation(const std::unique_ptr<Population>& population) override {this->population = population.get();}
};

class MinimumUniquePopulationTerminationFlag : public TerminationFlagBase {
    private:
        Population* population;
        int minimumUnique;
    public:

        MinimumUniquePopulationTerminationFlag(int minimumUnique) : minimumUnique(minimumUnique) {}

        virtual bool checkTermination() override {
            if(!population) {
                std::cerr << "Population pointer not initialised, the population must be maintained for lifetime of GeneticAlgorithm\nExiting Program\n";
                exit(-1);
            }
            int uniqueCount = population->countUnique();
            if(uniqueCount < minimumUnique) {

                if(!this->alreadyPrinted) std::cout << "MinimumUniquePopulationTerminationFlag terminating: unique population count = " << uniqueCount << "\n";
                this->alreadyPrinted = true;
            }
            return false;
        }

    virtual void setPopulation(const std::unique_ptr<Population>& population) override {this->population = population.get();}
};


class TerminationManager {
    private:
        std::vector<std::unique_ptr<TerminationFlagBase>> terminationFlags;
        bool terminated = false;
        int lastReportIndex = -1;
        int numberOfReports;
    public:
        TerminationManager() {}
        void addTerminationFlag(std::unique_ptr<TerminationFlagBase> terminationFlag) {
            terminationFlags.push_back(std::move(terminationFlag));
        }

        bool checkTermination() {
            if(terminated) return true;
            for(const auto& flag : terminationFlags) {
                if(flag->checkTermination()) {
                    terminated = true;
                    return true;
                }
            }
            return false;
        }

        void initialiseTerminationFlags(const std::unique_ptr<Population>& population, const std::unique_ptr<ObjectiveBase>& objective) {
            for(auto& flag : terminationFlags) {
                flag->setPopulation(population);
                flag->setObjective(objective);
            }
        }

        void checkHasHardstopFlag() {
            for(auto& flag : terminationFlags) {
                /* if(dynamic_cast<TimeTerminationFlag>(flag)) return true;
                if(dynamic_cast<FitnessFunctionCallTerminationFlag>(flag)) return true;
                if(dyanmic_cast<IterationTerminationFlag>(flag)) return true;
                */
                if(flag->isHardstopFlag()) return;
            }
            std::cerr 
                << "At least one termination condition must be a hard stop kind:\n"
                << "IterationTerminationFlag\n"
                << "TimeTerminationFlag\n"
                << "FitnessFunctionCallTerminationFlag\n"
                << "Or define your own, make sure to override 'virtual bool TerminationManager::isHardstopFlag()'\n"
                << "Exiting program\n";
            exit(-1);
        }

        int size() const {
            return terminationFlags.size();
        }

        bool reportProgress() {
            if(numberOfReports == -1) return false;
            double maxProgress = 0.0;
            TerminationFlagBase* maxFlag = nullptr;
            for (const auto& flag : terminationFlags) {
                if(!flag->isHardstopFlag()) continue;
                double progress = flag->checkProgress();
                if (progress > maxProgress) {
                    maxProgress = progress;
                    maxFlag = flag.get();
                }
            }

            int currentReportIndex = static_cast<int>(maxProgress * numberOfReports);
            bool isReporting = false;
            if (currentReportIndex > lastReportIndex) {
                std::cout << "Progress: " << maxProgress * 100 << "%\n";
                maxFlag->reportProgress();
                lastReportIndex = currentReportIndex;
                isReporting = true;
            }
            return isReporting;
        }

        void setProgressReportCount(int numberOfReports) {
            this->numberOfReports = numberOfReports;
        }
};
#endif
