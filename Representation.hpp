#ifndef REPRESENTATION_HPP
#define REPRESENTATION_HPP
#include <memory>
#include <vector>
#include <string>
class RepresentationBase {
    public:
        virtual ~RepresentationBase() = default;
        virtual std::string toString() const {return "toString() not implemented";}
        virtual int size() const {return 0;}
        //COPY - must be implemented
        virtual std::unique_ptr<RepresentationBase> emptyCopy() const = 0;
        virtual std::unique_ptr<RepresentationBase> deepCopy() const = 0;
        //GET - do not need to be implemented but must exist to support common
        //mutation / selection / crossover
        virtual std::vector<int> getIntegerVectorRepresentation() const {return std::vector<int>{};}
        virtual std::vector<double> getDoubleVectorRepresentation() const {return std::vector<double>{};}
        virtual void setIntegerVectorRepresentation(std::vector<int>& rep) {return;}
        virtual void setDoubleVectorRepresentation() {return;}
};
std::ostream& operator<<(std::ostream& os, const RepresentationBase& rep) {
    os << rep.toString();
    return os;
}
#endif