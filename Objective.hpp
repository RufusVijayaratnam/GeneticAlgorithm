#ifndef Objective_hpp
#define Objective_hpp
class Phenotype;
class Objective {
    private:
        int fitnessFunctionCallCount;
    protected:
        virtual double fitnessFunction(Phenotype& phenotype) = 0;
    public:
        virtual ~Objective() {}
        void incrementFitnessFunctionCallCount() {
            fitnessFunctionCallCount++;
        }
        virtual double evaluate(Phenotype& phenotype) final {
            incrementFitnessFunctionCallCount();
            return fitnessFunction(phenotype);
        }
};
#endif