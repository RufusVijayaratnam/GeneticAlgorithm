#ifndef Objective_hpp
#define Objective_hpp

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
    protected:
        void incrementFitnessFunctionCallCount() {
            fitnessFunctionCallCount++;
        }
        virtual double fitnessFunction(PhenotypeBase<T>& phenotype) = 0;
        int fitnessFunctionCallCount = 0;
};
#endif