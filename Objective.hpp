#ifndef OBJECTIVE_HPP
#define OBJECTIVE_HPP

template<typename T>
class PhenotypeBase;

template<typename T>
class ObjectiveBase {
    public:
        /// @brief Virtual destructor of abstract base ObjectiveBase class
        virtual ~ObjectiveBase() {}
        virtual double evaluate(PhenotypeBase<T>& phenotype) final {
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
        virtual double fitnessFunction(PhenotypeBase<T>& phenotype) = 0;
        int fitnessFunctionCallCount = 0;
};
#endif