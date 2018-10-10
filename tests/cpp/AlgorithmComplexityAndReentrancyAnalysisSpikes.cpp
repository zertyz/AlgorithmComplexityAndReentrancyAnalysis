#include <iostream>
#include <map>
#include <unordered_map>
#include <thread>
#include <random>
#include <algorithm>

#include <mutex>

using namespace std;

#include <TimeMeasurements.h>
using namespace mutua::cpputils;

#include "../../cpp/AlgorithmComplexityAndReentrancyAnalysis.h"
#include <SplitRun.h>
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


// select functions
///////////////////
// 'n' should be the total elements on each pass
// note: returning 'r' is needed in order for compiler optimizations not avoid looping

unsigned int O1Select(unsigned int n) {
    unsigned int r = 0;
    for (unsigned int i=0; i<DELAY_CONSTANT; i++) r ^= i;
    return r;
}

unsigned int OLogSelect(unsigned int n) {
    unsigned int r = 0;
    while (n > 0) {
        for (unsigned int i=0; i<DELAY_CONSTANT; i++) r ^= i;
        n /= 2;
    }
    return r;
}

unsigned int ONSelect(unsigned int n) {
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
    cout << "If Selecting/Updating in O(1), I'd get: " << endl;
    tie(complexity, algorithmAnalysisReport) = AlgorithmComplexityAndReentrancyAnalysis::computeSelectOrUpdateAlgorithmAnalysis("Update/Select Test"s, 100, 200, 200, 300, 1000, 2000, 1000);
    cout << algorithmAnalysisReport << flush;
    cout << "If Selecting/Updating in O(log(n)), I'd get: " << endl;
    tie(complexity, algorithmAnalysisReport) = AlgorithmComplexityAndReentrancyAnalysis::computeSelectOrUpdateAlgorithmAnalysis("Update/Select Test"s, 100, 200, 200, 311, 1000, 2000, 1000);
    cout << algorithmAnalysisReport << flush;

    AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity lastSelectOrUpdateComplexity = AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity::BetterThanO1;
    AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity currentSelectOrUpdateComplexity;
    for (unsigned long endMillis = 300; endMillis < 10000; endMillis++) {
        tie(currentSelectOrUpdateComplexity, algorithmAnalysisReport) = AlgorithmComplexityAndReentrancyAnalysis::computeSelectOrUpdateAlgorithmAnalysis("Insertion Test"s, 100, 200, 200, endMillis, 1000, 2000, 1000);
        if (currentSelectOrUpdateComplexity != lastSelectOrUpdateComplexity) {
            cout << "Select/Update test started measuring " << AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexityToString(currentSelectOrUpdateComplexity) << " at endMillis " << endMillis << endl;
            lastSelectOrUpdateComplexity = currentSelectOrUpdateComplexity;
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

#define TEST_SELECT(testName, selectFunction)                                                                                                                       \
    n = 1000;                                                                                                                                                       \
    cout << "Real " << testName << " with " << n << " elements on each pass" << flush;                                                                              \
    start1US = TimeMeasurements::getMonotonicRealTimeUS();                                                                                                          \
    for (unsigned int e=0; e<n; e++) {                                                                                                                              \
        r += selectFunction(n);                                                                                                                                     \
    }                                                                                                                                                               \
    end1US   = TimeMeasurements::getMonotonicRealTimeUS();                                                                                                          \
    delta1US = end1US - start1US;                                                                                                                                   \
    cout << " (pass1 = " << (delta1US) << "µs" << flush;                                                                                                            \
    start2US = TimeMeasurements::getMonotonicRealTimeUS();                                                                                                          \
    for (unsigned int e=n; e<n*2; e++) {                                                                                                                            \
        r += selectFunction(n*2);                                                                                                                                   \
    }                                                                                                                                                               \
    end2US   = TimeMeasurements::getMonotonicRealTimeUS();                                                                                                          \
    delta2US = end2US - start2US;                                                                                                                                   \
    cout << "; pass2 = " << (delta2US) << "µs): " << endl << flush;                                                                                                 \
    tie(complexity, algorithmAnalysisReport) = AlgorithmComplexityAndReentrancyAnalysis::computeSelectOrUpdateAlgorithmAnalysis("Select/Update", start1US, end1US, start2US, end2US, n, n*2, n); \
    cerr << algorithmAnalysisReport << "--> (computation result is " << r << ")" << endl << flush;

    TEST_INSERT("O(1) Insertion Test", O1Insert);
    TEST_INSERT("O(log(n)) Insertion Test", OLogNInsert);
    TEST_INSERT("O(n) Insertion Test", ONInsert);

    TEST_SELECT("O(1) Select Test",      O1Select);
    TEST_SELECT("O(log(n)) Select Test", OLogSelect);
    TEST_SELECT("O(n) Select Test",      ONSelect);
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
        std::mutex  writeGuard;
        std::mutex* readGuard;

        HelloDatabaseAlgorithmAnalysisWorld()
                : AlgorithmComplexityAndReentrancyAnalysis("Hello, Database Algorithm Analysis World!!", 2000, 2000, 2000)
                , readGuard(nullptr) {
            elements = std::vector<int>(2000);
        }

        void resetTables(EResetOccasion occasion) override {
            elements.clear();
        }

        void insertAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            elements[i] = i;
            readGuard = nullptr;
        }

        void selectAlgorithm(unsigned int i) override {
        	if (readGuard != nullptr) std::lock_guard<std::mutex> lock(*readGuard);
            if (elements[i] != ((int)i)) {
                cerr << "Select: item #" << i << ", on the insert phase, should be " << ((int)i) << " but is " << elements[i] << endl << flush;
            }
        }

        void updateAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            elements[i] = -((int)i);
            readGuard = nullptr;
        }

        void deleteAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
        	int value = elements[i];
            elements[i] -1;
            if (value != -((int)i)) {
                cerr << "Delete: item #" << i << ", on the update phase, should be " << -((int)i) << " but was " << value << endl << flush;
            }
            readGuard = nullptr;
        }
    };
    HelloDatabaseAlgorithmAnalysisWorld().analyseComplexity(true, 4, 4, 4, 4, true);
    HelloDatabaseAlgorithmAnalysisWorld().testReentrancy(2000, true);

    class ReentrancyExperiments: public AlgorithmComplexityAndReentrancyAnalysis {
    public:
        std::vector<int> insertElements;
        std::vector<int> selectElements;
        std::vector<int> updateElements;
        std::vector<int> deleteElements;

        ReentrancyExperiments()
                : AlgorithmComplexityAndReentrancyAnalysis("ReentrancyExperiments", _numberOfElements) {
        }

        void resetTables(EResetOccasion occasion) override {
        	if (occasion == EResetOccasion::FULL_RESET) {
				insertElements = std::vector<int>(_numberOfElements, 0);
				selectElements = std::vector<int>(_numberOfElements, 0);
				updateElements = std::vector<int>(_numberOfElements, 0);
				deleteElements = std::vector<int>(_numberOfElements, 0);
        	}
        }

        void insertAlgorithm(unsigned int i) override {
            insertElements[i]++;
        }

        void selectAlgorithm(unsigned int i) override {
            selectElements[i]++;
        }

        void updateAlgorithm(unsigned int i) override {
            updateElements[i]++;
        }

        void deleteAlgorithm(unsigned int i) override {
            deleteElements[i]++;
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
    reentrancyExperiments.testReentrancy(_numberOfElements, true);
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
