#ifndef Population_hpp
#define Population_hpp
#include <vector>
#include <functional>
#include <algorithm>
#include <ostream>
#include "phenotype.hpp"
class Population {
    public:
        //Default constructor
        Population();
        Population(std::vector<Phenotype> population) : population(population) {}
        void setPopulation(std::vector<Phenotype> population) {
            this->population = population;
        }

        Phenotype getPopulationMember(int n) {
            return population[n];
        }

        int size() const {return this->population.size();}

        void setMinOrMax(bool isMinimisation) {
            this->isMinimisation = isMinimisation;
        }
        bool getMinOrMax() {
            return this->isMinimisation;
        }

        /// @brief Sorts the underlying population in the order worst -> best
        void sort() {
            if(isMinimisation) {
                this->sortDescending();
            } else {
                this->sortAscending();
            }
        }

        /// @brief Attempt to preallocate enough memory in selected vector for n elements
        /// @param n Number or elements required
        void reserveSelected(int n) {
            selected.reserve(n);
        }
        
        /// @brief Add index n to vector of selected candidates
        /// @param n Index of population element to select
        void select(int n) {
            selected.push_back(n);
        }

        /// @brief Clear the selected vector
        void clearSelected() {
            selected.clear();
        }

        /// @brief Get copy of selected vector
        /// @return vector<int> of selected population members
        std::vector<int> getSelected() {
            return selected;
        }

        void addPopulationMember(Phenotype member) {
            population.push_back(member);
        }

        void printScoresInline() {
            for(auto m : population) {
                m.printScoreInline();
            }
        }

    private:
        std::vector<Phenotype> population;
        std::vector<int> selected;
        bool isMinimisation = true;

        /// @brief Sort population in ascending order by fitness value
        void sortAscending() {
            std::sort(population.begin(), population.end(), std::less<Phenotype>());
        }

        void sortDescending() {
            std::sort(population.begin(), population.end(), std::greater<Phenotype>());
        }
        

};
#endif