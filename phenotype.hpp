#ifndef PHENOTYPE_HPP
#define PHENOTYPE_HPP
#include <memory>
#include <iostream>
#include <iomanip>

template<typename T>
class ObjectiveBase;

template<typename T>
class PhenotypeBase {
    public:
        /// @brief Used to allow correct typing in Population Class to support polymorphism
        using value_type = T;
        /// @brief Virtual destructor for abstract base PhenotypeBase class
        virtual ~PhenotypeBase() {}

        PhenotypeBase(std::vector<T> chromosome, const std::unique_ptr<ObjectiveBase<T>>& objective) : chromosome(chromosome) {
            if(objective) {
                score = objective->evaluate(*this);
            } else {
                std::cerr << "In PhenotypeBase(std::vector<T> chromosome, std::unique_ptr<ObjectiveBase<T>>& objective)\n";
                std::cerr << "Could not construct object because objective pointer is nullptr\n";
                std::cerr << "Check that the pointer passed to the constructor was not std::move()'d\nExiting program\n";
                exit(0);
            }
        }

        /// @brief Copy constructor
        /// @param b Const reference to object to copy
        PhenotypeBase(const PhenotypeBase& b) {
            this->score = b.score;
            this->chromosome = b.chromosome;
        }

        /// @brief A getter for the score of the phenotype when evaluated by
        /// fitness function
        /// @return double score
        virtual double getScore() const {return score;};

        /// @brief Getter for the size of phenotype chromosome
        /// @return int: size of phenotype chromosome
        virtual int getChromosomeSize() const {return chromosome.size();};

        /// @brief Getter for the chromosome of phenotype
        /// @return Const reference to phenotype chromosome
        const std::vector<T>& getChromosome() const {return chromosome;}

        //Operator overloads
        /// @brief Implementation of lesser operator for PhenotypeBase<T>
        /// @param b const reference to other operand
        /// @return bool return score < b.getScore()
        virtual const bool operator<(PhenotypeBase const &b) {return this->score < b.getScore();}

        /// @brief Implementation of greater operator for PhenotypeBase<T>
        /// @param b const reference to other operand
        /// @return bool return score > b.getScore()
        virtual const bool operator>(PhenotypeBase const &b) {return this->score > b.getScore();}

        /// @brief Pure virtual operator==, user must overload
        /// @param b const reference to other operand
        /// @return bool
        virtual const bool operator==(PhenotypeBase const &b) const = 0;

        /// @brief Prints the score of a chromosome, does not print any
        /// whitespace or newline
        virtual void printScore() {std::cout << score;};

        /// @brief Prints the chromosome separated by commas inline
        /// @param setWidth set width of spacing between commas, i.e with std::setw(setWidth)
        virtual void printChromosomeInline(int setWidth=3) const {
            for(T c : chromosome) {
                std::cout << std::setw(setWidth) << c << ",";
            }
            std::cout << "\n";
        }

        /// @brief Set the chromosome of the Phenotype to a new chromosome
        /// @param newChromosome New chromosome to set, must be same size as
        /// original chromosome
        /// @param objective Objective class which will be used to evaluate the phenotype
        void setChromosome(std::vector<T>& newChromosome, const std::unique_ptr<ObjectiveBase<T>>& objective) {
            if(newChromosome.size() != chromosome.size()) {
                std::cerr << "chromosome.size() must be equal to newChromosome.size()\nchromosome NOT updated\n";
                return;
            }
            chromosome = newChromosome;
            if(objective) {
                score = objective->evaluate(*this);
            } else {
                std::cerr << "In void setChromosome(std::vector<T>& newChromosome, std::unique_ptr<Objective<T>>& objective)\n";
                std::cerr << "Could not change chromosome because objective pointer is nullptr\n";
                std::cerr << "Check that the pointer passed to setChromosome() was not std::move()'d\nExiting program\n";
                exit(0);
            }
        }

        //Member variables
        protected:
            std::vector<T> chromosome;
            double score;
};
#endif