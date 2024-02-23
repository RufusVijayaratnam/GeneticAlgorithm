#ifndef GeneticAlgorithm_hpp
#define GeneticAlgorithm_hpp
#include <memory>
template<typename T>
class ObjectiveBase;

template<typename T>
class Population;

template<typename T>
class GeneticAlgorithm {
    public:
        ~GeneticAlgorithm() {}
        GeneticAlgorithm(std::unique_ptr<Population<T>> population, std::unique_ptr<ObjectiveBase<typename T::value_type>> objective) 
        : population(std::move(population)), objective(std::move(objective)) {}

        virtual void run() final {
            setup();
            geneticAlgorithm();
        }
    protected:
        virtual void geneticAlgorithm() = 0;
        std::unique_ptr<Population<T>> population;
        std::unique_ptr<ObjectiveBase<typename T::value_type>> objective;
    private:
        void setup() {
            population->setObjective(std::move(objective));
        }
};
#endif