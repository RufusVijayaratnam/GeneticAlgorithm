#ifndef TerminationCondition_hpp
#define TerminationCondition_hpp
#include <chrono>
#include <memory>

template<typename T>
class ObjectiveBase;

template<typename T>
class Population;

template<typename T>
class TerminationFlagBase {
    public:
        virtual bool checkTermination() = 0;
        virtual void setPopulation(const std::unique_ptr<Population<T>>& population) {return;}
        virtual void setObjective(const std::unique_ptr<ObjectiveBase<typename T::value_type>>& objective) {return;}
};

template<typename T>
class TerminationManager {
    private:
        std::vector<std::unique_ptr<TerminationFlagBase<T>>> terminationFlags;
        bool terminated = false;
    public:
        TerminationManager() {}
        void addTerminationFlag(std::unique_ptr<TerminationFlagBase<T>> terminationFlag) {
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

        void initialiseTerminationFlags(const std::unique_ptr<Population<T>>& population, const std::unique_ptr<ObjectiveBase<typename T::value_type>>& objective) {
            for(auto& flag : terminationFlags) {
                flag->setPopulation(population);
                flag->setObjective(objective);
            }
        }

        int size() const {
            return terminationFlags.size();
        }
};

template<typename T>
class TimeTerminationFlag : public TerminationFlagBase<T> {
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
                static bool alreadyPrinted = false;
                if(!alreadyPrinted) std::cout << "TimeTerminationFlag terminating: time = " << std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count() << " seconds\n";
                alreadyPrinted = true;
                return true;
            }
            return false;
        }
};

template<typename T>
class IterationTerminationFlag : public TerminationFlagBase<T> {
    private:
        int iterationLimit;
        int iterationCount = 0;
    
    public:
        virtual bool checkTermination() override {
            iterationCount++;
            if(iterationCount >= iterationLimit) {
                static bool alreadyPrinted = false;
                if(!alreadyPrinted) std::cout << "IterationTerminationFlag terminating: iteration = " << iterationCount << "\n";
                alreadyPrinted = true;
                return true;
            }
            return false;
        }
};

template<typename T>
class FitnessFunctionCallTerminationFlag : public TerminationFlagBase<T> {
    private:
        std::unique_ptr<ObjectiveBase<T>>& objective;
        int callCountLimit;

    public:
        FitnessFunctionCallTerminationFlag(int callCountLimit) : callCountLimit(callCountLimit) {}
        
        virtual bool checkTermination() override {
            if(objective->getCallCount() >= callCountLimit) {
                static bool alreadyPrinted = false;
                if(alreadyPrinted) std::cout << "FitnessFunctionCallTerminationFlag terminating: objective function call count = " << objective->getCallCount() << "\n";
                alreadyPrinted = true;
                return true;
            }
            return false;
        }

        virtual void setObjective(const std::unique_ptr<ObjectiveBase<typename T::value_type>>& objective) override {this->objective = objective;}
};

template<typename T>
class MinimumPopulationTerminationFlag : public TerminationFlagBase<T> {
    private:
        std::unique_ptr<Population<T>>& population;
        int minimumPopulation;
        
    public:
    MinimumPopulationTerminationFlag(int minimumPopulation) : minimumPopulation(minimumPopulation) {}
    
    virtual bool checkTermination() override {
        if(population->size() < minimumPopulation) {
            static bool alreadyPrinted = false;
            if(!alreadyPrinted) std::cout << "MinimumPopulationTerminationFlag terminating: population size = " << population->size() << "\n";
            alreadyPrinted = true;
        }
    }

    virtual void setPopulation(const std::unique_ptr<Population<T>>& population) override {this->population = population;}
};

template<typename T>
class MaximumPopulationTerminationFlag : public TerminationFlagBase<T> {
    private:
        std::unique_ptr<Population<T>>& population;
        int minimumPopulation;

    public:
    MaximumPopulationTerminationFlag(int minimumPopulation) : minimumPopulation(minimumPopulation) {}
    
    virtual bool checkTermination() override {
        if(population->size() > minimumPopulation) {
            static bool alreadyPrinted = false;
            if(!alreadyPrinted) std::cout << "MaximumPopulationTerminationFlag terminating: population size = " << population->size() << "\n";
            alreadyPrinted = true;
        }
    }

    virtual void setPopulation(const std::unique_ptr<Population<T>>& population) override {this->population = population;}
};

template<typename T>
class MinimumUniquePopulationTerminationFlag : public TerminationFlagBase<T> {
    private:
        std::unique_ptr<Population<T>>& population;
        int minimumUnique;
    public:

        MinimumUniquePopulationTerminationFlag(int minimumUnique) : minimumUnique(minimumUnique) {}

        virtual bool checkTermination() override {
            int uniqueCount = population->countUnique();
            if(uniqueCount < minimumUnique) {
                static bool alreadyPrinted = false;
                if(!alreadyPrinted) std::cout << "MinimumUniquePopulationTerminationFlag terminating: unique population count = " << uniqueCount << "\n";
                alreadyPrinted = true;
            }
        }

    virtual void setPopulation(const std::unique_ptr<Population<T>>& population) override {this->population = population;}
};

#endif
