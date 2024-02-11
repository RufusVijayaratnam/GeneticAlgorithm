#include <algorithm>
#include <random>
#include <iostream>
#include <time.h>
#include "phenotype.hpp"
#include "selectionFunctions.hpp"

void Selection::linearRankingSelection(std::vector<Phenotype>& solutions) {
    using namespace std;
    double beta = 1.5;
    double alpha = 2 - beta;
    int M = solutions.size();
    
    if(Selection::verbose) {
        cout << "Before selection: M = " << solutions.size() << "\n";
        for_each(solutions.begin(), solutions.end(), [](const Phenotype& a) -> void {a.printScoreInline();});
        cout << "\n";
    } 
    
    //Sort descending
    sort(solutions.begin(), solutions.end(), greater<Phenotype>());
    cout << "Managed to sort\n";
    auto acceptedEnd = remove_if(solutions.begin(), solutions.end(), [&solutions, &M, &alpha, &beta](const Phenotype& a) -> bool {
        return linearRankingShouldReject(M, &a - &*solutions.begin(), alpha, beta);
    });
    solutions.erase(acceptedEnd, solutions.end());

    if(Selection::verbose) {
        cout << "After selection: M = " << solutions.size() << "\n";
        for_each(solutions.begin(), solutions.end(), [](const Phenotype& a) -> void {a.printScoreInline();});
    } 
}

bool Selection::linearRankingShouldReject(int M, int gamma, double alpha, double beta) {
    static bool seeded = false;
    if(!seeded) {
        time_t ct = time(NULL);
        srand(ct);
        seeded = true;
    }
    //Random number between 0 and 1
    double R = double(rand()) / double(RAND_MAX);
    double P = double(alpha + double(beta - alpha) * double(gamma) / double(M - 1)) / double(M);
    if(verbose) {
        if(P < R) {
            std::cout << "Rejecting gamma = " << gamma << ", P < R : " << P << " < " << R << "\n";
        } else {
            std::cout << "Accepting gamma = " << gamma << ", P >= R : " << P << " >= " << R << "\n";
        }
    }
    return (P < R);
}

void Selection::truncateSelection(std::vector<Phenotype>& solutions, int n) {
    if(solutions.size() < n) return;
    sort(solutions.begin(), solutions.end(), std::greater<Phenotype>());
    solutions.resize(n);
}

void Selection::setVerbosity(bool verbosity) {
    verbose = verbosity;
}



bool Selection::verbose = false;