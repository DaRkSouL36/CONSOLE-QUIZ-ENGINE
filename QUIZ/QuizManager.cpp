#include "QuizManager.h"
#include "../UTILS/FileLoader.h" 
#include <iostream>
#include <cctype>
#include <algorithm> 
#include <random>    
#include <chrono> // IMPORT FOR TIMERS

using namespace std;
using namespace std::chrono; // MAKES CHRONO CODE CLEANER TO READ

// CONSTRUCTOR IMPLEMENTATION
// LOGIC: INITIALIZES THE INTERNAL QUESTION LIST, SCORE, ATTEMPTS, AND DYNAMIC TIME LIMIT
QuizManager::QuizManager(const vector<Question>& loadedQuestions, double timeLimit)
{
    questions = loadedQuestions;
    score = 0;
    questionsAttempted = 0;
    timeLimitSeconds = timeLimit; // NEW: ASSIGN THE DYNAMIC LIMIT
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
// LOGIC: USES THE DYNAMIC TIMELIMITSECONDS FOR EVALUATING TIMEOUTS
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
    cout << "NOTE: YOU HAVE " << timeLimitSeconds << " SECONDS PER QUESTION!\n"; // UPDATED

    for (size_t i = 0; i < questions.size(); ++i)
    {
        cout << "\n--- QUESTION " << (i + 1) << " OF " << questions.size() << " ---";
        questions[i].displayQuestion();

        auto startTime = steady_clock::now();
        char answer = getUserInput();
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

        // CHECK IF THEY TOOK TOO LONG USING THE DYNAMIC LIMIT
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
            {
                cout << "WRONG! THE CORRECT ANSWER WAS: " << questions[i].getCorrectAnswer() << "\n";
            }
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