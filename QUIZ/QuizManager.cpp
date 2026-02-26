#include "QuizManager.h"
#include "../UTILS/FileLoader.h"
#include <iostream>
#include <cctype>
#include <algorithm> 
#include <random>    

using namespace std;

// CONSTRUCTOR IMPLEMENTATION
// LOGIC: INITIALIZES THE INTERNAL QUESTION LIST, SCORE, AND ATTEMPTS TO 0
QuizManager::QuizManager(const vector<Question>& loadedQuestions)
{
    questions = loadedQuestions;
    score = 0;
    questionsAttempted = 0;
}

// RANDOMIZE QUESTIONS IMPLEMENTATION
// LOGIC: GENERATES A RANDOM SEED AND SHUFFLES THE QUESTION ORDER BEFORE THE QUIZ STARTS
void QuizManager::randomizeQuestions()
{
    random_device rd;
    mt19937 g(rd());
    shuffle(questions.begin(), questions.end(), g);
}

// GET USER INPUT IMPLEMENTATION
// LOGIC: INFINITE LOOP THAT BREAKS WHEN A VALID CHARACTER (A, B, C, D, OR X) IS ENTERED
char QuizManager::getUserInput()
{
    char input;
    while (true)
    {
        cout << "\nENTER YOUR ANSWER (A/B/C/D) OR 'X' TO EXIT: ";
        cin >> input;
        
        // CONVERT TO UPPERCASE IMMEDIATELY FOR EASIER COMPARISON
        input = toupper(input);

        // CHECK IF THE INPUT IS ONE OF THE ALLOWED OPTIONS OR THE EXIT COMMAND
        if (input == 'A' || input == 'B' || input == 'C' || input == 'D' || input == 'X')
            return input;
        
        else
            cout << "INVALID INPUT. PLEASE ENTER EXACTLY A, B, C, D, OR X.\n";
    }
}

// START METHOD IMPLEMENTATION
// LOGIC: SHUFFLES QUESTIONS, LOOPS THROUGH THEM, CHECKS FOR EARLY EXIT ('X'), AND TRACKS ATTEMPTS
void QuizManager::start()
{
    // SAFETY CHECK: DO NOT START IF NO QUESTIONS WERE LOADED
    if (questions.empty())
    {
        cout << "ERROR: NO QUESTIONS LOADED. CANNOT START THE QUIZ.\n";
        return;
    }

    // RANDOMIZE THE QUESTION ORDER SO NO TWO GAMES ARE IDENTICAL
    randomizeQuestions();

    cout << "\n================================\n";
    cout << "   WELCOME TO QUIZ MASTER (C++)   \n";
    cout << "================================\n";

    // LOOP THROUGH ALL QUESTIONS USING PRE-INCREMENT FOR EFFICIENCY
    for (size_t i = 0; i < questions.size(); ++i)
    {
        cout << "\n--- QUESTION " << (i + 1) << " OF " << questions.size() << " ---";
        questions[i].displayQuestion();

        char answer = getUserInput();

        // CHECK FOR EARLY EXIT COMMAND
        if (answer == 'X')
        {
            cout << "\nEXITING QUIZ EARLY... CALCULATING RESULTS FOR ATTEMPTED QUESTIONS.\n";
            break; // BREAKS OUT OF THE FOR LOOP
        }

        // IF NOT EXITING, INCREMENT THE ATTEMPT COUNTER
        ++questionsAttempted;

        // EVALUATE THE ANSWER
        if (questions[i].isCorrect(answer))
        {
            cout << "CORRECT!\n";
            ++score; 
        }

        else
            cout << "WRONG! THE CORRECT ANSWER WAS: " << questions[i].getCorrectAnswer() << "\n";
    }

    // ONCE THE LOOP FINISHES OR BREAKS, SHOW THE FINAL RESULTS
    displayResults();
}

// DISPLAY RESULTS IMPLEMENTATION
// LOGIC: DISPLAYS STATS BASED ON ATTEMPTED QUESTIONS. PREVENTS DIVISION BY ZERO.
// DISPLAY RESULTS IMPLEMENTATION
// LOGIC: DISPLAYS STATS BASED ON ATTEMPTED QUESTIONS. PROMPTS FOR NAME AND SAVES SCORE.
void QuizManager::displayResults() const
{
    cout << "\n================================\n";
    cout << "         QUIZ FINISHED!         \n";
    cout << "================================\n";
    
    cout << "TOTAL QUESTIONS AVAILABLE: " << questions.size() << "\n";
    cout << "QUESTIONS ATTEMPTED:       " << questionsAttempted << "\n";
    cout << "CORRECT ANSWERS:           " << score << "\n";

    // PREVENT DIVIDE-BY-ZERO IF THE USER EXITS ON THE VERY FIRST QUESTION
    if (questionsAttempted > 0)
    {
        // CALCULATE PERCENTAGE BASED ON ATTEMPTED QUESTIONS ONLY
        double percentage = (static_cast<double>(score) / questionsAttempted) * 100;
        cout << "PERCENTAGE:                " << percentage << "%\n";

        // PASS/FAIL THRESHOLD
        if (percentage >= 50.0)
            cout << "RESULT:                    PASS! GREAT JOB!\n";
        
        else
            cout << "RESULT:                    FAIL. BETTER LUCK NEXT TIME.\n";

        // PROMPT USER FOR NAME TO RECORD HIGH SCORE
        cout << "\nENTER YOUR NAME TO SAVE YOUR SCORE: ";
        string playerName;
        
        // CLEAR THE INPUT BUFFER OF ANY LINGERING NEWLINES BEFORE GETLINE
        cin >> ws; 
        getline(cin, playerName);

        // CALL STATIC FILE I/O UTILITY TO APPEND SCORE TO FILE
        FileLoader::saveScore(playerName, score, questionsAttempted);
    }
    
    else
    {
        cout << "PERCENTAGE:                N/A (NO QUESTIONS ANSWERED)\n";
        cout << "RESULT:                    INCOMPLETE\n";
    }
    cout << "================================\n\n";
}