#include "Question.h"
#include <iostream>
#include <cctype>

using namespace std;

// CONSTRUCTOR IMPLEMENTATION
// LOGIC: INITIALIZES ALL MEMBER VARIABLES, INCLUDING NEW CATEGORY AND DIFFICULTY STRINGS
Question::Question(const string& cat, const string& diff, const string& text, const vector<string>& opts, char answer)
    : category(cat), difficulty(diff), questionText(text), options(opts), correctAnswer(toupper(answer)) 
{
}

// DISPLAY QUESTION IMPLEMENTATION
// LOGIC: PRINTS CATEGORY AND DIFFICULTY BADGES, THEN THE QUESTION TEXT AND OPTIONS
void Question::displayQuestion() const 
{
    cout << "\n[" << category << "] | [" << difficulty << "]\n";
    cout << questionText << "\n";
    
    for (const string& option : options) 
        cout << option << "\n";
}

// CHECK ANSWER IMPLEMENTATION
bool Question::isCorrect(char userAnswer) const 
{
    return toupper(userAnswer) == correctAnswer;
}

// GETTER FOR CORRECT ANSWER
char Question::getCorrectAnswer() const 
{
    return correctAnswer;
}

// GETTER FOR CATEGORY
string Question::getCategory() const 
{
    return category;
}

// GETTER FOR DIFFICULTY
string Question::getDifficulty() const 
{
    return difficulty;
}