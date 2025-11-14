#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include "problem.h"

// Split the string <s> at every occurence of the delimiter <del>.
std::vector<std::string> split(std::string s, std::string del) {
    std::vector<std::string> v;
    int start = 0;
    for (int end = s.find(del, start); end != std::string::npos; end = s.find(del, start)) {
        v.push_back(s.substr(start, end - start));
        start = end + del.size();
    }
    v.push_back(s.substr(start));
    return v;
}   

std::string FORMAT = "^([\\s\\S]*)\\\\answer\\{([\\s\\S]*)\\}[\\s\\S]*\\\\topic\\{(.*)\\}[\\s\\S]*\\\\difficulty\\{(.*)\\}";
std::regex re(FORMAT);

std::string Problem::getQuestion() {return question;}
std::string Problem::getAnswer() {return answer;}
std::string Problem::getTopic() {return topic;}
int Problem::getDifficulty() {return difficulty;}


Problem::Problem(std::string rawProblem) {
    std::smatch match;
    if (!std::regex_search(rawProblem, match, re) == true) {
        std::cerr << "Invalid problem: " << rawProblem;
        throw std::runtime_error("Invalid problem");
    }
    question = match.str(1);
    answer = match.str(2);
    topic = match.str(3);
    difficulty = std::stoi(match.str(4));
}

std::vector<Problem> Problem::problemList(std::string filename) {
    // Read problems from file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening the problem list" << std::endl;
        throw std::runtime_error("Cannot open problem list");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string str = buffer.str();

    // Split into individual problems, throw out header at beginning
    std::vector<std::string> problemStrings = split(str, "\\item");
    problemStrings.erase(problemStrings.begin());

    // Convert strings to Problems
    std::vector<Problem> problems = {};
    for (std::string problemString : problemStrings) {
        Problem problem(problemString);
        problems.push_back(problem);
    }
    return problems;
}