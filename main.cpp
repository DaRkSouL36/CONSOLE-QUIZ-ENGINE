#include <iostream>
#include <vector>
#include <string>
#include <cctype> 
#include <set>       // IMPORT FOR EXTRACTING UNIQUE CATEGORIES
#include <algorithm> // IMPORT FOR SHUFFLING AND COPY_IF
#include <random>    // IMPORT FOR SHUFFLING BEFORE SLICING LIMIT
#include "MODELS/Question.h"
#include "UTILS/FileLoader.h"
#include "QUIZ/QuizManager.h"

using namespace std;

// PURPOSE: PROMPTS USER SAFELY FOR AN INTEGER TO PREVENT INFINITE LOOPS ON BAD INPUT
int getValidIntInput()
{
    int choice;
    while (true)
    {
        if (cin >> choice)
            return choice;

        // IF CIN FAILS (USER ENTERED A LETTER), CLEAR ERROR FLAG AND IGNORE BAD INPUT
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "INVALID INPUT. PLEASE ENTER A NUMBER: ";
    }
}

// PURPOSE: MAIN ENTRY POINT FOR THE QUIZ MASTER APPLICATION
int main()
{
    const string filePath = "DATA/Questions.txt";
    cout << "LOADING QUESTIONS FROM: " << filePath << "...\n";

    vector<Question> masterQuestions = FileLoader::loadQuestions(filePath);

    if (masterQuestions.empty())
    {
        cout << "CRITICAL ERROR: FAILED TO LOAD QUESTIONS. EXITING PROGRAM.\n";
        return 1;
    }

    // EXTRACT UNIQUE CATEGORIES FOR THE DYNAMIC MENU
    set<string> categories;
    for (size_t i = 0; i < masterQuestions.size(); ++i)
    {
        categories.insert(masterQuestions[i].getCategory());
    }

    char playAgain;
    
    // MASTER GAME LOOP
    do
    {
        cout << "\n================================\n";
        cout << "          GAME SETUP            \n";
        cout << "================================\n";

        // --- 1. CATEGORY SELECTION ---
        cout << "SELECT A CATEGORY:\n";
        cout << "0. ALL CATEGORIES\n";
        int catIndex = 1;
        vector<string> catList(categories.begin(), categories.end());
        
        for (size_t i = 0; i < catList.size(); ++i)
            cout << catIndex++ << ". " << catList[i] << "\n";
        
        cout << "CHOICE: ";
        int catChoice = getValidIntInput();
        string selectedCategory = (catChoice > 0 && catChoice <= catList.size()) ? catList[catChoice - 1] : "ALL";

        // --- 2. DIFFICULTY SELECTION ---
        cout << "\nSELECT DIFFICULTY (AFFECTS TIMER):\n";
        cout << "1. EASY   (30 SECONDS/QUESTION)\n";
        cout << "2. MEDIUM (20 SECONDS/QUESTION)\n";
        cout << "3. HARD   (10 SECONDS/QUESTION)\n";
        cout << "4. ANY    (15 SECONDS/QUESTION)\n";
        cout << "CHOICE: ";
        
        int diffChoice = getValidIntInput();
        string selectedDifficulty = "ALL";
        double timeLimit = 15.0; // DEFAULT

        if (diffChoice == 1) { selectedDifficulty = "EASY"; timeLimit = 30.0; }
        else if (diffChoice == 2) { selectedDifficulty = "MEDIUM"; timeLimit = 20.0; }
        else if (diffChoice == 3) { selectedDifficulty = "HARD"; timeLimit = 10.0; }

        // --- 3. FILTER THE QUESTION POOL ---
        vector<Question> filteredQuestions;
        for (size_t i = 0; i < masterQuestions.size(); ++i)
        {
            bool matchCat = (selectedCategory == "ALL" || masterQuestions[i].getCategory() == selectedCategory);
            bool matchDiff = (selectedDifficulty == "ALL" || masterQuestions[i].getDifficulty() == selectedDifficulty);
            
            if (matchCat && matchDiff)
                filteredQuestions.push_back(masterQuestions[i]);
        }

        // CHECK IF ANY QUESTIONS MATCHED THE FILTERS
        if (filteredQuestions.empty())
        {
            cout << "\nNO QUESTIONS MATCH THOSE FILTERS. PLEASE TRY AGAIN.\n";
            playAgain = 'Y';
            continue; // SKIP THE REST OF THE LOOP AND RESTART MENU
        }

        // --- 4. QUESTION LIMIT SELECTION ---
        cout << "\nFOUND " << filteredQuestions.size() << " MATCHING QUESTIONS.\n";
        cout << "ENTER NUMBER OF QUESTIONS TO PLAY (1 TO " << filteredQuestions.size() << "): ";
        int limitChoice = getValidIntInput();
        
        if (limitChoice < 1) 
            limitChoice = 1;
        if (limitChoice > filteredQuestions.size()) 
            limitChoice = filteredQuestions.size();

        // SHUFFLE THE FILTERED POOL BEFORE SLICING SO WE GET A RANDOM SUBSET
        random_device rd;
        mt19937 g(rd());
        shuffle(filteredQuestions.begin(), filteredQuestions.end(), g);

        // SLICE THE VECTOR TO THE REQUESTED LIMIT
        filteredQuestions.resize(limitChoice);

        // --- 5. START QUIZ ---
        // INSTANTIATE WITH FILTERED/SLICED QUESTIONS AND THE DYNAMIC TIMELIMIT
        QuizManager quiz(filteredQuestions, timeLimit);
        quiz.start();

        // PROMPT USER TO PLAY AGAIN
        cout << "WOULD YOU LIKE TO PLAY AGAIN? (Y/N): ";
        cin >> playAgain;
        playAgain = toupper(playAgain);

    } while (playAgain == 'Y');

    cout << "\nTHANK YOU FOR PLAYING QUIZ MASTER. GOODBYE!\n";
    return 0;
}