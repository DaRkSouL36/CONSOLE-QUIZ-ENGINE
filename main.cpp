#include <iostream>
#include <vector>
#include <string>
#include <cctype> // IMPORT FOR TOUPPER
#include "MODELS/Question.h"
#include "UTILS/FileLoader.h"
#include "QUIZ/QuizManager.h"

using namespace std;

// PURPOSE: MAIN ENTRY POINT FOR THE QUIZ MASTER APPLICATION
// INPUT: COMMAND LINE ARGUMENTS
// OUTPUT: INTEGER EXIT STATUS (0 FOR SUCCESS)
// LOGIC: LOADS QUESTIONS ONCE, THEN ENTERS A DO-WHILE LOOP ALLOWING THE USER TO REPLAY
int main()
{
    const string filePath = "DATA/Questions.txt";

    cout << "LOADING QUESTIONS FROM: " << filePath << "...\n";

    vector<Question> loadedQuestions = FileLoader::loadQuestions(filePath);

    if (loadedQuestions.empty())
    {
        cout << "CRITICAL ERROR: FAILED TO LOAD QUESTIONS. EXITING PROGRAM.\n";
        return 1;
    }

    cout << "SUCCESSFULLY LOADED " << loadedQuestions.size() << " QUESTIONS.\n";

    char playAgain;
    
    // MASTER GAME LOOP
    do
    {
        // INSTANTIATE A FRESH MANAGER EACH TIME SO SCORES/HISTORY ARE RESET
        QuizManager quiz(loadedQuestions);
        quiz.start();

        // PROMPT USER TO PLAY AGAIN
        cout << "WOULD YOU LIKE TO PLAY AGAIN? (Y/N): ";
        cin >> playAgain;
        playAgain = toupper(playAgain);

    } while (playAgain == 'Y');

    cout << "\nTHANK YOU FOR PLAYING QUIZ MASTER. GOODBYE!\n";

    return 0;
}