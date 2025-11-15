#include <ranges>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <map>
#include <set>
#include "problem.h"

class Header {
private:
    std::string texFile;      
    std::string contentFile;  
    std::map<std::string, std::string> commands; 

public:
    Header(const std::string& tex, const std::string& content): texFile(tex), contentFile(content) {}

    void addCommand(const std::string& key, const std::string& val) {
        commands[key] = val;
    }

    void write(std::ofstream& out) const {
        out << "\\input{" << texFile << "}\n";
    for (const auto& pair : commands) {
        out << "\\newcommand{\\" << pair.first << "}{" << pair.second << "}\n";
    }
    out << "\\input{" << contentFile << "}\n";
};

class FancyLayout : public ProblemLayout {
public:
    //Pretty much copied from fancy_test_generator and modified 
    std::string format(const Problem& p, int number) const override {
        std::string out;

        if (number % 2 == 1) {       // Start a new page before 
            out +=  "\\pagebreak\n\n"; // each odd-numbered problem
        } else {                                 // Insert blank space before
            out +=  "\\vspace{350pt}\n\n"; // each even-numbered problem
        }
        out += "\\item\\begin{tabular}[t]{p{5in} p{.3in} p{.8in}}\n";
        out +=  p.getQuestion();
        out +=  "& & \\arabic{enumi}.\\hrulefill\n\\end{tabular}\n";

        return out;
    };
};

// ****************************************************************************
// Configuration details

// Variable information to be printed on the test
std::string CLASS = "Arithmetic";
std::string TERM = "Fall 2025";
std::string EXAM = "925";
std::string TIME = "Day";
std::string TITLE = "Final Exam";
std::string FORM = "A";

// Source file for problem bank
std::string BANK = "arithmetic_problems.tex";

// Filename for the created test
std::string FILENAME = "fancy_test.tex";

// Constraints on the problem choice.
int NUM_PROBLEMS = 20; // The test must have 20 problems.
int MIN_TOPIC = 3; // Each topic must be covered 
int MAX_TOPIC = 7; // by 3-7 problems.
int MIN_DIFFICULTY = 65; // Total difficulty (using the difficulty defined 
int MAX_DIFFICULTY = 75; // in the problem bank) must be 65-75.

// tex files to include in the test file
std::string TEX_HEADER = "fancy_tex_header.tex";
std::string CONTENT_HEADER = "fancy_content_header.tex";

// ****************************************************************************

// Check whether a proposed test is valid according to the above constraints.
bool valid(std::vector<Problem> test, std::set<std::string> topics) {
    // Initialize metrics
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

    // Open the file to write the test to
    std::ofstream outputFile(FILENAME); 
    if (!outputFile.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        return 1;
    }

    // Generate the test problems
    std::vector<Problem> test = testProblems(bank);


    // Uses the header class to add commands and write the header 
    Header header(TEX_HEADER, CONTENT_HEADER);
    header.addCommand("class", CLASS);        
    header.addCommand("term", TERM);          
    header.addCommand("examno", EXAM);        
    header.addCommand("dayeve", TIME);        
    header.addCommand("formletter", FORM);    
    header.addCommand("numproblems", std::to_string(NUM_PROBLEMS)); 
    header.addCommand("testtitle", TITLE);    
    header.write(outputFile);
    // // Write the tex header to the file
    // outputFile << "\\input{" << TEX_HEADER << "}\n";

    // // Include the manually-entered information
    // outputFile << "\\newcommand{\\class}{" << CLASS << "}\n";
    // outputFile << "\\newcommand{\\term}{" << TERM << "}\n";
    // outputFile << "\\newcommand{\\examno}{" << EXAM << "}\n";
    // outputFile << "\\newcommand{\\dayeve}{" << TIME << "}\n";
    // outputFile << "\\newcommand{\\formletter}{" << FORM << "}\n";
    // outputFile << "\\newcommand{\\numproblems}{" << NUM_PROBLEMS << " }\n";
    // outputFile << "\\newcommand{\\testtitle}{" << TITLE << "}\n";

    // // Write the content header to the file
    // outputFile << "\\input{" << CONTENT_HEADER << "}\n";



    int problem_number = 1;
    FancyLayout layout;
    for (Problem problem : test) {
        // if (problem_number % 2 == 1) {       // Start a new page before 
        //     outputFile << "\\pagebreak\n\n"; // each odd-numbered problem
        // } else {                                 // Insert blank space before
        //     outputFile << "\\vspace{350pt}\n\n"; // each even-numbered problem
        // }
        // outputFile << "\\item\\begin{tabular}[t]{p{5in} p{.3in} p{.8in}}\n";
        // outputFile << problem.getQuestion();
        // outputFile << "& & \\arabic{enumi}.\\hrulefill\n\\end{tabular}\n";
        // problem_number += 1;
        outputFile << layout.format(problem, problem_number);
        problem_number += 1;
    }

    // End the file
    outputFile << "\\end{enumerate}\n\\end{document}";
    outputFile.close();
}