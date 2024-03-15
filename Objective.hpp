#ifndef OBJECTIVE_HPP
#define OBJECTIVE_HPP

class PhenotypeBase;

class ObjectiveBase {
    public:
        /// @brief Virtual destructor of abstract base ObjectiveBase class
        virtual ~ObjectiveBase() {}
        virtual double evaluate(PhenotypeBase& phenotype) final {
            incrementFitnessFunctionCallCount();
            return fitnessFunction(phenotype);
        }
        int getCallCount() const {
            return fitnessFunctionCallCount;
        } 
    protected:
        void incrementFitnessFunctionCallCount() {
            fitnessFunctionCallCount++;
        }
        virtual double fitnessFunction(PhenotypeBase& phenotype) = 0;
        int fitnessFunctionCallCount = 0;
};
#endif