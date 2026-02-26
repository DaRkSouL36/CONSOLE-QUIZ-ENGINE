#pragma once
#include <string>
#include <vector>

using namespace std;

// CLASS: QUESTION
// PURPOSE: REPRESENTS A SINGLE MULTIPLE-CHOICE QUESTION
//          STORES THE TEXT, FOUR OPTIONS, AND THE CORRECT ANSWER.
class Question 
{
    private:
        string questionText;
        vector<string> options;
        char correctAnswer;

    public:
        // PURPOSE: CONSTRUCTOR TO INITIALIZE A QUESTION OBJECT
        // INPUT: THE QUESTION TEXT, A VECTOR OF FOUR OPTIONS, AND THE CORRECT ANSWER CHARACTER (A/B/C/D)
        // OUTPUT: NONE
        // LOGIC: ASSIGNS PARAMETERS TO MEMBER VARIABLES AND FORCES THE ANSWER TO UPPERCASE
        Question(const string& text, const vector<string>& opts, char answer);

        // PURPOSE: DISPLAYS THE QUESTION AND ITS OPTIONS TO THE USER
        // INPUT: NONE
        // OUTPUT: PRINTS TEXT TO STANDARD OUTPUT
        // LOGIC: ITERATES THROUGH THE OPTIONS VECTOR AND PRINTS EACH ONE
        void displayQuestion() const;

        // PURPOSE: EVALUATES IF THE USER'S GUESS MATCHES THE CORRECT ANSWER
        // INPUT: A CHARACTER REPRESENTING THE USER'S CHOICE
        // OUTPUT: BOOLEAN (TRUE IF CORRECT, FALSE IF INCORRECT)
        // LOGIC: CONVERTS THE INPUT TO UPPERCASE AND COMPARES IT TO THE CORRECT ANSWER
        bool isCorrect(char userAnswer) const;

        // PURPOSE: RETRIEVES THE CORRECT ANSWER
        // INPUT: NONE
        // OUTPUT: CHARACTER OF THE CORRECT ANSWER
        // LOGIC: RETURNS THE PRIVATE MEMBER VARIABLE
        char getCorrectAnswer() const;
};