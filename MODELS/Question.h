#pragma once
#include <string>
#include <vector>

using namespace std;

// CLASS: QUESTION
// PURPOSE: REPRESENTS A SINGLE MULTIPLE-CHOICE QUESTION
//          STORES CATEGORY, DIFFICULTY, TEXT, OPTIONS, AND CORRECT ANSWER.
class Question 
{
private:
    string category;
    string difficulty;
    string questionText;
    vector<string> options;
    char correctAnswer;

public:
    // PURPOSE: CONSTRUCTOR TO INITIALIZE A QUESTION OBJECT
    // INPUT: CATEGORY, DIFFICULTY, QUESTION TEXT, VECTOR OF OPTIONS, CORRECT ANSWER CHAR
    // OUTPUT: NONE
    // LOGIC: ASSIGNS PARAMETERS AND FORCES ANSWER TO UPPERCASE
    Question(const string& cat, const string& diff, const string& text, const vector<string>& opts, char answer);

    // PURPOSE: DISPLAYS THE QUESTION, CATEGORY, DIFFICULTY, AND OPTIONS
    // INPUT: NONE
    // OUTPUT: PRINTS TEXT TO STANDARD OUTPUT
    // LOGIC: ITERATES THROUGH THE OPTIONS VECTOR AND PRINTS EACH ONE
    void displayQuestion() const;

    // PURPOSE: EVALUATES IF THE USER'S GUESS MATCHES THE CORRECT ANSWER
    // INPUT: A CHARACTER REPRESENTING THE USER'S CHOICE
    // OUTPUT: BOOLEAN (TRUE IF CORRECT, FALSE IF INCORRECT)
    // LOGIC: CONVERTS THE INPUT TO UPPERCASE AND COMPARES IT
    bool isCorrect(char userAnswer) const;

    // PURPOSE: RETRIEVES THE CORRECT ANSWER
    // INPUT: NONE
    // OUTPUT: CHARACTER OF THE CORRECT ANSWER
    char getCorrectAnswer() const;

    // PURPOSE: RETRIEVES THE CATEGORY OF THE QUESTION
    // INPUT: NONE
    // OUTPUT: STRING REPRESENTING THE CATEGORY
    string getCategory() const;

    // PURPOSE: RETRIEVES THE DIFFICULTY OF THE QUESTION
    // INPUT: NONE
    // OUTPUT: STRING REPRESENTING THE DIFFICULTY
    string getDifficulty() const;
};