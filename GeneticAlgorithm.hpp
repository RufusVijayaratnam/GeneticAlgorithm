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
        GeneticAlgorithm(std::vector<std::vector<typename T::value_type>>& chromosomes, std::unique_ptr<ObjectiveBase<typename T::value_type>> objective) 
        : objective(std::move(objective)) {
            this->population = std::unique_ptr<Population<T>>(new Population<T>(chromosomes, this->objective));
        }

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
            
        }
};
#endif