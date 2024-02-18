#ifndef Population_hpp
#define Population_hpp
#include <vector>
#include <algorithm>
#include <ostream>
#include <unordered_set>
#include <memory>
#include <string>
#include "CheckHashable.hpp"
class Phenotype;
class Objective;
template<typename T> class Population {
    public:
        Population(std::vector<Phenotype<T>> population, std::unique_ptr<Objective> objective) : population(population), objective(objective) {}
        void setPopulation(std::vector<Phenotype<T>> population) {
            this->population = population;
        }

        /// @brief Gets a const reference to population member
        /// @param n Index of population member to get
        /// @return A reference to a population member (Phenotype<T>&)
        const Phenotype<T>& operator[](int n) const {
            return population[n];
        }

        /// @brief Returns the size of the population
        /// @return 
        int size() const {return this->population.size();}

        /// @brief Sorts the underlying population in the order best -> worst
        void sort() {
            sortAscending();
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

        void addPopulationMember(Phenotype<T> member) {
            population.push_back(member);
        }

        void printScoresInline() {
            for(auto m : population) {
                m.printScore();
                std::cout << ", ";
            }
        }

        /// @brief Resizes the population vector to have n elements, destroys
        /// all elements at indices >= n
        /// @param n Length of vector after resizing
        void resizePopulation(int n) {
            population.resize(n);
        }


        /// @brief Counts the number of unique population members, useful to
        /// check if getting stuck in local minima. Warning:
        /// computationally expensive
        /// @return Number of unique population members
        int countUnique() {
            bool hashable;
            static int checkedHashable = false;
            if(!checkedHashable) {
                hashable = is_std_hashable_v<Phenotype<T>>;
                checkedHashable = true;
            }
            if(!hashable) {
                template <typename T> std::string type_name();
                if(population.size() != 0) {
                    std::string typeName = type_name<decltype(population[0])>();
                    std::cerr << typeName << " does not have an implementation for std::hash, you must implement one to use Population::countUnique(), returning 0\n";
                    return 0;
                } else {
                    return 0;
                }
            }
            std::unordered_set<Phenotype<T>> s;
            for(auto member : population) {
                s.insert(member);
            }
            return s.size();
        }
    private:
        std::vector<Phenotype<T>> population;
        std::vector<int> selected;
        std::unique_ptr<Objective> objective;

        /// @brief Sort population in ascending order by fitness value
        void sortAscending() {
            std::sort(population.begin(), population.end(), std::less<Phenotype<T>>());
        }

        void sortDescending() {
            std::sort(population.begin(), population.end(), std::greater<Phenotype<T>>());
        }
};
#endif