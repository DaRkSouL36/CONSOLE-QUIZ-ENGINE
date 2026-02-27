#include "QuizManager.h"
#include "../UTILS/FileLoader.h" 
#include <iostream>
#include <cctype>
#include <algorithm> 
#include <random>    
#include <chrono> 

using namespace std;
using namespace std::chrono; 

// CONSTRUCTOR IMPLEMENTATION
// LOGIC: INITIALIZES THE INTERNAL QUESTION LIST, SCORE, ATTEMPTS, AND DYNAMIC TIME LIMIT
QuizManager::QuizManager(const vector<Question>& loadedQuestions, double timeLimit)
{
    questions = loadedQuestions;
    score = 0;
    questionsAttempted = 0;
    timeLimitSeconds = timeLimit; 
    lifelinesRemaining = 3;
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
// LOGIC: INFINITE LOOP THAT BREAKS WHEN A VALID CHARACTER (A, B, C, D, OR X) IS ENTERED, DYNAMICALLY ADAPTS THE CONSOLE PROMPT AND VALIDATES 'H' INPUT
char QuizManager::getUserInput(bool hintUsedThisQuestion)
{
    char input;
    while (true)
    {
        cout << "\nENTER YOUR ANSWER (A/B/C/D)";
        
        // ONLY SHOW THE HINT OPTION IF THEY HAVEN'T USED IT THIS TURN AND HAVE SOME LEFT
        if (!hintUsedThisQuestion && lifelinesRemaining > 0)
            cout << ", 'H' FOR 50/50 (" << lifelinesRemaining << " LEFT)";
        
        cout << ", OR 'X' TO EXIT: ";
        cin >> input;
        input = toupper(input);

        if (input == 'A' || input == 'B' || input == 'C' || input == 'D' || input == 'X')
            return input;
        
        else if (input == 'H')
        {
            // VALIDATE IF THE HINT COMMAND IS LEGAL IN THIS CURRENT STATE
            if (hintUsedThisQuestion)
                cout << "YOU HAVE ALREADY USED A LIFELINE ON THIS QUESTION!\n";
            
            else if (lifelinesRemaining <= 0)
                cout << "YOU HAVE NO LIFELINES REMAINING!\n";
            
            else
                return input; // VALID HINT REQUEST
            
        }

        else
            cout << "INVALID INPUT. PLEASE TRY AGAIN.\n";
        
    }
}

// START METHOD IMPLEMENTATION
// LOGIC: USES THE DYNAMIC TIMELIMITSECONDS FOR EVALUATING TIMEOUTS, WRAPS INPUT FETCHING IN A WHILE LOOP TO PROCESS HINTS WITHOUT STOPPING THE TIMER
void QuizManager::start()
{
    if (questions.empty())
    {
        cout << "ERROR: NO QUESTIONS LOADED. CANNOT START THE QUIZ.\n";
        return;
    }

    randomizeQuestions();

    cout << "\n================================\n";
    cout << "   WELCOME TO QUIZ MASTER (C++)   \n";
    cout << "================================\n";
    cout << "NOTE: YOU HAVE " << timeLimitSeconds << " SECONDS PER QUESTION!\n";
    cout << "NOTE: YOU HAVE " << lifelinesRemaining << " 50/50 LIFELINES AVAILABLE.\n"; 

    for (size_t i = 0; i < questions.size(); ++i)
    {
        cout << "\n--- QUESTION " << (i + 1) << " OF " << questions.size() << " ---";
        questions[i].displayQuestion();

        bool hintUsedThisQuestion = false;
        char answer;
        
        // START TIMER - NOTE THAT IT CONTINUES RUNNING EVEN IF THEY PAUSE TO ASK FOR A HINT
        auto startTime = steady_clock::now();
        
        while (true)
        {
            answer = getUserInput(hintUsedThisQuestion);

            // INTERCEPT THE HINT COMMAND
            if (answer == 'H')
            {
                --lifelinesRemaining;
                hintUsedThisQuestion = true;
                
                cout << "\n>>> 50/50 LIFELINE ACTIVATED! (" << lifelinesRemaining << " REMAINING) <<<\n";
                questions[i].apply5050();
                questions[i].displayQuestion();
                // LOOP REPEATS TO ASK FOR THE ACTUAL ANSWER NOW
            }

            else
                break; // IT IS A STANDARD ANSWER OR 'X', BREAK THE INPUT LOOP            
        }

        // STOP TIMER
        auto endTime = steady_clock::now();
        duration<double> timeSpan = duration_cast<duration<double>>(endTime - startTime);
        double secondsTaken = timeSpan.count();

        if (answer == 'X')
        {
            cout << "\nEXITING QUIZ EARLY... CALCULATING RESULTS FOR ATTEMPTED QUESTIONS.\n";
            break; 
        }

        ++questionsAttempted;
        bool isCorrect = false;
        bool isTimeout = false;

        if (secondsTaken > timeLimitSeconds)
        {
            cout << "\nTIME'S UP! YOU TOOK " << secondsTaken << " SECONDS. (LIMIT: " << timeLimitSeconds << "s)\n";
            isTimeout = true;
            isCorrect = false; 
        }

        else
        {
            if (questions[i].isCorrect(answer))
            {
                cout << "CORRECT! (TIME: " << secondsTaken << "s)\n";
                isCorrect = true;
                ++score; 
            }

            else
                cout << "WRONG! THE CORRECT ANSWER WAS: " << questions[i].getCorrectAnswer() << "\n";
            
        }

        AnswerRecord record = {questions[i], answer, isCorrect, secondsTaken, isTimeout};
        attemptHistory.push_back(record);
    }

    displayResults();
}

// SHOW INCORRECT ANSWERS IMPLEMENTATION
// LOGIC: ITERATES THROUGH ATTEMPT HISTORY AND ONLY PRINTS OUT FAILED OR TIMED OUT QUESTIONS
void QuizManager::showIncorrectAnswers() const
{
    cout << "\n================================\n";
    cout << "     YOUR INCORRECT ANSWERS     \n";
    cout << "================================\n";

    bool foundWrong = false;

    for (size_t i = 0; i < attemptHistory.size(); ++i)
    {
        if (!attemptHistory[i].isCorrect)
        {
            foundWrong = true;
            cout << "\n--- QUESTION ---\n";
            attemptHistory[i].question.displayQuestion();
            
            if (attemptHistory[i].wasTimeout)
                cout << "\n[RESULT]: TIMEOUT (" << attemptHistory[i].timeTaken << "s)\n";
            
            else
                cout << "\n[YOUR ANSWER]: " << attemptHistory[i].userAnswer << "\n";
            
            cout << "[CORRECT ANSWER]: " << attemptHistory[i].question.getCorrectAnswer() << "\n";
            cout << "--------------------------------\n";
        }
    }

    if (!foundWrong)
        cout << "\nYOU DID NOT GET ANY QUESTIONS WRONG! PERFECT RUN!\n";
}

// DISPLAY RESULTS IMPLEMENTATION
// LOGIC: DISPLAYS STATS, ASKS FOR REVIEW OPTION, THEN PROMPTS FOR NAME TO SAVE SCORE.
void QuizManager::displayResults() const
{
    cout << "\n================================\n";
    cout << "         QUIZ FINISHED!         \n";
    cout << "================================\n";
    
    cout << "TOTAL QUESTIONS AVAILABLE: " << questions.size() << "\n";
    cout << "QUESTIONS ATTEMPTED:       " << questionsAttempted << "\n";
    cout << "CORRECT ANSWERS:           " << score << "\n";

    if (questionsAttempted > 0)
    {
        double percentage = (static_cast<double>(score) / questionsAttempted) * 100;
        cout << "PERCENTAGE:                " << percentage << "%\n";

        if (percentage >= 50.0)
            cout << "RESULT:                    PASS! GREAT JOB!\n";
        
        else
            cout << "RESULT:                    FAIL. BETTER LUCK NEXT TIME.\n";

        // OPTIONAL REVIEW PROMPT
        char wantReview;
        cout << "\nWOULD YOU LIKE TO SEE THE QUESTIONS YOU GOT WRONG? (Y/N): ";
        cin >> wantReview;

        if (toupper(wantReview) == 'Y')
            showIncorrectAnswers();

        // PROMPT USER FOR NAME TO RECORD HIGH SCORE
        cout << "\nENTER YOUR NAME TO SAVE YOUR SCORE: ";
        string playerName;
        
        cin >> ws; 
        getline(cin, playerName);

        // SAVE THE SCORE
        FileLoader::saveScore(playerName, score, questionsAttempted);

        // NEW: DISPLAY THE LEADERBOARD AFTER SAVING
        FileLoader::displayTopHighScores();
    }
    
    else
    {
        cout << "PERCENTAGE:                N/A (NO QUESTIONS ANSWERED)\n";
        cout << "RESULT:                    INCOMPLETE\n";
    }
    cout << "================================\n\n";
}