#include <iostream>
#include <vector>
#include <string>
#include "MODELS/Question.h"
#include "UTILS/FileLoader.h"
#include "QUIZ/QuizManager.h"

using namespace std;

// PURPOSE: MAIN ENTRY POINT FOR THE QUIZ MASTER APPLICATION
// INPUT: COMMAND LINE ARGUMENTS (NOT USED IN THIS BASIC VERSION)
// OUTPUT: INTEGER EXIT STATUS (0 FOR SUCCESS)
// LOGIC: DEFINES FILE PATH, LOADS QUESTIONS, INITIALIZES MANAGER, AND STARTS THE QUIZ
int main()
{
    // DEFINE THE RELATIVE PATH TO THE QUESTIONS TEXT FILE
    const string filePath = "DATA/Questions.txt";

    cout << "LOADING QUESTIONS FROM: " << filePath << "...\n";

    // LOAD THE QUESTIONS USING OUR STATIC UTILITY METHOD
    vector<Question> loadedQuestions = FileLoader::loadQuestions(filePath);

    // IF NO QUESTIONS WERE LOADED (FILE MISSING OR EMPTY), EXIT GRACEFULLY
    if (loadedQuestions.empty())
    {
        cout << "CRITICAL ERROR: FAILED TO LOAD QUESTIONS. EXITING PROGRAM.\n";
        return 1;
    }

    cout << "SUCCESSFULLY LOADED " << loadedQuestions.size() << " QUESTIONS.\n";

    // INSTANTIATE THE QUIZ MANAGER WITH THE LOADED QUESTIONS
    QuizManager quiz(loadedQuestions);

    // START THE MAIN GAME LOOP
    quiz.start();

    // RETURN 0 TO INDICATE SUCCESSFUL EXECUTION
    return 0;
}