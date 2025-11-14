#include <ranges>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <map>
#include <set>
#include "problem.h"




// ****************************************************************************
// Configuration details

// Title to be printed at the beginning of the test
std::string TITLE = "Arithmetic Test";

// Source file for problem bank
std::string BANK = "arithmetic_problems.tex";

// Filename for the created test
std::string FILENAME = "simple_test.tex";

// Constraints on the problem choice.
int NUM_PROBLEMS = 20; // The test must have 20 problems.
int MIN_TOPIC = 3; // Each topic must be covered 
int MAX_TOPIC = 7; // by 3-7 problems.
int MIN_DIFFICULTY = 65; // Total difficulty (using the difficulty defined 
int MAX_DIFFICULTY = 75; // in the problem bank) must be 65-75.

// tex files to include in the test file
std::string TEX_HEADER = "simple_tex_header.tex";
std::string CONTENT_HEADER = "simple_content_header.tex";

// ****************************************************************************

// Check whether a proposed test is valid according to the above constraints.
bool valid(std::vector<Problem> test, std::set<std::string> topics) {
    // Initialize metricsx
    int difficulty = 0;
    std::map<std::string, int> topicCounts;
    for (std::string topic : topics) {
        topicCounts[topic] = 0;
    }

    // Calculate the metrics
    for (Problem p : test) {
        difficulty += p.getDifficulty();
        topicCounts[p.getTopic()] += 1;
    }

    // Check the metrics
    if (difficulty < MIN_DIFFICULTY || difficulty > MAX_DIFFICULTY) {
        return false;
    }
    for (std::string topic : topics) {
        int count = topicCounts[topic];
        if (count < MIN_TOPIC || count > MAX_TOPIC) {
            return false;
        }
    }
    return true;
}

// Given a bank of possible test problems, return randomly-chosen 
// problems that form a valid test, according to the contraints above.
std::vector<Problem> testProblems(std::vector<Problem> bank) {
    // Determine the topics covered on the test
    std::set<std::string> topics;
    for (Problem p : bank) {
        topics.insert(p.getTopic());
    }

    // Used for random generation
    std::random_device rd;
    std::mt19937 gen(rd());

    while (true) {
        std::shuffle(bank.begin(), bank.end(), gen);
        std::vector<Problem> testProblems(bank.begin(), bank.begin() + NUM_PROBLEMS);
        if (valid(testProblems, topics)) {
            return testProblems;
        }
    }
}

int main() {
    // Read in problem list and convert to Problem objects
    std::vector<Problem> bank = Problem::problemList(BANK);

    // Generate the test problems
    std::vector<Problem> test = testProblems(bank);

    // Open the file to write the test to
    std::ofstream outputFile(FILENAME); 
    if (!outputFile.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        return 1;
    }

    // Write the header to the file
    outputFile << "\\input{" << TEX_HEADER << "}\n";
    outputFile << "\\newcommand{\\testtitle}{" << TITLE << "}\n";
    outputFile << "\\input{" << CONTENT_HEADER << "}\n";

    // Write the problems to the file
    for (Problem problem : test) {
        outputFile << "\\item " << problem.getQuestion() << "\n";
    }

    // End the file
    outputFile << "\\end{enumerate}\n\\end{document}";
    outputFile.close();
}