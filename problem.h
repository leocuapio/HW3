#ifndef PROBLEM_H
#define PROBLEM_H

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ranges>
#include <algorithm>
#include <fstream>

class Problem;

class IProblem {
public:
    virtual ~IProblem() = default;
    virtual std::string getQuestion() const= 0;
    virtual std::string getAnswer() const= 0;
protected:
    std::string question;
    std::string answer;
};

class Problem: public IProblem {
public:
    std::string getQuestion() const override;
    std::string getAnswer() const override;
    std::string getTopic() const;
    int getDifficulty() const;
    Problem(const std::string rawProblem);
    static std::vector<Problem> problemList(std::string filename);
    
private:
    // std::string question;
    // std::string answer;
    std::string topic;
    int difficulty;
};

class Problemv2: public IProblem {
public:
    Problemv2(const std::string rawProblem);
    // Any new info would go here 
};

class ProblemLayout {
public:
    virtual ~ProblemLayout() = default;
    virtual std::string format(const Problem& p, int number) const = 0;
};

class AlgorithmSelect {
public:
    virtual ~AlgorithmSelect() = default;
    virtual std::vector<Problem> select(const std::vector<Problem>& bank) const = 0;
};

#endif