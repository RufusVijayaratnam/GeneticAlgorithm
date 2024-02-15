#ifndef Mutation_hpp
#define Mutation_hpp
class Population;
namespace Variation {
    extern double mutationRate;
    extern bool mutationRateSet;
    void setMutationRate(double mutationRate);
    void rotationToRight(Population& population);
}
#endif