#include "Question.h"
#include <iostream>
#include <cctype>

using namespace std;

// CONSTRUCTOR IMPLEMENTATION
// INITIALIZES THE MEMBER VARIABLES WITH THE PROVIDED ARGUMENTS
// CONVERTS THE CORRECT ANSWER TO UPPERCASE IMMEDIATELY TO ENSURE INTERNAL CONSISTENCY
Question::Question(const string& text, const vector<string>& opts, char answer)
    : questionText(text), options(opts), correctAnswer(toupper(answer)) {
}

// DISPLAY QUESTION IMPLEMENTATION
// LOGIC: PRINTS THE QUESTION TEXT, THEN LOOPS THROUGH THE VECTOR TO PRINT OPTIONS A, B, C, AND D
void Question::displayQuestion() const 
{
    cout << "\n" << questionText << "\n";
    
    // ITERATE THROUGH THE OPTIONS AND PRINT THEM
    for (const string& option : options) 
        cout << option << "\n";
}

// CHECK ANSWER IMPLEMENTATION
// LOGIC: CONVERTS PASSED USER INPUT TO UPPERCASE USING TOUPPER BEFORE COMPARING
bool Question::isCorrect(char userAnswer) const 
{
    return toupper(userAnswer) == correctAnswer;
}

// GETTER IMPLEMENTATION
// SIMPLY RETURNS THE STORED CORRECT ANSWER
char Question::getCorrectAnswer() const 
{
    return correctAnswer;
}