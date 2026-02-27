#include "Question.h"
#include <iostream>
#include <cctype>
#include <algorithm>
#include <random>

using namespace std;

// DEFAULT CONSTRUCTOR IMPLEMENTATION
// LOGIC: INITIALIZES WITH SAFE FALLBACK VALUES SO STD::VECTOR CAN ALLOCATE MEMORY SAFELY
Question::Question()
    : category("UNKNOWN"), difficulty("UNKNOWN"), questionText("EMPTY QUESTION"), correctAnswer('A') 
{
}

// PARAMETERIZED CONSTRUCTOR IMPLEMENTATION
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

// APPLY 50/50 LIFELINE IMPLEMENTATION
// LOGIC: IDENTIFIES THE 3 INCORRECT OPTIONS, SHUFFLES THEM, AND MASKS 2 OF THEM
void Question::apply5050()
{
    // CONVERT CHARACTER (E.G., 'C') TO ARRAY INDEX (E.G., 2)
    int correctIdx = correctAnswer - 'A';
    vector<int> wrongIndices;
    
    // GATHER ALL INDICES THAT ARE NOT THE CORRECT ANSWER
    for (int i = 0; i < 4; ++i)
    {
        if (i != correctIdx)
            wrongIndices.push_back(i);
    }
    
    // RANDOMIZE THE WRONG INDICES TO ENSURE UNPREDICTABILITY
    random_device rd;
    mt19937 g(rd());
    shuffle(wrongIndices.begin(), wrongIndices.end(), g);
    
    // ELIMINATE THE FIRST TWO RANDOMIZED INCORRECT OPTIONS
    for (int i = 0; i < 2; ++i)
    {
        int eliminateIdx = wrongIndices[i];
        char optionLetter = 'A' + eliminateIdx;
        
        // OVERWRITE THE TEXT TO SHOW IT HAS BEEN REMOVED
        options[eliminateIdx] = string(1, optionLetter) + ") [ELIMINATED]";
    }
}