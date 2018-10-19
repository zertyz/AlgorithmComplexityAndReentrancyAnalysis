#include <iostream>
#include <string>
#include <math.h>
#include <thread>
#include <mutex>

#include <SplitRun.h>
#include "AlgorithmComplexityAndReentrancyAnalysis.h"
using namespace mutua::testutils;

#include <BetterExceptions.h>
#include <TimeMeasurements.h>
using namespace mutua::cpputils;

using namespace std;


string AlgorithmComplexityAndReentrancyAnalysis::
		EAlgorithmComplexityToString(EAlgorithmComplexity complexity) {

    switch (complexity) {
        case EAlgorithmComplexity::BetterThanO1:
            return "Better than O(1) -- aren't the machines idle or is there too little RAM?"s;
        case EAlgorithmComplexity::O1:
            return "O(1)"s;
        case EAlgorithmComplexity::Ologn:
            return "O(log(n))"s;
        case EAlgorithmComplexity::BetweenOLogNAndOn:
            return "Worse than O(log(n)) but better than O(n)"s;
        case EAlgorithmComplexity::On:
            return "O(n)"s;
        case EAlgorithmComplexity::WorseThanOn:
            return "Worse than O(n)"s;
        default:
            return "unpredicted algorithm complexity -- update 'DatabaseAlgorithmAnalysis' source code to account for such case"s;
    }
}


AlgorithmComplexityAndReentrancyAnalysis::
        AlgorithmComplexityAndReentrancyAnalysis(string testName, int numberOfInsertElements, int numberOfSelectElements, int numberOfUpdateElements)
            : testName                (testName)
            , inserts                 (numberOfInsertElements)
			, selects                 (numberOfSelectElements)
            , updates                 (numberOfUpdateElements)
            , deletes                 (numberOfInsertElements) {}


AlgorithmComplexityAndReentrancyAnalysis::
        AlgorithmComplexityAndReentrancyAnalysis(string testName, int elements)
            : AlgorithmComplexityAndReentrancyAnalysis(testName, elements, elements, elements) {}


// complexity analysis & reentrancy algorithms under test
/////////////////////////////////////////////////////////


void AlgorithmComplexityAndReentrancyAnalysis::insertAlgorithm(unsigned int i) {
    THROW_EXCEPTION(std::runtime_error, "If you want your algorithm analysis & reentrancy tests instance to validate INSERTs, you must override 'insertAlgorithm'");
}


void AlgorithmComplexityAndReentrancyAnalysis::selectAlgorithm(unsigned int i) {
    THROW_EXCEPTION(std::runtime_error, "If you want your algorithm analysis & reentrancy tests instance to validate SELECTs, you must override 'selectAlgorithm'");
}


void AlgorithmComplexityAndReentrancyAnalysis::updateAlgorithm(unsigned int i) {
    THROW_EXCEPTION(std::runtime_error, "If you want your algorithm analysis & reentrancy tests instance to validate UPDATEs, you must override 'updateAlgorithm'");
}


void AlgorithmComplexityAndReentrancyAnalysis::deleteAlgorithm(unsigned int i) {
    THROW_EXCEPTION(std::runtime_error, "If you want your algorithm analysis & reentrancy tests instance to validate DELETEs, you must override 'deleteAlgorithm'");
}


class AlgorithmAnalysisSplitRun: public SplitRun {
public:
    const int                                 threadNumber;
    const int                                 perThreadNumberOfOperations;
    AlgorithmComplexityAndReentrancyAnalysis* algorithms;
    const int                                 pass;
    const int                                 perPassNumberOfElements;

    AlgorithmAnalysisSplitRun(int threadNumber, int perThreadNumberOfOperations, AlgorithmComplexityAndReentrancyAnalysis* algorithms, int pass, int perPassNumberOfElements)
            : SplitRun(threadNumber)
            , threadNumber               (threadNumber)
            , perThreadNumberOfOperations(perThreadNumberOfOperations)
            , algorithms                 (algorithms)
            , pass                       (pass)
            , perPassNumberOfElements    (perPassNumberOfElements) {}
};

/** Warm up tasks inserts 1% of the total inserts */
class WarmUpSplitRun: public AlgorithmAnalysisSplitRun {
public:
    WarmUpSplitRun(int threadNumber, int perThreadNumberOfOperations, AlgorithmComplexityAndReentrancyAnalysis* algorithms)
        : AlgorithmAnalysisSplitRun(threadNumber, perThreadNumberOfOperations, algorithms, -1, -1) {}

    void splitRun() override {
        for (unsigned int i=perThreadNumberOfOperations*threadNumber; i<perThreadNumberOfOperations*(threadNumber+1)/100; i++) {
            algorithms->insertAlgorithm(i);
        }
    }
};

class InsertSplitRun: public AlgorithmAnalysisSplitRun {
public:
    InsertSplitRun(int threadNumber, int perThreadNumberOfOperations, AlgorithmComplexityAndReentrancyAnalysis* algorithms, int pass, int perPassNumberOfElements)
            : AlgorithmAnalysisSplitRun(threadNumber, perThreadNumberOfOperations, algorithms, pass, perPassNumberOfElements) {}

    void splitRun() override {
        for (unsigned int i=((perPassNumberOfElements*(pass-1))+(perThreadNumberOfOperations*threadNumber)); i<((perPassNumberOfElements*(pass-1))+(perThreadNumberOfOperations*(threadNumber+1))); i++) {
            algorithms->insertAlgorithm(i);
        }
    }
};

class SelectSplitRun: public AlgorithmAnalysisSplitRun {
public:
    SelectSplitRun(int threadNumber, int perThreadNumberOfOperations, AlgorithmComplexityAndReentrancyAnalysis* algorithms, int pass, int perPassNumberOfElements)
            : AlgorithmAnalysisSplitRun(threadNumber, perThreadNumberOfOperations, algorithms, pass, perPassNumberOfElements) {}

    void splitRun() override {
        for (unsigned int i=((perPassNumberOfElements*(pass-1))+(perThreadNumberOfOperations*threadNumber)); i<((perPassNumberOfElements*(pass-1))+(perThreadNumberOfOperations*(threadNumber+1))); i++) {
            algorithms->selectAlgorithm(i);
        }
    }
};

class UpdateSplitRun: public AlgorithmAnalysisSplitRun {
public:
    UpdateSplitRun(int threadNumber, int perThreadNumberOfOperations, AlgorithmComplexityAndReentrancyAnalysis* algorithms, int pass, int perPassNumberOfElements)
            : AlgorithmAnalysisSplitRun(threadNumber, perThreadNumberOfOperations, algorithms, pass, perPassNumberOfElements) {}

    void splitRun() override {
        for (unsigned int i=((perPassNumberOfElements*(pass-1))+(perThreadNumberOfOperations*threadNumber)); i<((perPassNumberOfElements*(pass-1))+(perThreadNumberOfOperations*(threadNumber+1))); i++) {
            algorithms->updateAlgorithm(i);
        }
    }
};

class DeleteSplitRun: public AlgorithmAnalysisSplitRun {
public:
    DeleteSplitRun(int threadNumber, int perThreadNumberOfOperations, AlgorithmComplexityAndReentrancyAnalysis* algorithms, int pass, int perPassNumberOfElements)
            : AlgorithmAnalysisSplitRun(threadNumber, perThreadNumberOfOperations, algorithms, pass, perPassNumberOfElements) {}

    void splitRun() override {
        for (unsigned int i=((perPassNumberOfElements*(pass-1))+(perThreadNumberOfOperations*threadNumber)); i<((perPassNumberOfElements*(pass-1))+(perThreadNumberOfOperations*(threadNumber+1))); i++) {
            algorithms->deleteAlgorithm(i);
        }
    }
};

tuple<
      tuple<AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity, unsigned long long, unsigned long long, vector<string>, vector<string>, vector<string>, vector<string>>,      // INSERTs
      tuple<AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity, unsigned long long, unsigned long long, vector<string>, vector<string>, vector<string>, vector<string>>,      // SELECTs
      tuple<AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity, unsigned long long, unsigned long long, vector<string>, vector<string>, vector<string>, vector<string>>,      // UPDATEs
      tuple<AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity, unsigned long long, unsigned long long, vector<string>, vector<string>, vector<string>, vector<string>>       // DELETEs
> AlgorithmComplexityAndReentrancyAnalysis::
        analyseComplexity(bool performWarmUp, int insertThreads, int selectThreads, int updateThreads, int deleteThreads, bool verbose) {

    constexpr int        numberOfPasses = 2;
    unsigned int         perThreadInserts = inserts / numberOfPasses / insertThreads;
    unsigned int         perThreadSelects = selects / numberOfPasses / selectThreads;
    unsigned int         perThreadUpdates = updates / numberOfPasses / updateThreads;
    unsigned int         perThreadDeletes = deletes / numberOfPasses / deleteThreads;
    unsigned int         numberOfFirstPassInsertElements = inserts / numberOfPasses, numberOfSecondPassInsertElements = inserts;
    unsigned int         numberOfFirstPassSelectElements = selects / numberOfPasses, numberOfSecondPassSelectElements = selects;
    unsigned int         numberOfFirstPassUpdateElements = updates / numberOfPasses, numberOfSecondPassUpdateElements = updates;
    unsigned int         numberOfFirstPassDeleteElements = deletes / numberOfPasses, numberOfSecondPassDeleteElements = deletes;
    unsigned long long   insertStart[numberOfPasses], insertEnd[numberOfPasses];
    unsigned long long   selectStart[numberOfPasses], selectEnd[numberOfPasses];
    unsigned long long   updateStart[numberOfPasses], updateEnd[numberOfPasses];
    unsigned long long   deleteStart[numberOfPasses], deleteEnd[numberOfPasses];
    vector<string>       insertExceptions[numberOfPasses], insertExceptionReportMessages[numberOfPasses];
    vector<string>       selectExceptions[numberOfPasses], selectExceptionReportMessages[numberOfPasses];
    vector<string>       updateExceptions[numberOfPasses], updateExceptionReportMessages[numberOfPasses];
    vector<string>       deleteExceptions[numberOfPasses], deleteExceptionReportMessages[numberOfPasses];
    EAlgorithmComplexity insertComplexity;
    EAlgorithmComplexity selectComplexity;
    EAlgorithmComplexity updateComplexity;
    EAlgorithmComplexity deleteComplexity;
    string               algorithmAnalisysReport;

    if (verbose) cerr << testName << " Algorithm Complexity Analysis: " << flush;

    // WARMUP
    if (performWarmUp) {
        if (verbose) cerr << "Warm" << flush;
        resetTables(EResetOccasion::PRE_WARMUP_RESET);
        if (verbose) cerr << " Up; " << flush;
        std::vector<unique_ptr<WarmUpSplitRun>> warmUpSplitRunInstances(insertThreads);
        for (int threadNumber=0; threadNumber<insertThreads; threadNumber++) {
            warmUpSplitRunInstances[threadNumber] = unique_ptr<WarmUpSplitRun>(new WarmUpSplitRun(threadNumber, perThreadInserts, this));
            SplitRun::add(*warmUpSplitRunInstances[threadNumber]);
        }
        SplitRun::runAndWaitForAll();
    }

    resetTables(EResetOccasion::FULL_RESET);

    // insert / select / select passes
    //////////////////////////////////

    for (int pass=1; pass<=numberOfPasses; pass++) {

        if (pass == 1) {
            if (verbose) cerr << "First Pass ( " << flush;
        } else if (pass == 2) {
            if (verbose) cerr << "); Second Pass ( " << flush;
        } else {
            THROW_EXCEPTION(std::runtime_error, __FILE__ " was not prepared for pass #" + std::to_string(pass) + " while inserting / updating / selecting");
        }

        // INSERTS
        if (insertThreads > 0) {
            if (verbose) cerr << "Insert " << flush;
            std::vector<unique_ptr<InsertSplitRun>> insertSplitRunInstances(insertThreads);
            for (int threadNumber=0; threadNumber<insertThreads; threadNumber++) {
                insertSplitRunInstances[threadNumber] = unique_ptr<InsertSplitRun>(new InsertSplitRun(threadNumber, perThreadInserts, this, pass, numberOfFirstPassInsertElements));
                SplitRun::add(*insertSplitRunInstances[threadNumber]);
            }
            insertStart[pass-1] = TimeMeasurements::getMonotonicRealTimeUS();
            tie(insertExceptions[pass-1], insertExceptionReportMessages[pass-1]) = SplitRun::runAndWaitForAll();
            insertEnd[pass-1] = TimeMeasurements::getMonotonicRealTimeUS();
        }

        // SELECTS
        if (selectThreads > 0) {
            if (verbose) cerr << "Select " << flush;
            std::vector<unique_ptr<SelectSplitRun>> selectSplitRunInstances(selectThreads);
            for (int threadNumber=0; threadNumber<selectThreads; threadNumber++) {
                selectSplitRunInstances[threadNumber] = unique_ptr<SelectSplitRun>(new SelectSplitRun(threadNumber, perThreadSelects, this, pass, numberOfFirstPassSelectElements));
                SplitRun::add(*selectSplitRunInstances[threadNumber]);
            }
            selectStart[pass-1] = TimeMeasurements::getMonotonicRealTimeUS();
            tie(selectExceptions[pass-1], selectExceptionReportMessages[pass-1]) = SplitRun::runAndWaitForAll();
            selectEnd[pass-1] = TimeMeasurements::getMonotonicRealTimeUS();
        }

        // UPDATES
        if (updateThreads > 0) {
            if (verbose) cerr << "Update " << flush;
            std::vector<unique_ptr<UpdateSplitRun>> updateSplitRunInstances(updateThreads);
            for (int threadNumber=0; threadNumber<updateThreads; threadNumber++) {
                updateSplitRunInstances[threadNumber] = unique_ptr<UpdateSplitRun>(new UpdateSplitRun(threadNumber, perThreadUpdates, this, pass, numberOfFirstPassUpdateElements));
                SplitRun::add(*updateSplitRunInstances[threadNumber]);
            }
            updateStart[pass-1] = TimeMeasurements::getMonotonicRealTimeUS();
            tie(updateExceptions[pass-1], updateExceptionReportMessages[pass-1]) = SplitRun::runAndWaitForAll();
            updateEnd[pass-1] = TimeMeasurements::getMonotonicRealTimeUS();
        }
    }

    if (verbose) cerr << ")" << flush;

    // delete passes
    ////////////////

    if (deleteThreads > 0) {

        if (verbose) cerr << "; Delete ( " << flush;

        for (int pass=numberOfPasses; pass>=1; pass--) {

            if (pass == 1) {
                if (verbose) cerr << "First Pass " << flush;
            } else if (pass == 2) {
                if (verbose) cerr << "Second Pass " << flush;
            } else {
                THROW_EXCEPTION(std::runtime_error, __FILE__  " was not prepared for pass #" + std::to_string(pass) + " while deleting");
            }

            // DELETES
            std::vector<unique_ptr<DeleteSplitRun>> deleteSplitRunInstances(deleteThreads);
            for (int threadNumber=0; threadNumber<deleteThreads; threadNumber++) {
                deleteSplitRunInstances[threadNumber] = unique_ptr<DeleteSplitRun>(new DeleteSplitRun(threadNumber, perThreadDeletes, this, pass, numberOfFirstPassDeleteElements));
                SplitRun::add(*deleteSplitRunInstances[threadNumber]);
            }
            deleteStart[pass-1] = TimeMeasurements::getMonotonicRealTimeUS();
            tie(deleteExceptions[pass-1], deleteExceptionReportMessages[pass-1]) = SplitRun::runAndWaitForAll();
            deleteEnd[pass-1] = TimeMeasurements::getMonotonicRealTimeUS();

        }

        if (verbose) cerr << ")" << flush;

    }

    if (verbose) cerr << "." << endl << flush;

    if (insertThreads > 0) {
        tie(insertComplexity, algorithmAnalisysReport) = computeInsertOrDeleteAlgorithmAnalysis("Insert",
                                                                                                insertStart[0], insertEnd[0],
                                                                                                insertStart[1], insertEnd[1], inserts/2);
        if (verbose) cerr << algorithmAnalisysReport << endl << flush;
    }
    if (selectThreads > 0) {
        tie(selectComplexity, algorithmAnalisysReport) = computeSelectOrUpdateAlgorithmAnalysis("Select",
                                                                                                selectStart[0], selectEnd[0],
                                                                                                selectStart[1], selectEnd[1],
                                                                                                numberOfFirstPassSelectElements, numberOfSecondPassSelectElements, selects);
        if (verbose) cerr << algorithmAnalisysReport << endl << flush;
    }
    if (updateThreads > 0) {
        tie(updateComplexity, algorithmAnalisysReport) = computeSelectOrUpdateAlgorithmAnalysis("Update",
                                                                                                updateStart[0], updateEnd[0],
                                                                                                updateStart[1], updateEnd[1],
                                                                                                numberOfFirstPassUpdateElements, numberOfSecondPassUpdateElements, updates);
        if (verbose) cerr << algorithmAnalisysReport << endl << flush;
    }
    if (deleteThreads > 0) {
        tie(deleteComplexity, algorithmAnalisysReport) = computeInsertOrDeleteAlgorithmAnalysis("Delete",
                                                                                                deleteStart[0], deleteEnd[0],
                                                                                                deleteStart[1], deleteEnd[1], deletes/2);
        if (verbose) cerr << algorithmAnalisysReport << endl << flush;
    }

    resetTables(EResetOccasion::FINAL_RESET);

    return {
        {insertComplexity, insertEnd[0]-insertStart[0], insertEnd[1]-insertStart[1], insertExceptions[0], insertExceptions[1], insertExceptionReportMessages[0], insertExceptionReportMessages[1]},     // INSERTs
        {selectComplexity, selectEnd[0]-selectStart[0], selectEnd[1]-selectStart[1], selectExceptions[0], selectExceptions[1], selectExceptionReportMessages[0], selectExceptionReportMessages[1]},     // SELECTs
        {updateComplexity, updateEnd[0]-updateStart[0], updateEnd[1]-updateStart[1], updateExceptions[0], updateExceptions[1], updateExceptionReportMessages[0], updateExceptionReportMessages[1]},     // UPDATEs
        {deleteComplexity, deleteEnd[0]-deleteStart[0], deleteEnd[1]-deleteStart[1], deleteExceptions[0], deleteExceptions[1], deleteExceptionReportMessages[0], deleteExceptionReportMessages[1]}      // DELETEs
    };
}


class ReentrancySplitRunTest: public SplitRun {
public:
    AlgorithmComplexityAndReentrancyAnalysis* algorithms;
    int                                       op;           // {operations: 0: insert, 1: select, 2: update, 3: delete}
    std::mutex                                opGuard;
    unsigned int                              numberOfElements;
    unsigned int                              verbosityFactor;
    unsigned int                              insertIndex;
    unsigned int                              selectIndex;
    unsigned int                              updateIndex;
    unsigned int                              deleteIndex;
    unsigned long long int                    timeusSpentInserting;
    unsigned long long int                    timeusSpentTestingInsertsAndSelecting;
    unsigned long long int                    timeusSpentUpdating;
    unsigned long long int                    timeusSpentTestingUpdatesAndDeleting;
    string&                                   testOutput;

    ReentrancySplitRunTest(AlgorithmComplexityAndReentrancyAnalysis* algorithms, unsigned int numberOfElements, unsigned int verbosityFactor, string& testOutput)
            : SplitRun(-1)
            , algorithms(algorithms)
            , numberOfElements(numberOfElements)
            , verbosityFactor(verbosityFactor)
            , op(0)
            , timeusSpentInserting                 (0ull)
            , timeusSpentTestingInsertsAndSelecting(0ull)
            , timeusSpentUpdating                  (0ull)
            , timeusSpentTestingUpdatesAndDeleting (0ull)
    		, testOutput(testOutput) {}

    void output(const char* op) {
    	testOutput.append(op);
    	cerr << op << flush;
    }

    void splitRun() override {
        opGuard.lock();
        int operation = op++;
        opGuard.unlock();
        if (operation  == 0) {                    // INSERT
            for (insertIndex=0; insertIndex<numberOfElements; insertIndex++) {
                if (verbosityFactor && (insertIndex % verbosityFactor == 0)) output("I");
                unsigned long long int start  = TimeMeasurements::getMonotonicRealTimeUS();
                algorithms->insertAlgorithm(insertIndex);
                unsigned long long int finish = TimeMeasurements::getMonotonicRealTimeUS();
                timeusSpentInserting += finish-start;
            }
        } else if (operation == 1) {             // SELECT
            for (selectIndex=0; selectIndex<numberOfElements; selectIndex++) {
                while (selectIndex >= insertIndex) this_thread::sleep_for(chrono::milliseconds(100));
                if (verbosityFactor && (selectIndex % verbosityFactor == 0)) output("S");
                unsigned long long start  = TimeMeasurements::getMonotonicRealTimeUS();
                algorithms->selectAlgorithm(selectIndex);
                unsigned long long finish = TimeMeasurements::getMonotonicRealTimeUS();
                timeusSpentTestingInsertsAndSelecting += finish-start;
            }
        } else if (operation == 2) {             // UPDATE
            for (updateIndex=0; updateIndex<numberOfElements; updateIndex++) {
                while (updateIndex >= selectIndex) this_thread::sleep_for(chrono::milliseconds(100));
                if (verbosityFactor && (updateIndex % verbosityFactor == 0)) output("U");
                unsigned long long start  = TimeMeasurements::getMonotonicRealTimeUS();
                algorithms->updateAlgorithm(updateIndex);
                unsigned long long finish = TimeMeasurements::getMonotonicRealTimeUS();
                timeusSpentUpdating += finish-start;
            }
        } else if (operation == 3) {             // DELETE
            for (deleteIndex=0; deleteIndex<numberOfElements; deleteIndex++) {
                while (deleteIndex >= updateIndex) this_thread::sleep_for(chrono::milliseconds(100));
                if (verbosityFactor && (deleteIndex % verbosityFactor == 0)) output("D");
                unsigned long long start  = TimeMeasurements::getMonotonicRealTimeUS();
                algorithms->deleteAlgorithm(deleteIndex);
                unsigned long long finish = TimeMeasurements::getMonotonicRealTimeUS();
                timeusSpentTestingUpdatesAndDeleting += finish-start;
            }
        } else {
            THROW_EXCEPTION(std::runtime_error, "unknown operation #" + to_string(operation));
        }
    }
};

#define OUTPUT(s) testOutput.append(s); if (verbose) cerr << s << flush
string AlgorithmComplexityAndReentrancyAnalysis::
		testReentrancy(unsigned int numberOfElements, bool verbose) {

    string testOutput = "";
	OUTPUT(testName + " Algorithm Reentrancy Tests");
    resetTables(EResetOccasion::FULL_RESET);
    OUTPUT(": ");

    ReentrancySplitRunTest reentrancyTest(this, numberOfElements, verbose ? numberOfElements/4 : 0, testOutput);

    // prepare the simultaneous tasks
    //for (unsigned int threadSet=0; threadSet<numberOfThreadSets; threadSet++) {
        SplitRun::add(reentrancyTest);  // INSERT
        SplitRun::add(reentrancyTest);  // SELECT
        SplitRun::add(reentrancyTest);  // UPDATE
        SplitRun::add(reentrancyTest);  // DELETE
    //}

    SplitRun::runAndWaitForAll();

    OUTPUT(" Done: ");
    resetTables(EResetOccasion::FINAL_RESET);
    OUTPUT(to_string(reentrancyTest.timeusSpentInserting                  / 1000llu) + "ms inserting, " +
           to_string(reentrancyTest.timeusSpentTestingInsertsAndSelecting / 1000llu) + "ms selecting & testing, " +
		   to_string(reentrancyTest.timeusSpentUpdating                   / 1000llu) + "ms updating, " +
		   to_string(reentrancyTest.timeusSpentTestingUpdatesAndDeleting  / 1000llu) + "ms testing & deleting.\n");

    return testOutput;
}
#undef OUTPUT


#define lPAD12(n) std::to_string(n) + ((n)>99999999999 ? "" : ((n)>9999999999 ? " " : ((n)>999999999 ? "  " : ((n)>99999999 ? "   " : ((n)>9999999 ? "    " : ((n)>999999 ? "     " : ((n)>99999 ? "      " : ((n)>9999 ? "       " : ((n)>999 ? "        " : ((n)>99 ? "         " : ((n)>9 ? "          " : "           ")))))))))))

std::tuple<AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity, string> AlgorithmComplexityAndReentrancyAnalysis::
    	computeSelectOrUpdateAlgorithmAnalysis(
    			const string&            operation,
				const unsigned long int& start1,
				const unsigned long int& end1,
				const unsigned long int& start2,
				const unsigned long int& end2,
				const unsigned int       n1,
				const unsigned int       n2,
				const unsigned int       r) {

    // the acceptable percent measurement error when computing complexity
    double percentError = 0.10;

    unsigned long int deltaT1 = end1 - start1;
    unsigned long int deltaT2 = end2 - start2;
    double t1 = ((double)deltaT1) / ((double)r);
    double t2 = ((double)deltaT2) / ((double)r);

    EAlgorithmComplexity complexity;

    // sanity check
    if (((t1/t2) - 1.0) >= percentError) {
        complexity = EAlgorithmComplexity::BetterThanO1;
    } else
        // test for O(1) -- t2/t1 ~= 1
    if ( abs((t2/t1) - 1.0) < percentError ) {
        complexity = EAlgorithmComplexity::O1;
    } else
        // test for O(log(n)) -- (t2/t1) / (log(n2)/log(n1)) ~= 1
    if ( abs( ((t2/t1) / (log(n2)/log(n1))) - 1.0 ) < percentError ) {
        complexity = EAlgorithmComplexity::Ologn;
    } else
        // test for O(n) -- (t2/t1) / (n2/n1) ~= 1
    if ( abs( ((t2/t1) / (n2/n1)) - 1.0 ) < percentError ) {
        complexity = EAlgorithmComplexity::On;
    } else
        // test for worse than O(n)
    if ( ( ((t2/t1) / (n2/n1)) - 1.0 ) > percentError ) {
        complexity = EAlgorithmComplexity::WorseThanOn;
    } else {
        complexity = EAlgorithmComplexity::BetweenOLogNAndOn;
    }

    string algorithmAnalysisReport = operation + " algorithm analysis:\n" +
                                     "    (^t)            n              r               t(1)\n" +
                                     "1:  "  + lPAD12(deltaT1) + "\t" + lPAD12(n1) + "\t" + lPAD12(r) + "\t" + std::to_string(t1) + "\n" +
                                     "2:  "  + lPAD12(deltaT2) + "\t" + lPAD12(n2) + "\t" + lPAD12(r) + "\t" + std::to_string(t2) + "\n" +
                                     "--> " + EAlgorithmComplexityToString(complexity) + "\n";

    return {complexity, algorithmAnalysisReport};
}

tuple<AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity, string> AlgorithmComplexityAndReentrancyAnalysis::
    	computeInsertOrDeleteAlgorithmAnalysis(
    			const string&            operation,
				const unsigned long int& start1,
				const unsigned long int& end1,
				const unsigned long int& start2,
				const unsigned long int& end2,
				const unsigned int       n) {

    // the acceptable percent measurement error when computing complexity
    double percentError = 0.10;

    unsigned long int deltaT1 = end1 - start1;
    unsigned long int deltaT2 = end2 - start2;
    double t1 = ((double)deltaT1) / ((double)n);
    double t2 = ((double)deltaT2) / ((double)n);

    EAlgorithmComplexity complexity;

    // sanity check
    if (((t1/t2) - 1.0) >= percentError) {
        complexity = EAlgorithmComplexity::BetterThanO1;
    } else
        // test for O(1) -- t2/t1 ~= 1
    if ( abs((t2/t1) - 1.0) < percentError ) {
        complexity = EAlgorithmComplexity::O1;
    } else
        // test for O(log(n)) -- (t2/t1) / (log(n*3)/log(n)) ~= 1
    if ( abs( ((t2/t1) / (log(n*3)/log(n))) - 1.0 ) < percentError ) {
        complexity = EAlgorithmComplexity::Ologn;
    } else
        // test for O(n) -- t2/t1 / 3 ~= 1
    if ( abs( ((t2/t1)/3) - 1.0 ) < percentError ) {
        complexity = EAlgorithmComplexity::On;
    } else
        // test for worse than O(n)
    if ( ( ((t2/t1)/3) - 1.0 ) > percentError ) {
        complexity = EAlgorithmComplexity::WorseThanOn;
    } else {
        complexity = EAlgorithmComplexity::BetweenOLogNAndOn;
    }

    string algorithmAnalysisReport = operation + " algorithm analysis:\n"s +
                                     "    (^t)            n              t(1)\n"s +
                                     "1:  "  + lPAD12(deltaT1) + "\t" + lPAD12(n)   + "\t" + std::to_string(t1) + "\n" +
                                     "2:  "  + lPAD12(deltaT2) + "\t" + lPAD12(n*2) + "\t" + std::to_string(t2) + "\n" +
                                     "--> " + EAlgorithmComplexityToString(complexity) + "\n";

    return {complexity, algorithmAnalysisReport};

}
