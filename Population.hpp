#ifndef Population_hpp
#define Population_hpp
#include <vector>
#include <functional>
#include <algorithm>
#include <ostream>
#include <unordered_set>
#include "phenotype.hpp"
class Population {
    public:
        //Default constructor
        Population();
        Population(std::vector<Phenotype> population) : population(population) {}
        void setPopulation(std::vector<Phenotype> population) {
            this->population = population;
        }

        /// @brief Gets a reference to population member
        /// @param n Index of population member to get
        /// @return A reference to a population member (Phenotype&)
        Phenotype& getPopulationMember(int n) {
            return population[n];
        }

        int size() const {return this->population.size();}

        void setMinOrMax(bool isMinimisation) {
            this->isMinimisation = isMinimisation;
        }
        bool getMinOrMax() {
            return this->isMinimisation;
        }

        /// @brief Sorts the underlying population in the order best -> worst
        void sort() {
            if(isMinimisation) {
                this->sortAscending();
            } else {
                this->sortDescending();
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

        /// @brief Resizes the population vector to have n elements
        /// @param n Length of vector after resizing
        void resizePopulation(int n) {
            population.resize(n);
        }


        /// @brief Counts the number of unique population members, useful to
        /// check if getting stuck in local minima. Warning:
        /// computationally expensive
        /// @return Number of unique population members
        int countUnique() {
            std::unordered_set<Phenotype> s;
            for(auto member : population) {
                s.insert(member);
            }
            return s.size();
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