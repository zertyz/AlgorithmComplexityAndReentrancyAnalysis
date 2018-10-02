#include <iostream>
#include <map>
#include <unordered_map>
#include <thread>
#include <random>
#include <algorithm>

#include <mutex>

using namespace std;

#include "../../../../CppUtils/cpp/TimeMeasurements.h"
using namespace mutua::cpputils;

#include "../../cpp/AlgorithmComplexityAndReentrancyAnalysis.h"
#include "../../../SplitRun/cpp/SplitRun.h"
using namespace mutua::testutils;


static unsigned int DELAY_CONSTANT = 99999;


// insert functions
///////////////////
// 'n' should be the number of the element being inserted
// note: returning 'r' is needed in order for compiler optimizations not avoid looping

unsigned int O1Insert(unsigned int n) {
    unsigned int r = 0;
    for (unsigned int i=0; i<DELAY_CONSTANT; i++) r ^= i;
    return r;
}

unsigned int OLogNInsert(unsigned int n) {
    unsigned int r = 0;
    while (n > 0) {
        for (unsigned int i=0; i<DELAY_CONSTANT; i++) r ^= i;
        n /= 2;
    }
    return r;
}

unsigned int ONInsert(unsigned int n) {
    unsigned int r = 0;
    while (n > 0) {
        for (unsigned int i=0; i<DELAY_CONSTANT; i++) r ^= i;
        n -= 1;
    }
    return r;
}


// update functions
///////////////////
// 'n' should be the total elements on each pass
// note: returning 'r' is needed in order for compiler optimizations not avoid looping

unsigned int O1Update(unsigned int n) {
    unsigned int r = 0;
    for (unsigned int i=0; i<DELAY_CONSTANT; i++) r ^= i;
    return r;
}

unsigned int OLogNUpdate(unsigned int n) {
    unsigned int r = 0;
    while (n > 0) {
        for (unsigned int i=0; i<DELAY_CONSTANT; i++) r ^= i;
        n /= 2;
    }
    return r;
}

unsigned int ONUpdate(unsigned int n) {
    unsigned int r = 0;
    while (n > 0) {
        for (unsigned int i=0; i<DELAY_CONSTANT; i++) r ^= i;
        n -= 1;
    }
    return r;
}


// test & exploration functions
///////////////////////////////

void databaseAlgorithmAnalysisLowLevelExperiments() {

    unsigned long long                              start1US, end1US, start2US, end2US, delta1US, delta2US;
    unsigned int                                    n, r=0;
    AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity complexity;
    string                                          algorithmAnalysisReport;

    cout << endl << endl;
    cout << "Database Algorithm Analysis Low Level Experiments:" << endl;
    cout << "================================================= " << endl << endl;

    cout << "EAlgorithmComplexity members, as per EAlgorithmComplexityToVerboseString:" << endl;
    cout << "\t::BetterThanO1:      " << AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexityToString(AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity::BetterThanO1)      << endl;
    cout << "\t::O1:                " << AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexityToString(AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity::O1)                << endl;
    cout << "\t::Ologn:             " << AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexityToString(AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity::Ologn)             << endl;
    cout << "\t::BetweenOLogNAndOn: " << AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexityToString(AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity::BetweenOLogNAndOn) << endl;
    cout << "\t::On:                " << AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexityToString(AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity::On)                << endl;
    cout << "\t::WorseThanOn:       " << AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexityToString(AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity::WorseThanOn)       << endl;

    // insertion test:
    cout << "If Updating/Selecting in O(1), I'd get: " << endl;
    tie(complexity, algorithmAnalysisReport) = AlgorithmComplexityAndReentrancyAnalysis::computeUpdateOrSelectAlgorithmAnalysis("Update/Select Test"s, 100, 200, 200, 300, 1000, 2000, 1000);
    cout << algorithmAnalysisReport << flush;
    cout << "If Updating/Selecting in O(log(n)), I'd get: " << endl;
    tie(complexity, algorithmAnalysisReport) = AlgorithmComplexityAndReentrancyAnalysis::computeUpdateOrSelectAlgorithmAnalysis("Update/Select Test"s, 100, 200, 200, 311, 1000, 2000, 1000);
    cout << algorithmAnalysisReport << flush;

    AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity lastUpdateOrSelectComplexity = AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity::BetterThanO1;
    AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity currentUpdateOrSelectComplexity;
    for (unsigned long endMillis = 300; endMillis < 10000; endMillis++) {
        tie(currentUpdateOrSelectComplexity, algorithmAnalysisReport) = AlgorithmComplexityAndReentrancyAnalysis::computeUpdateOrSelectAlgorithmAnalysis("Insertion Test"s, 100, 200, 200, endMillis, 1000, 2000, 1000);
        if (currentUpdateOrSelectComplexity != lastUpdateOrSelectComplexity) {
            cout << "Update/Select test started measuring " << AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexityToString(currentUpdateOrSelectComplexity) << " at endMillis " << endMillis << endl;
            lastUpdateOrSelectComplexity = currentUpdateOrSelectComplexity;
        }
    }

    AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity lastInsertComplexity = AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity::BetterThanO1;
    AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity currentInsertComplexity;
    for (unsigned long endMillis = 300; endMillis < 10000; endMillis++) {
        tie(currentInsertComplexity, algorithmAnalysisReport) = AlgorithmComplexityAndReentrancyAnalysis::computeInsertOrDeleteAlgorithmAnalysis(
                "Insert",
                100, 200, 200,
                endMillis, 1000);
        if (currentInsertComplexity != lastInsertComplexity) {
            cout << "Insert test started measuring " << AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexityToString(currentInsertComplexity) << " at endMillis " << endMillis << endl;
            lastInsertComplexity = currentInsertComplexity;
        }
    }

#define TEST_INSERT(testName, insertFunction)                                                                                                              \
    n = 1000;                                                                                                                                              \
    cout << "Real " << testName << " with " << n << " elements on each pass" << flush;                                                                     \
    start1US = TimeMeasurements::getMonotonicRealTimeUS();                                                                                                 \
    for (unsigned int e=0; e<n; e++) {                                                                                                                     \
        r += insertFunction(e);                                                                                                                            \
    }                                                                                                                                                      \
    end1US   = TimeMeasurements::getMonotonicRealTimeUS();                                                                                                 \
    delta1US = end1US - start1US;                                                                                                                          \
    cout << " (pass1 = " << (delta1US) << "µs" << flush;                                                                                                   \
    start2US = TimeMeasurements::getMonotonicRealTimeUS();                                                                                                 \
    for (unsigned int e=n; e<n*2; e++) {                                                                                                                   \
        r += insertFunction(e);                                                                                                                            \
    }                                                                                                                                                      \
    end2US   = TimeMeasurements::getMonotonicRealTimeUS();                                                                                                 \
    delta2US = end2US - start2US;                                                                                                                          \
    cout << "; pass2 = " << (delta2US) << "µs): " << endl << flush;                                                                                        \
    tie(complexity, algorithmAnalysisReport) = AlgorithmComplexityAndReentrancyAnalysis::computeInsertOrDeleteAlgorithmAnalysis("Insert/Delete", start1US, end1US, start2US, end2US, n); \
    cerr << algorithmAnalysisReport << "--> (computation result is " << r << ")" << endl << flush;

#define TEST_UPDATE(testName, updateFunction)                                                                                                                       \
    n = 1000;                                                                                                                                                       \
    cout << "Real " << testName << " with " << n << " elements on each pass" << flush;                                                                              \
    start1US = TimeMeasurements::getMonotonicRealTimeUS();                                                                                                          \
    for (unsigned int e=0; e<n; e++) {                                                                                                                              \
        r += updateFunction(n);                                                                                                                                     \
    }                                                                                                                                                               \
    end1US   = TimeMeasurements::getMonotonicRealTimeUS();                                                                                                          \
    delta1US = end1US - start1US;                                                                                                                                   \
    cout << " (pass1 = " << (delta1US) << "µs" << flush;                                                                                                            \
    start2US = TimeMeasurements::getMonotonicRealTimeUS();                                                                                                          \
    for (unsigned int e=n; e<n*2; e++) {                                                                                                                            \
        r += updateFunction(n*2);                                                                                                                                   \
    }                                                                                                                                                               \
    end2US   = TimeMeasurements::getMonotonicRealTimeUS();                                                                                                          \
    delta2US = end2US - start2US;                                                                                                                                   \
    cout << "; pass2 = " << (delta2US) << "µs): " << endl << flush;                                                                                                 \
    tie(complexity, algorithmAnalysisReport) = AlgorithmComplexityAndReentrancyAnalysis::computeUpdateOrSelectAlgorithmAnalysis("Select/Update", start1US, end1US, start2US, end2US, n, n*2, n); \
    cerr << algorithmAnalysisReport << "--> (computation result is " << r << ")" << endl << flush;

    TEST_INSERT("O(1) Insertion Test", O1Insert);
    TEST_INSERT("O(log(n)) Insertion Test", OLogNInsert);
    TEST_INSERT("O(n) Insertion Test", ONInsert);

    TEST_UPDATE("O(1) Update Test",      O1Update);
    TEST_UPDATE("O(log(n)) Update Test", OLogNUpdate);
    TEST_UPDATE("O(n) Update Test",      ONUpdate);
}

void databaseAlgorithmAnalysisHighLevelExperiments() {

#define _numberOfElements 200'000
#define _threads          10

    cout << endl << endl;
    cout << "Database Algorithm Analysis High Level Experiments:" << endl;
    cout << "================================================== " << endl << endl;

    class HelloDatabaseAlgorithmAnalysisWorld: public AlgorithmComplexityAndReentrancyAnalysis {
    public:
        std::vector<int> elements;

        HelloDatabaseAlgorithmAnalysisWorld()
                : AlgorithmComplexityAndReentrancyAnalysis("Hello, Database Algorithm Analysis World!!", 2000, 2000, 2000) {
            elements = std::vector<int>(2000);
        }

        void resetTables(EResetOccasion occasion) override {
            elements.clear();
        }

        void insertForComplexityAnalysis(unsigned int i) override {
            elements[i] = i;
        }

        void updateForComplexityAnalysis(unsigned int i) override {
            elements[i] = -i;
        }

        void selectForComplexityAnalysis(unsigned int i) override {
            int e = elements[i];
        }

        void deleteForComplexityAnalysis(unsigned int i) override {
            elements.pop_back();
        }
    };
    HelloDatabaseAlgorithmAnalysisWorld().analyseComplexity(true, 4, 4, 4, 4, true);

    class ReentrancyExperiments: public AlgorithmComplexityAndReentrancyAnalysis {
    public:
        std::vector<int> insertElements;
        std::vector<int> updateElements;
        std::vector<int> selectElements;
        std::vector<int> deleteElements;
        std::mutex opGuard;
        int currentOperation;   // 1: inserting, 2: updating, 3: selecting, 4: deleting
        int pass;
        int index;

        ReentrancyExperiments()
                : AlgorithmComplexityAndReentrancyAnalysis("ReentrancyExperiments", _numberOfElements) {
            insertElements = std::vector<int>(_numberOfElements, 0);
            updateElements = std::vector<int>(_numberOfElements, 0);
            selectElements = std::vector<int>(_numberOfElements, 0);
            deleteElements = std::vector<int>(_numberOfElements, 0);
            currentOperation = 0;
        }

        void resetTables(EResetOccasion occasion) override {}

        void insertForComplexityAnalysis(unsigned int i) override {

            std::lock_guard<std::mutex> lock(opGuard);

            if (currentOperation != 1) {
                if (currentOperation == 0) {
                    pass = 1;
                } else {
                    pass = 2;
                }
                currentOperation = 1;
                index = (pass-1)*(_numberOfElements/2);
            }

            insertElements[index++]++;
        }

        void updateForComplexityAnalysis(unsigned int i) override {

            std::lock_guard<std::mutex> lock(opGuard);

            if (currentOperation != 2) {
                currentOperation = 2;
                index = (pass-1)*(_numberOfElements/2);
            }

            updateElements[index++]++;
        }

        void selectForComplexityAnalysis(unsigned int i) override {

            std::lock_guard<std::mutex> lock(opGuard);

            if (currentOperation != 3) {
                currentOperation = 3;
                index = (pass-1)*(_numberOfElements/2);
            }

            selectElements[index++]++;
        }

        void deleteForComplexityAnalysis(unsigned int i) override {

            std::lock_guard<std::mutex> lock(opGuard);

            if (currentOperation != 4) {
                currentOperation = 4;
                index = 0;
            }

            deleteElements[index++]++;
        }

        int countGaps(std::vector<int> operation) {
            int gapCount = 0;
            for (int e : operation) {
                gapCount += ( e == 0 ? 1 : 0 );
            }
            return gapCount;
        }

        int sumElements(std::vector<int> operation) {
            int sum = 0;
            for (int e : operation) {
                sum += e;
            }
            return sum;
        }

        void report() {
            int gaps, sum;
            cout << "Insert: " << countGaps(insertElements) << " gaps, summing " << sumElements(insertElements) << ";" << endl;
            cout << "Select: " << countGaps(selectElements) << " gaps, summing " << sumElements(selectElements) << ";" << endl;
            cout << "Update: " << countGaps(updateElements) << " gaps, summing " << sumElements(updateElements) << ";" << endl;
            cout << "Delete: " << countGaps(deleteElements) << " gaps, summing " << sumElements(deleteElements) << ";" << endl;
        }
    };
    ReentrancyExperiments reentrancyExperiments = ReentrancyExperiments();
    reentrancyExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, true);
    reentrancyExperiments.report();

    cout << "Memory Leak Tests (go check on top if the ram usage is constant over time): " << flush;
    for (int i=0; i<5; i++) {
        if (i%76 == 0) {
            cout << endl;
        }
        reentrancyExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, false);
        cout << '.' << flush;
    }
    cout << endl << endl << flush;

#undef _numberOfElements
#undef _threads
}

int main() {

    try {
        databaseAlgorithmAnalysisLowLevelExperiments();
    } catch (const std::exception& e) {
        DUMP_EXCEPTION(e, "Error while running databaseAlgorithmAnalysisLowLevelExperiments()");
    }

    try {
        databaseAlgorithmAnalysisHighLevelExperiments();
    } catch (const std::exception& e) {
        DUMP_EXCEPTION(e, "Error while running databaseAlgorithmAnalysisHighLevelExperiments()");
    }

    return EXIT_SUCCESS;
}
