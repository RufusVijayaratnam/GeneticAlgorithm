#ifndef PHENOTYPE_HPP
#define PHENOTYPE_HPP
#include <memory>
#include <iostream>
#include <iomanip>
#include "Representation.hpp"
#include "Objective.hpp"

class PhenotypeBase {
    public:
        /// @brief Virtual destructor for abstract base PhenotypeBase class
        virtual ~PhenotypeBase() {}

        PhenotypeBase() {};

        PhenotypeBase(std::unique_ptr<RepresentationBase> representation, const std::unique_ptr<ObjectiveBase>& objective) : representation(std::move(representation)) {
            this->tempObj = objective.get();
            if(objective) {
                score = objective->evaluate(*this);
            } else {
                std::cerr << "In PhenotypeBase(std::unique_ptr<RepresentationBase> representation, std::unique_ptr<ObjectiveBase>& objective)\n";
                std::cerr << "Could not construct object because objective pointer is nullptr\n";
                std::cerr << "Check that the pointer passed to the constructor was not std::move()'d\nExiting program\n";
                exit(0);
            }
        }

        /// @brief Copy constructor
        /// @param b Const reference to object to copy
        PhenotypeBase(const PhenotypeBase& b) {
            this->score = b.score;
            this->representation = b.representation->deepCopy();
        }

        virtual std::shared_ptr<PhenotypeBase> emptyCopy() const = 0;

        virtual std::shared_ptr<PhenotypeBase> deepCopy() const = 0;

        /// @brief A getter for the score of the phenotype when evaluated by
        /// fitness function
        /// @return double score
        virtual double getScore() const {return score;};

        /// @brief Getter for the size of phenotype representation
        /// @return int: size of phenotype representation
        virtual int getRepresentationSize() const {return representation->size();};

        /// @brief Getter for the representation of phenotype
        /// @return Const reference to phenotype representation
        const RepresentationBase& getRepresentation() const {return *representation;}

        //Operator overloads
        /// @brief Implementation of lesser operator for PhenotypeBase<T>
        /// @param b const reference to other operand
        /// @return bool return score < b.getScore()
        virtual const bool operator<(PhenotypeBase const &b) const {return this->score < b.getScore();}

        /// @brief Implementation of greater operator for PhenotypeBase
        /// @param b const reference to other operand
        /// @return bool return score > b.getScore()
        virtual const bool operator>(PhenotypeBase const &b) const {return this->score > b.getScore();}

        /// @brief Pure virtual operator==, user must overload
        /// @param b const reference to other operand
        /// @return bool
        virtual const bool operator==(PhenotypeBase const &b) const = 0;

        /// @brief Prints the score of a representation, does not print any
        /// whitespace or newline
        virtual void printScore() const {std::cout << score;};

        /// @brief Prints the representation separated by commas inline
        virtual void printRepresentation() const {
            std::cout << *representation << "\n";
        }

        virtual void setScore(double s) final {this->score = s;}
        virtual void setRepresentation_NOEVALUATE(std::unique_ptr<RepresentationBase> rep) {
            this->representation = std::move(rep);
        }

        /// @brief Set the representation of the Phenotype to a new representation
        /// @param newRepresentation New representation to set, must be same size as
        /// original representation
        /// @param objective Objective class which will be used to evaluate the phenotype
        void setRepresentation(std::unique_ptr<RepresentationBase> newRepresentation, const std::unique_ptr<ObjectiveBase>& objective) {
            representation = std::move(newRepresentation);
            if(objective) {
                score = objective->evaluate(*this);
            } else {
                std::cerr << "In void setRepresentation(std::unique_ptr<RepresentationBase> newRepresentation, std::unique_ptr<Objective<T>>& objective)\n";
                std::cerr << "Could not change representation because objective pointer is nullptr\n";
                std::cerr << "Check that the pointer passed to setRepresentation() was not std::move()'d\nExiting program\n";
                exit(0);
            }
        }

        //Member variables
        protected:
            std::unique_ptr<RepresentationBase> representation;
            ObjectiveBase* tempObj;
            double score;
};
#endif