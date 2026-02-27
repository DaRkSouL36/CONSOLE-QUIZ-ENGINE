#pragma once
#include <vector>
#include "../MODELS/Question.h"

using namespace std;

// STRUCT: ANSWERRECORD
// PURPOSE: A DATA STRUCTURE TO HOLD THE HISTORY OF THE USER'S ATTEMPT FOR A SPECIFIC QUESTION
struct AnswerRecord
{
    Question question;
    char userAnswer;
    bool isCorrect;
    double timeTaken;
    bool wasTimeout;
};

// CLASS: QUIZMANAGER
// PURPOSE: CONTROLS THE MAIN GAME FLOW, TRACKS THE USER SCORE, LOGS HISTORY, AND HANDLES DYNAMIC TIMERS
class QuizManager
{
private:
    vector<Question> questions;
    vector<AnswerRecord> attemptHistory; 
    int score;
    int questionsAttempted; 
    double timeLimitSeconds; 

    // PURPOSE: PROMPTS THE USER FOR AN ANSWER AND STRICTLY VALIDATES IT
    // INPUT: NONE
    // OUTPUT: A VALIDATED UPPERCASE CHARACTER
    char getUserInput();

    // PURPOSE: SHUFFLES THE INTERNAL VECTOR OF QUESTIONS TO ENSURE A UNIQUE EXPERIENCE EVERY TIME
    void randomizeQuestions();

    // PURPOSE: PROMPTS THE USER IF THEY WANT TO SEE THEIR INCORRECT ANSWERS
    void showIncorrectAnswers() const;

public:
    // PURPOSE: CONSTRUCTOR TO INITIALIZE THE QUIZ MANAGER WITH DYNAMIC TIMERS
    // INPUT: A VECTOR OF LOADED QUESTION OBJECTS AND A TIME LIMIT DOUBLE
    // OUTPUT: NONE
    // LOGIC: COPIES THE QUESTIONS AND INITIALIZES SCORE AND TIMER
    QuizManager(const vector<Question>& loadedQuestions, double timeLimit);

    // PURPOSE: STARTS AND RUNS THE MAIN QUIZ LOOP
    void start();

    // PURPOSE: DISPLAYS THE FINAL SCORE, PERCENTAGE, PASS/FAIL STATUS, AND OPTIONAL REVIEW
    void displayResults() const;
};