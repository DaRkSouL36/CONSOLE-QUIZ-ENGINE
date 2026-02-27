#include "FileLoader.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

// STRUCT: SCORERECORD
// PURPOSE: INTERNAL DATA STRUCTURE TO HOLD PARSED SCORE DATA FOR SORTING
struct ScoreRecord
{
    string name;
    int score;
    int attempted;
};

// LOAD QUESTIONS IMPLEMENTATION
// LOGIC: USES STD::IFSTREAM TO OPEN THE FILE. READS CHUNKS OF 6 LINES AT A TIME.
//        SKIPS BLANK LINES. SECURELY GRABS THE LAST CHARACTER OF THE ANSWER LINE.
vector<Question> FileLoader::loadQuestions(const string& filePath)
{
    vector<Question> questions;
    ifstream file(filePath);

    // CHECK IF THE FILE OPENED SUCCESSFULLY
    if (!file.is_open())
    {
        cerr << "ERROR: COULD NOT OPEN FILE AT " << filePath << "\n";
        return questions; // RETURNS AN EMPTY VECTOR TO PREVENT CRASHES
    }

    string line;
    
    // READ UNTIL THE END OF THE FILE
    while (getline(file, line))
    {
        // SKIP EMPTY LINES THAT ACT AS SEPARATORS BETWEEN QUESTION BLOCKS
        if (line.empty())
            continue;

        // IF THE LINE IS NOT EMPTY, IT MUST BE THE START OF A NEW QUESTION.
        // STORE THE QUESTION TEXT.
        string questionText = line;
        vector<string> options;
        
        // READ THE NEXT 4 LINES FOR THE OPTIONS (A, B, C, D)
        for (int i = 0; i < 4; ++i)
        {
            if (getline(file, line))
                options.push_back(line);
        }

        // READ THE ANSWER LINE (E.G., "ANSWER: A")
        char correctAnswer = 'A'; // DEFAULT FALLBACK IN CASE OF PARSING ERROR
        if (getline(file, line))
        {
            // EXTRACT THE VERY LAST CHARACTER OF THE LINE (WHICH SHOULD BE A, B, C, OR D)
            if (!line.empty())
                correctAnswer = line.back(); 
        }

        // INSTANTIATE A NEW QUESTION OBJECT AND ADD IT TO OUR COLLECTION
        Question q(questionText, options, correctAnswer);
        questions.push_back(q);
    }

    file.close();
    return questions;
}

// SAVE SCORE IMPLEMENTATION
// LOGIC: USES STD::OFSTREAM IN APPEND MODE TO ADD A NEW RECORD TO THE FILE
void FileLoader::saveScore(const string& playerName, int score, int attempted)
{
    // OPEN THE FILE IN APPEND MODE
    ofstream file("DATA/HighScores.txt", ios::app);

    if (file.is_open())
    {
        file << "PLAYER: " << playerName << " | SCORE: " << score << "/" << attempted << "\n";
        file.close();
        cout << "HIGH SCORE SAVED SUCCESSFULLY TO DATA/HighScores.txt!\n";
    }

    else
        cerr << "ERROR: COULD NOT OPEN HIGH SCORES FILE FOR WRITING.\n";
}

// DISPLAY TOP HIGH SCORES IMPLEMENTATION
// LOGIC: PARSES THE TEXT FILE, EXTRACTS SUBSTRINGS, CONVERTS TO INTEGERS, SORTS, AND PRINTS TOP 5
void FileLoader::displayTopHighScores()
{
    ifstream file("DATA/HighScores.txt");
    vector<ScoreRecord> records;

    // IF FILE DOESN'T EXIST YET, JUST RETURN GRACEFULLY
    if (!file.is_open())
    {
        return; 
    }

    string line;
    
    // READ THE FILE LINE BY LINE
    while (getline(file, line))
    {
        // EXPECTED FORMAT: "PLAYER: [NAME] | SCORE: [SCORE]/[ATTEMPTED]"
        size_t playerPos = line.find("PLAYER: ");
        size_t separatorPos = line.find(" | SCORE: ");
        size_t slashPos = line.find("/");

        // ENSURE ALL EXPECTED DELIMITERS EXIST TO PREVENT PARSING ERRORS
        if (playerPos != string::npos && separatorPos != string::npos && slashPos != string::npos)
        {
            // EXTRACT THE SUBSTRINGS BASED ON DELIMITER POSITIONS
            string name = line.substr(playerPos + 8, separatorPos - (playerPos + 8));
            string scoreStr = line.substr(separatorPos + 10, slashPos - (separatorPos + 10));
            string attemptStr = line.substr(slashPos + 1);

            try 
            {
                ScoreRecord sr;
                sr.name = name;
                sr.score = stoi(scoreStr);
                sr.attempted = stoi(attemptStr);
                records.push_back(sr);
            }
            catch (...) 
            {
                // SILENTLY CATCH ANY STOI (STRING TO INTEGER) CONVERSION ERRORS IF FILE WAS CORRUPTED
                continue; 
            }
        }
    }
    file.close();

    // SORT THE RECORDS USING A CUSTOM LAMBDA FUNCTION
    // LOGIC: SORT DESCENDING BY SCORE. IF SCORES ARE EQUAL, SORT ASCENDING BY ATTEMPTS (EFFICIENCY).
    sort(records.begin(), records.end(), [](const ScoreRecord& a, const ScoreRecord& b) 
    {
        if (a.score == b.score)
        {
            return a.attempted < b.attempted;
        }
        return a.score > b.score;
    });

    // DISPLAY THE LEADERBOARD
    cout << "\n================================\n";
    cout << "       TOP 5 HIGH SCORES        \n";
    cout << "================================\n";

    if (records.empty())
    {
        cout << "NO SCORES RECORDED YET.\n";
    }
    else
    {
        // PRINT ONLY UP TO THE FIRST 5 RECORDS
        for (size_t i = 0; i < records.size() && i < 5; ++i)
        {
            cout << (i + 1) << ". " << records[i].name << " - " 
                 << records[i].score << "/" << records[i].attempted << "\n";
        }
    }
    cout << "================================\n\n";
}