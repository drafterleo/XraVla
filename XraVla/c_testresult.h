#ifndef C_TESTRESULT_H
#define C_TESTRESULT_H

struct STestResult
{
    bool isCorrect;
    long timeMs;
    int  tipsCount;

    STestResult()
    {
        isCorrect = false;
        timeMs = 0;
        tipsCount = 0;
    }
};

#endif // C_TESTRESULT_H
