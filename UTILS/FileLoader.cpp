#include "FileLoader.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>  
#include <iomanip>

using namespace std;

// STRUCT: SCORERECORD
// PURPOSE: INTERNAL DATA STRUCTURE TO HOLD PARSED SCORE DATA FOR SORTING
struct ScoreRecord
{
    string name;
    int score;
    int attempted;
    double percentage;
    string date;
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
// LOGIC: USES STD::OFSTREAM IN APPEND MODE TO ADD A NEW RECORD TO THE FILE; USES CTIME TO GET SYSTEM TIME, FORMATS IT, AND APPENDS IT WITH THE SCORE
void FileLoader::saveScore(const string& playerName, int score, int attempted)
{
    // GET CURRENT SYSTEM TIME
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    // FORMAT THE TIME INTO A STRING BUFFER
    char dateBuffer[30];
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M:%S", ltm);

    ofstream file("DATA/HighScores.txt", ios::app);

    if (file.is_open())
    {
        // APPEND SCORE WITH THE NEW DATE DELIMITER
        file << "PLAYER: " << playerName 
             << " | SCORE: " << score << "/" << attempted 
             << " | DATE: " << dateBuffer << "\n";
            
        file.close();
        cout << "HIGH SCORE SAVED SUCCESSFULLY!\n";
    }

    else
        cerr << "ERROR: COULD NOT OPEN HIGH SCORES FILE FOR WRITING.\n";
}

// DISPLAY TOP HIGH SCORES IMPLEMENTATION
// LOGIC: EXTRACTS SUBSTRINGS, CALCULATES PERCENTAGE, SORTS BY PERCENTAGE DESCENDING, AND FORMATS OUTPUT
void FileLoader::displayTopHighScores()
{
    ifstream file("DATA/HighScores.txt");
    vector<ScoreRecord> records;

    if (!file.is_open())
        return; 

    string line;
    
    while (getline(file, line))
    {
        size_t playerPos = line.find("PLAYER: ");
        size_t separatorPos = line.find(" | SCORE: ");
        size_t slashPos = line.find("/");
        size_t datePos = line.find(" | DATE: "); // NEW DELIMITER SEARCH

        if (playerPos != string::npos && separatorPos != string::npos && slashPos != string::npos)
        {
            string name = line.substr(playerPos + 8, separatorPos - (playerPos + 8));
            string scoreStr = line.substr(separatorPos + 10, slashPos - (separatorPos + 10));
            string attemptStr;
            string dateStr = "N/A"; // FALLBACK IF READING AN OLDER SAVE FILE WITHOUT A DATE

            // CHECK IF DATE EXISTS TO SUPPORT BACKWARDS COMPATIBILITY
            if (datePos != string::npos)
            {
                attemptStr = line.substr(slashPos + 1, datePos - (slashPos + 1));
                dateStr = line.substr(datePos + 9); 
            }

            else
                attemptStr = line.substr(slashPos + 1);

            try 
            {
                ScoreRecord sr;
                sr.name = name;
                sr.score = stoi(scoreStr);
                sr.attempted = stoi(attemptStr);
                sr.date = dateStr;
                
                // CALCULATE PERCENTAGE TO ALLOW FAIR SORTING
                sr.percentage = (sr.attempted > 0) ? (static_cast<double>(sr.score) / sr.attempted) * 100.0 : 0.0;
                
                records.push_back(sr);
            }

            catch (...) 
            {
                continue; 
            }
        }
    }

    file.close();

    // SORT THE RECORDS USING A CUSTOM LAMBDA FUNCTION (BY PERCENTAGE FIRST, THEN TOTAL SCORE)
    sort(records.begin(), records.end(), [](const ScoreRecord& a, const ScoreRecord& b) 
    {
        if (a.percentage == b.percentage)
            return a.score > b.score; // TIE BREAKER: RAW SCORE
        
        return a.percentage > b.percentage;
    });

    // DISPLAY THE LEADERBOARD
    cout << "\n================================\n";
    cout << "       TOP 5 HIGH SCORES        \n";
    cout << "================================\n";

    if (records.empty())
        cout << "NO SCORES RECORDED YET.\n";
    
    else
    {
        for (size_t i = 0; i < records.size() && i < 5; ++i)
        {
            cout << (i + 1) << ". " << records[i].name 
                 << " - " << fixed << setprecision(1) << records[i].percentage << "% "
                 << "(" << records[i].score << "/" << records[i].attempted << ") "
                 << "[" << records[i].date << "]\n";
        }
    }
    cout << "================================\n\n";
}