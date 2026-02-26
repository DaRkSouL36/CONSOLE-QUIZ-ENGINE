#include "FileLoader.h"
#include <iostream>
#include <fstream>

using namespace std;

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