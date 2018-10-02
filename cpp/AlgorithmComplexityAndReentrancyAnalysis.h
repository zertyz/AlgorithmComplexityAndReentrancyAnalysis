//#pragma once
#ifndef HASHTABLESPIKES_DATABASEALGORITHMANALYSIS_H
#define HASHTABLESPIKES_DATABASEALGORITHMANALYSIS_H

#include <string>
#include <tuple>
#include <vector>

using namespace std;

namespace mutua::testutils {

    /**
     * DatabaseAlgorithmAnalysis.cpp
     * =============================
     * created (in Java) by luiz, Jul 28, 2015, as DatabaseAlgorithmAnalysis.java
     * transcoded to C++ by luiz, Sep 18, 2018
     * last transcoding  by luiz, Sep 18, 2018
     *
     * Measures the complexity of (database) algorithms
     *
     *   AND
     *
     * Tests the reentrancy of such algorithms.
     *
     *
     * Complexity Analysis:
     * ===================
     *
     * Methodology: We will insert, update, select and delete in two passes, using the same amount of items each time.
     *              The algorithm complexity is measured on the time difference it takes to insert, update, select
     *              or delete each element on each pass. A little on Algorithm Complexity Analysis theory: Being n the
     *              number of elements on which the algorithm under analysis will work on, the algorithm is said to be:
     *              - O(1) complexity -- the operation times are independent on the number of elements already present;
     *              - O(n) complexity -- the operation times are linearly dependent on the number of elements;
     *              - O(log(n)) complexity -- operation duration times with log2(n) (base 2) proportionality.
     *              - O(n*log(n)) complexity -- operation duration times with n * log2(n) (base 2) proportionality;
     *              - O(n^2) complexity -- the operation times are quadratically dependent on the number of elements;
     * Notes:
     *  - We seek to build algorithms (and database queries) that are have O(1) complexity -- meaning the performance
     *    will not deteriorate over time, when the number of elements gets bigger and bigger;
     *  - This class will not test specifically for O(n*log(n)) and O(n^2), for they are so undesirable that we assume they
     *    will could only occur during the development phase. If those cases happen, we will only say "greater than O(log(n))".
     *
     * That being said, we're lead to the following usable formulas:
     *
     *   Variables:
     *     t(1)    -- time to insert / update / select / delete one element
     *     t<z>(1) -- time measured on pass z (remember we only do two passes)
     *
     * Inserts and Deletes of n elements (p threads):
     * O(1)       when  t2(1)/t1(1) / 1 ~= 1
     * O(n)       when  t2(1)/t1(1) / 3 ~= 1
     * O(log(n))  when  t2(1)/t1(1) / log(n*3)/log(n) ~= 1 +/- 10%
     *
     * Updates and Selects:
     * O(1)      when  t2 / t1                       ~= 1 for any p
     * O(n)      when  t2 / t1  /  (n2 / n1)         ~= 1 for any p
     * O(log n)  when  t2 / t1  /  log(n2) / log(n1) ~= 1 for any p
     *
     *
     * Reentrancy Analysis:
     * ===================
     *
     * Methodology: We will insert, update, select and delete (all at the same time) the same number of elements and
     *              by the same routines as in the algorithm complexity analysis phase, with the following constraints:
     *              - All operations are performed by their own threads -- therefore, we'll have multiples of 4 threads;
     *              - Insertions occur freely, at the maximum speed;
     *              - Selections occur only for already inserted elements;
     *              - Updates occur only for already selected elements;
     *              - Deletions occur only for already updated elements.
     * Notes:
     *  - The operation functions must test the data -- selects must check inserts, deletes must check updates. You may
     *    either print or throw an exception in case of errors;
    */
    class AlgorithmComplexityAndReentrancyAnalysis {

    private:
        const string testName;
        const int    inserts;
        const int    updates;
        const int    selects;
        const int    deletes;

    public:

        enum class EAlgorithmComplexity {
            BetterThanO1, O1, Ologn, BetweenOLogNAndOn, On, WorseThanOn
        };

        /** Returns an human explanation of {@link #EAlgorithmComplexity} */
        static string EAlgorithmComplexityToString(EAlgorithmComplexity complexity);


        /** Prepares for algorithm analysis & reentrancy test, with the given number of Inserts, Updates, Selects and Deletes */
        AlgorithmComplexityAndReentrancyAnalysis(string testName, int numberOfInsertElements, int numberOfUpdateElements, int numberOfSelectElements);

        /** Prepares for algorithm analysis & reentrancy test with the same number of elements for Inserts, Updates, Selects and Deletes */
        AlgorithmComplexityAndReentrancyAnalysis(string testName, int elements);


        enum class EResetOccasion {PRE_WARMUP_RESET, FULL_RESET, FINAL_RESET};
        virtual void resetTables(EResetOccasion occasion) = 0;

        // complexity analysis
        //////////////////////

        virtual void insertForComplexityAnalysis(unsigned int i);
        virtual void updateForComplexityAnalysis(unsigned int i);
        virtual void selectForComplexityAnalysis(unsigned int i);
        virtual void deleteForComplexityAnalysis(unsigned int i);

        /**
         * Returns :
         * {
         *     {EAlgorithmComplexity, (ull)pass1MicroS, (ull)pass2MicroS, (vector<string>:) pass1Exceptions, pass2Exceptions, pass1ExceptionReportMessages, pass2ExceptionReportMessages},       // INSERTs
         *     {EAlgorithmComplexity, (ull)pass1MicroS, (ull)pass2MicroS, (vector<string>:) pass1exceptions, pass2Exceptions, pass1ExceptionReportMessages, pass2ExceptionReportMessages},       // UPDATEs
         *     {EAlgorithmComplexity, (ull)pass1MicroS, (ull)pass2MicroS, (vector<string>:) pass1exceptions, pass2Exceptions, pass1ExceptionReportMessages, pass2ExceptionReportMessages},       // SELECTs
         *     {EAlgorithmComplexity, (ull)pass1MicroS, (ull)pass2MicroS, (vector<string>:) pass1exceptions, pass2Exceptions, pass1ExceptionReportMessages, pass2ExceptionReportMessages}        // DELETEs
         * }
         **/
        tuple<
              tuple<EAlgorithmComplexity, unsigned long long, unsigned long long, vector<string>, vector<string>, vector<string>, vector<string>>,      // INSERTs
              tuple<EAlgorithmComplexity, unsigned long long, unsigned long long, vector<string>, vector<string>, vector<string>, vector<string>>,      // UPDATEs
              tuple<EAlgorithmComplexity, unsigned long long, unsigned long long, vector<string>, vector<string>, vector<string>, vector<string>>,      // SELECTs
              tuple<EAlgorithmComplexity, unsigned long long, unsigned long long, vector<string>, vector<string>, vector<string>, vector<string>>       // DELETEs
        >
            analyseComplexity(bool performWarmUp, int insertThreads, int updateThreads, int selectThreads, int deleteThreads, bool verbose);

        // reentrancy tests
        ///////////////////

        virtual void insertForReentrancyTests(unsigned int i);
        virtual void selectForReentrancyTests(unsigned int i);
        virtual void updateForReentrancyTests(unsigned int i);
        virtual void deleteForReentrancyTests(unsigned int i);

        EAlgorithmComplexity testReentrancy(unsigned int numberOfElements, bool verbose);


        /** Performs the algorithm analysis for a reasonably large update/select operation (on a database or not).
          * To perform the analysis, two passes of updates/selects of r elements must be done.
          * On the first pass, the data set must have n1 elements and on the second pass, n2 elements -- n2 must be (at least?) twice n1.
          * r should be reasonably large so that end-start can be accurately measured and account for OS, IO and network latencies.
          * start 1 & 2 and end 1 & 2 are measurement times, regardless of the measurement unit -- milliseconds or microseconds.
          * The returned algorithm complexity is an indication of the time taken to update/select one element on a data set containing
          * n elements, where O is the constant of proportionality -- the average time to update/select 1 element.
          * Returns: [1] -- the algorithm complexity;
          *          [2] -- a string with the algorithm analysis report. */
        static std::tuple<EAlgorithmComplexity, string> computeUpdateOrSelectAlgorithmAnalysis(
                const string&            operation,
                const unsigned long int& start1,
                const unsigned long int& end1,
                const unsigned long int& start2,
                const unsigned long int& end2,
                const unsigned int       n1,
                const unsigned int       n2,
                const unsigned int       r);

        /** Performs the algorithm analysis for a reasonably large insert/delete operation (on a database or not).
          * To perform the analysis, two insertions passes of 'n' elements must be done on an empty data set (or database).
          * start 1 & 2 and end 1 & 2 are measurement times, regardless of the measurement unit -- milliseconds or microseconds.
          * The returned algorithm complexity is an indication of the time taken to insert/delete one element
          * on a data set containing n elements, where O is the constant of proportionality --
          * the average time to insert 1 element on an empty data set (or to delete 1 element on a 1 element data set).
          * Returns: [1] -- the algorithm complexity;
          *          [2] -- a string with the algorithm analysis report. */
        static std::tuple<AlgorithmComplexityAndReentrancyAnalysis::EAlgorithmComplexity, string> computeInsertOrDeleteAlgorithmAnalysis(
                const string&            operation,
                const unsigned long int& start1,
                const unsigned long int& end1,
                const unsigned long int& start2,
                const unsigned long int& end2,
                const unsigned int       n);

    };
}

#endif //HASHTABLESPIKES_DATABASEALGORITHMANALYSIS_H