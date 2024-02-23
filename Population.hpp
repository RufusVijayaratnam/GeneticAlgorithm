#ifndef Population_hpp
#define Population_hpp
#include <vector>
#include <algorithm>
#include <ostream>
#include <unordered_set>
#include <memory>
#include <string>
#include <type_traits>
#include "CheckHashable.hpp"

#if defined(__GNUC__) || defined(__clang__)
    #include <cxxabi.h>
    #define HAS_CXXABI
#endif

template<typename T>
class PhenotypeBase;

template<typename T>
class ObjectiveBase;

template<typename T> 
class Population {
    public:
        Population(std::vector<std::shared_ptr<T>> population) : population(population) {}

        /// @brief Gets a const reference to population member
        /// @param n Index of population member to get
        /// @return A const reference to a population member (const T&)
        const T& operator[](int n) const {
            return *population[n];
        }

        /// @brief Gets a non-const reference to population member, better to
        /// use Population::operator[] in general unless absolutely necessary to
        /// modify population member
        /// @param n Index of population member to get
        /// @return A reference to a population member (T&)
        T& getPopulationMember(int n) {
            return *population[n];
        }

        /// @brief Returns the size of the population
        /// @return 
        int size() const {return population.size();}

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
        std::vector<int> getSelectedIndices() {
            return selected;
        }

        void addPopulationMember(std::shared_ptr<T>& member) {
            population.push_back(member);
        }

        void printScoresInline() {
            for(auto& m : population) {
                m->printScore();
                std::cout << ", ";
            }
            std::cout << "\n";
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

            if constexpr (!is_std_hashable_v<T>) {
                // If T is not hashable, print an error and return 0 or handle it as needed
                std::cerr << "Type " << typeName() << " does not have an implementation for std::hash. "
                        << "You must implement one to use Population::countUnique().\n";
                return 0;
            } else {
                // If T is hashable, proceed to count unique individuals
                std::unordered_set<T> uniqueSet;
                for (const auto& individual : population) {
                    uniqueSet.insert(*individual);
                }
                return uniqueSet.size();
            }
        }

        /// @brief Sets the objective function of the population, ideally this
        /// should only be called by GeneticAlgorithm::setup()
        /// @param objective Unique pointer to objective
        void setObjective(std::unique_ptr<ObjectiveBase<typename T::value_type>> objective) {
            static bool objectiveSet = false;
            if(objectiveSet) {
                std::cerr << "Warning: The objective function has already been set, now that it is changed, results may be affected\n";
            }
            objectiveSet = true;
            this->objective = std::move(objective);
            if(this->objective) {
                std::cout << "Objective successfully set\n";
            }
        }

        /// @brief Get a const reference to objective object
        /// @return const Objective<T>&
        const std::unique_ptr<ObjectiveBase<typename T::value_type>>& getObjective() const {
            return objective;
        }

    private:
        std::vector<std::shared_ptr<T>> population;
        std::vector<int> selected;
        std::unique_ptr<ObjectiveBase<typename T::value_type>> objective;

        /// @brief Sort population in ascending order by fitness value
        void sortAscending() {
            std::sort(population.begin(), population.end(), [](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
                return *a < *b;
            });
        }

        void sortDescending() {
            std::sort(population.begin(), population.end(), [](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
                return *a > *b;
            });
        }

        /// @brief Get typename of type
        /// @return std::string typename
        std::string typeName() {
            return demangle(typeid(T).name());
        }

        /// @brief Demangle typename on GCC / G++ / Clang otherwise return
        /// mangled name
        /// @param mangledName char array of typename to demangle
        /// @return demangled (cxxabi available) name or mangled name if not
        std::string demangle(const char* mangledName) {
            #ifdef HAS_CXXABI
            int status = -1;
            std::unique_ptr<char, void(*)(void*)> res {
                abi::__cxa_demangle(mangledName, NULL, NULL, &status),
                std::free
            };
            return (status == 0) ? res.get() : mangledName;
            #else
            // If the compiler does not support abi::__cxa_demangle, just return the mangled name
            return mangledName;
            #endif
        }

};
#endif