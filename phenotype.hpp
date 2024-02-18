#ifndef phenotype_hpp
#define phenotype_hpp
#include <memory>
class Objective;
template <typename T> class Phenotype {
    public:
        /// @brief Constructor for phenotype object
        /// @param chromosome std::vector<T> real valued 
        /// @param objective 
        Phenotype<T>(std::vector<T> chromosome, std::unique_ptr<Objective> objective) : chromosome(chromosome)  {
            score = Objective.fitnessFunction(chromosome);
        }
        Phenotype<T>();
        static int objectiveFunctionCount;
        /// @brief Getter for the fitness value of phenotype
        /// @return double: fitness value of phenotype
        double getScore() const {return score;}
        /// @brief Getter for the chromosome of phenotype
        /// @return Const reference to phenotype chromosome
        const std::vector<T>& getChromosome() const {return chromosome;}
        /// @brief Getter for the size of phenotype chromosome
        /// @return int: size of phenotype chromosome
        int getChromosomeSize() const {return chromosome.size();}

        //Operator overloads
        virtual bool operator<(const Phenotype<T>& b) const = 0;
        virtual bool operator>(const Phenotype<T>& b) const = 0;
        virtual bool operator==(const Phenotype<T>& b) const = 0;

        /// @brief Prints the score of a chromosome, does not print any whitespace
        void printScore() const {std::cout << score;}

        /// @brief Prints the chromosome separated by commas inline
        /// @param setWidth set width of spacing between commas, i.e with std::setw(setWidth)
        void printChromosomeInline(int setWidth = 3) const {
            for(T c : chromosome) {
                std::cout << std::setw(setWidth) << c << ",";
            }
        }
    private:
        double score;
        std::vector<int> chromosome;
        void setChromosome(std::vector<T>& newChromosome, Objective& objective) {
            chromosome = newChromosome;
            score = objective.evaluate();
        }
};
#endif