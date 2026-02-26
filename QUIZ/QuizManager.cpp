#include "QuizManager.h"
#include <iostream>
#include <cctype>

using namespace std;

// CONSTRUCTOR IMPLEMENTATION
// LOGIC: INITIALIZES THE INTERNAL QUESTION LIST AND SETS THE STARTING SCORE TO 0
QuizManager::QuizManager(const vector<Question>& loadedQuestions)
{
    questions = loadedQuestions;
    score = 0;
}

// GET USER INPUT IMPLEMENTATION
// LOGIC: INFINITE LOOP THAT ONLY BREAKS WHEN A VALID CHARACTER IS ENTERED
char QuizManager::getUserInput()
{
    char input;
    while (true)
    {
        cout << "\nENTER YOUR ANSWER (A/B/C/D): ";
        cin >> input;
        
        // CONVERT TO UPPERCASE IMMEDIATELY FOR EASIER COMPARISON
        input = toupper(input);

        // CHECK IF THE INPUT IS ONE OF THE ALLOWED OPTIONS
        if (input == 'A' || input == 'B' || input == 'C' || input == 'D')
            return input;
        
        else
            cout << "INVALID INPUT. PLEASE ENTER EXACTLY A, B, C, OR D.\n";
    }
}

// START METHOD IMPLEMENTATION
// LOGIC: CHECKS IF QUESTIONS EXIST, THEN LOOPS THROUGH EACH ONE USING PRE-INCREMENT
void QuizManager::start()
{
    // SAFETY CHECK: DO NOT START IF NO QUESTIONS WERE LOADED
    if (questions.empty())
    {
        cout << "ERROR: NO QUESTIONS LOADED. CANNOT START THE QUIZ.\n";
        return;
    }

    cout << "\n================================\n";
    cout << "   WELCOME TO QUIZ MASTER (C++)   \n";
    cout << "================================\n";

    // LOOP THROUGH ALL QUESTIONS USING PRE-INCREMENT FOR EFFICIENCY
    for (size_t i = 0; i < questions.size(); ++i)
    {
        cout << "\n--- QUESTION " << (i + 1) << " OF " << questions.size() << " ---";
        questions[i].displayQuestion();

        char answer = getUserInput();

        // EVALUATE THE ANSWER
        if (questions[i].isCorrect(answer))
        {
            cout << "CORRECT!\n";
            ++score; // USING PRE-INCREMENT FOR THE SCORE AS WELL
        }

        else
            cout << "WRONG! THE CORRECT ANSWER WAS: " << questions[i].getCorrectAnswer() << "\n";
    }

    // ONCE THE LOOP FINISHES, SHOW THE FINAL RESULTS
    displayResults();
}

// DISPLAY RESULTS IMPLEMENTATION
// LOGIC: CALCULATES THE PERCENTAGE AND DETERMINES IF THE USER PASSED (>= 50%)
void QuizManager::displayResults() const
{
    cout << "\n================================\n";
    cout << "         QUIZ FINISHED!         \n";
    cout << "================================\n";
    
    cout << "TOTAL QUESTIONS: " << questions.size() << "\n";
    cout << "CORRECT ANSWERS: " << score << "\n";

    // CALCULATE PERCENTAGE (CAST TO DOUBLE TO AVOID INTEGER DIVISION TRUNCATION)
    double percentage = (static_cast<double>(score) / questions.size()) * 100;
    cout << "PERCENTAGE:      " << percentage << "%\n";

    // PASS/FAIL THRESHOLD
    if (percentage >= 50.0)
        cout << "RESULT:          PASS! GREAT JOB!\n";
    
    else
        cout << "RESULT:          FAIL. BETTER LUCK NEXT TIME.\n";
        
    cout << "================================\n\n";
}