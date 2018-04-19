/**
 * \file stringtest.cpp
 *
 * \brief Tests a ChunkyList for correctness.
 */

#include "testing-logger.hpp"
#ifndef LOAD_GENERIC_STRING
/* This test code can be used two ways.  It can either test a local
 * ChunkyString implementation, or it can dynamically load an implementation
 * at runtime and test that.  In the first homework, you'll dynamically load an
 * implementation that someone else used, but in later assignments, you'll
 * set the value to zero to use your own.
 */
#define LOAD_GENERIC_STRING 0       // 0 = Normal, 1 = Load Code Dynamically
#endif

#if LOAD_GENERIC_STRING
#include "string-wrapper.hpp"       // Use dynamic loading magic!
using TestingString = GenericString;
#else
#include "chunkystring.hpp"         // Just include and link as normal.
using TestingString = ChunkyString;
#endif

#include <string>
#include <sstream>
#include <stdexcept>
#include <cstddef>
#include <cstdlib>
#include <cassert>

#include "signal.h"
#include "unistd.h"

using namespace std;
#include<iostream>

static const size_t TESTING_CHUNKSIZE = 12;     // Assuming a chunksize of 12

// Helper functions
/**
 * \brief Assuming chunks are supposed to be at least an average of 
 *        1/divisor full, checks for the lowest allowable utilization 
 *        for the input string
 *
 * \remarks A helper function for affirming a TestingString's utilization 
 *          is at least 1/divisor. E.g., to check for adherence to 1/2, 
 *          divisor would be 2. The function does so by calculating the 
 *          lowest allowable utilization for a string the length of the 
 *          input string, including handling the edge cases of small strings. 
 *          Since checkUtilization is not a test on its own, but rather
 *          a helper function to be used in other tests, it doesn't
 *          create its own TestingLogger object. Instead, its affirms
 *          will be associated with the TestingLogger of the calling
 *          function. 
 *
 * \param test          TestingString to check
 * \param divisor       Fullness of chunk = 1/divisor
 */
void checkUtilization(const TestingString& test, size_t divisor)
{
    double utilLimit = 0;

    if (test.size() > 0)
    {
        size_t chunks = 1;
        size_t size = test.size() - 1;
        divisor = TESTING_CHUNKSIZE / divisor;
        chunks += (size + divisor - 1) / divisor;
        utilLimit = double(size) / double(chunks * TESTING_CHUNKSIZE);
        affirm(test.utilization() > utilLimit);
    }
}



// Testing functions

///////////////////////
//CHUNKY STRING TESTS//
///////////////////////

bool exampleTest()
{
    // Set up the TestingLogger object
    TestingLogger log("example test");

    TestingString s;
    
    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}

bool emptyTest() //checks for the size of an empty string
{
    // Set up the TestingLogger object
    TestingLogger log("empty test");

    // Test Setup
    TestingString myChunkyString;
    TestingString::iterator itrBegin = myChunkyString.begin();
    TestingString::iterator itrEnd = myChunkyString.end();
    affirm(myChunkyString.size() == 0);


    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}

bool pushBackTest()
{
    // Set up the TestingLogger object
    TestingLogger log("push back test");

    // Test Setup
    TestingString myChunkyString;
    char charToPush = 'a';
    myChunkyString.push_back(charToPush);
    affirm(myChunkyString.size() == 1);
    myChunkyString.push_back('b');
    affirm(myChunkyString.size() == 2);
    TestingString::iterator itrBegin = myChunkyString.begin();
    affirm(*itrBegin == 'a');
    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}


bool concatenateTest()
{
    // Set up the TestingLogger object
    TestingLogger log("concatenation test");

    // Test Setup
    TestingString myChunkyString;
    char charToPush = 'a';
    myChunkyString.push_back(charToPush);

    TestingString myChunkyString2;
    char charToPush2 = 'b';
    myChunkyString2.push_back(charToPush2);

    TestingString myChunkyString3;
    myChunkyString3.push_back('a');
    myChunkyString3.push_back('b');


    myChunkyString+=(myChunkyString2);

    affirm(myChunkyString.size() == 2);
    affirm(myChunkyString == myChunkyString3);


    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}

bool emptyConcatenateTest()
{
    // Set up the TestingLogger object
    TestingLogger log("empty concatenation test");

    // Test Setup
    TestingString myChunkyString;
    char charToPush = 'a';
    myChunkyString.push_back(charToPush);

    TestingString myChunkyString2;


    TestingString myChunkyString3;
    myChunkyString3.push_back('a');



    myChunkyString+=(myChunkyString2);

    affirm(myChunkyString.size() == 1);
    affirm(myChunkyString == myChunkyString3);


    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}

bool sameConcatenateTest()
{
    // Set up the TestingLogger object
    TestingLogger log("same concatenation test");

    // Test Setup
    TestingString myChunkyString;
    char charToPush = 'a';
    myChunkyString.push_back(charToPush);


    TestingString myChunkyString3;
    myChunkyString3.push_back('a');
    myChunkyString3.push_back('a');


    myChunkyString+=(myChunkyString);

    affirm(myChunkyString.size() == 2);
    affirm(myChunkyString == myChunkyString3);


    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}

bool equalityTest()
{
    // Set up the TestingLogger object
    TestingLogger log("equality test");

    // Test Setup
    TestingString myChunkyString;

    myChunkyString.push_back('a');
    myChunkyString.push_back('b');
    myChunkyString.push_back('c');

    TestingString myChunkyString2;
    myChunkyString2.push_back('a');
    myChunkyString2.push_back('b');
    myChunkyString2.push_back('c');

    affirm(myChunkyString==(myChunkyString2));
    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}

bool emptyEqualityTest()
{
    // Set up the TestingLogger object
    TestingLogger log("empty equality test");

    // Test Setup
    TestingString myChunkyString;
    TestingString myChunkyString2; 
    affirm(myChunkyString==(myChunkyString2));

    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}

bool inequalityTest()
{
    // Set up the TestingLogger object
    TestingLogger log("inequality test");

    // Test Setup
    TestingString myChunkyString;

    myChunkyString.push_back('a');
    myChunkyString.push_back('b');
    myChunkyString.push_back('c');

    TestingString myChunkyString2;
    myChunkyString2.push_back('a');
    myChunkyString2.push_back('b');
   

    affirm(myChunkyString!=(myChunkyString2));
    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}

/*
bool printTest()
{
    // Set up the TestingLogger object
    TestingLogger log("print test");

    // Test Setup
    TestingString testString;
    testString.push_back('a');
    std::ostringstream foo;
    foo << testString;


    affirm("a"==foo.str());
    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();    
}
*/

bool smallerTest()
{
    // Set up the TestingLogger object
    TestingLogger log("smaller test");

    // Test Setup
    TestingString myChunkyString;
    myChunkyString.push_back('a');
    myChunkyString.push_back('a');
    TestingString myChunkyString2;
    myChunkyString2.push_back('a');
    myChunkyString2.push_back('b');

    affirm(myChunkyString<(myChunkyString2));
    // Print a summary of the all the affirmations and return true
    // if they were all successful.

    return log.summarize();    
}

//////////////////
//ITERATOR TESTS//
//////////////////

bool dereferenceTest()
{
    #include <iostream>
    // Set up the TestingLogger object
    TestingLogger log("dereference test");

    //Test Setup
    TestingString myChunkyString;
    myChunkyString.push_back('a');
    TestingString::iterator itrBegin = myChunkyString.begin();
    int a = *itrBegin;
    affirm(*itrBegin=='a');

    
    
    

    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();    
}

bool beginTest()
{

    // Set up the TestingLogger object
    TestingLogger log("begin test");

    //Test Setup
    TestingString myChunkyString;
    myChunkyString.push_back('a');
    myChunkyString.push_back('b');
    myChunkyString.push_back('c');
    TestingString::iterator itrBegin = myChunkyString.begin();
    affirm(*itrBegin=='a');
    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();    
}

bool endTest()
{
    // Set up the TestingLogger object
    TestingLogger log("end test");

    //Test Setup
    TestingString myChunkyString;
    TestingString::iterator itrBegin = myChunkyString.begin();
    TestingString::iterator itrEnd = myChunkyString.end();
    affirm(itrBegin==itrEnd);

    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();    
}

bool incrementTest()
{
    // Set up the TestingLogger object
    TestingLogger log("increment test");

    //Test Setup
    TestingString myChunkyString;
    myChunkyString.push_back('a');
    myChunkyString.push_back('b');
    TestingString::iterator itrBegin = myChunkyString.begin();
    affirm(*itrBegin=='a');
    ++itrBegin;
    affirm(*itrBegin=='b');

    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();      
}

bool decrementTest()
{
    // Set up the TestingLogger object
    TestingLogger log("decrement test");

    //Test Setup
    TestingString myChunkyString;
    myChunkyString.push_back('a');
    myChunkyString.push_back('b');
    TestingString::iterator itrEnd = myChunkyString.end();
    --itrEnd;
    affirm(*itrEnd=='b');
    --itrEnd;
    affirm(*itrEnd=='a');

    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();      
}




bool sameOneElementTest()
{
    // Set up the TestingLogger object
    TestingLogger log("== test - 1 element");

    // Test Setup
    TestingString myChunkyString;
    myChunkyString.push_back('a');
    TestingString::iterator itrBegin = myChunkyString.begin();
    TestingString::iterator itrEnd = myChunkyString.end();
    --itrEnd;

    affirm(itrBegin==(itrEnd));

    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();    
}

bool sameEmptyTest()
{
    // Set up the TestingLogger object
    TestingLogger log("== test - empty");

    // Test Setup
    TestingString myChunkyString; 
    TestingString::iterator itrBegin = myChunkyString.begin();
    TestingString::iterator itrEnd = myChunkyString.end();
  
    affirm(itrBegin==(itrEnd));

    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();    
}


bool notSameTest()
{
    // Set up the TestingLogger object
    TestingLogger log("!= test");

    // Test Setup
    TestingString myChunkyString; 
    myChunkyString.push_back('a');
    myChunkyString.push_back('b');
    TestingString::iterator itrBegin = myChunkyString.begin();
    TestingString::iterator itrEnd = myChunkyString.end();
    --itrEnd;
  
    affirm(itrBegin!=(itrEnd));
    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();    
}


bool insertTest()
{
    // Set up the TestingLogger object
    TestingLogger log("insert test");

    // Test Setup
    TestingString myChunkyString;
    myChunkyString.push_back('a');
    myChunkyString.push_back('b');
    myChunkyString.push_back('c');
    TestingString::iterator itrBegin = myChunkyString.begin();
    ++itrBegin;
    myChunkyString.insert('z', itrBegin);  

    TestingString myChunkyString2;
    myChunkyString2.push_back('a');
    myChunkyString2.push_back('z');
    myChunkyString2.push_back('b');
    myChunkyString2.push_back('c');

    affirm(myChunkyString==myChunkyString2);

    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();    
}

bool insertToFullChunkTest()
{
    // Set up the TestingLogger object
    TestingLogger log("insert test");

    // Test Setup
    TestingString myChunkyString;
    myChunkyString.push_back('a');
    myChunkyString.push_back('b');
    myChunkyString.push_back('c');
    myChunkyString.push_back('d');
    myChunkyString.push_back('e');
    myChunkyString.push_back('f');
    myChunkyString.push_back('g');
    myChunkyString.push_back('h');
    myChunkyString.push_back('i');
    myChunkyString.push_back('j');
    myChunkyString.push_back('k');
    myChunkyString.push_back('l');
 
    TestingString::iterator end = myChunkyString.end();
    --end;
    --end;
    myChunkyString.insert('z', end);  

    TestingString myChunkyString2;
    myChunkyString2.push_back('a');
    myChunkyString2.push_back('b');
    myChunkyString2.push_back('c');
    myChunkyString2.push_back('d');
    myChunkyString2.push_back('e');
    myChunkyString2.push_back('f');
    myChunkyString2.push_back('g');
    myChunkyString2.push_back('h');
    myChunkyString2.push_back('i');
    myChunkyString2.push_back('j');
    myChunkyString2.push_back('z');
    myChunkyString2.push_back('k');
    myChunkyString2.push_back('l');

    
    affirm(myChunkyString==myChunkyString2);
    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();    
}



bool eraseTest()
{
    // Set up the TestingLogger object
    TestingLogger log("erase test");

    // Test Setup
    TestingString myChunkyString;
    myChunkyString.push_back('a');
    myChunkyString.push_back('b');
    TestingString::iterator itrBegin = myChunkyString.begin();
    ++itrBegin;
    myChunkyString.erase(itrBegin);

    TestingString myChunkyString2;
    myChunkyString2.push_back('a');

    affirm(myChunkyString==myChunkyString2);

    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();    
}
/*
/////////////////////
//UTILIZATION TESTS//
/////////////////////

bool utilizationEasyTest(){


 // Set up the TestingLogger object
TestingLogger log("utilization easy test");

// Test Setup
TestingString myChunkyString;
myChunkyString.push_back('a');
myChunkyString.push_back('b');

    for(int i =0; i<100; ++i){
        TestingString::iterator itrBegin = myChunkyString.begin();
        ++itrBegin;
        myChunkyString.insert(itrBegin, 'z');
        checkUtilization(myChunkyString, 2);

    }

    for(int i =0; i<100; ++i){
        TestingString::iterator itrEnd = myChunkyString.end();
        --itrEnd;
        myChunkyString.erase(itrEnd);
        checkUtilization(myChunkyString, 4);

    }

// Print a summary of the all the affirmations and return true
// if they were all successful.
return log.summarize();  
}
*/
bool utilizationHardInsertTest(){


 // Set up the TestingLogger object
TestingLogger log("utilization hard insert test");

// Test Setup
TestingString myChunkyString;
myChunkyString.push_back('a');
myChunkyString.push_back('b');

    for(int i =0; i<50; ++i){
        TestingString::iterator itrBegin = myChunkyString.begin();
        ++itrBegin;
        myChunkyString.insert('z', itrBegin);
        checkUtilization(myChunkyString, 2);
    }
    for(int i =0; i<50; ++i){
        TestingString::iterator itrBegin = myChunkyString.begin();
        for(int j = 0; j < 30; ++j){
            ++itrBegin;
        }
        myChunkyString.insert('z', itrBegin);
        checkUtilization(myChunkyString, 2);
    }

// Print a summary of the all the affirmations and return true
// if they were all successful.
return log.summarize();   
}

/*
bool utilizationHardEraseTest(){


 // Set up the TestingLogger object
TestingLogger log("utilization hard erase test");

// Test Setup
TestingString myChunkyString;
for(int i =0; i<1000; ++i){
    myChunkyString.push_back('a');
}
checkUtilization(myChunkyString, 2);

for(int i =0; i<50; ++i){
    TestingString::iterator itrBegin = myChunkyString.begin();
    for(int j = 0; j < 40; ++j){
        ++itrBegin;
    }
    myChunkyString.erase(itrBegin);
    checkUtilization(myChunkyString, 4);
}

// Print a summary of the all the affirmations and return true
// if they were all successful.
return log.summarize();  
}
*/








//--------------------------------------------------
//           RUNNING THE TESTS
//--------------------------------------------------

// Called if the test runs too long.
static void timeout_handler(int)
{
    // We go super-low-level here, because we can't trust anything in
    // the C/C++ library to really be working right.
    write(STDERR_FILENO, "Timeout occurred!\n", 18);
    abort();
}

/// Run tests
int main(int argc, char** argv)
{
    // Initalize testing environment
    TestingLogger alltests{"All tests"};

#if LOAD_GENERIC_STRING
    // Load the desired string implementation, if that's what we're doing.

    if (argc != 2) {
        cerr << "Usage ./stringtest plugin" << endl;
        exit(0);
    }

    GenericString::loadImplementation(argv[1]);
#else
    // No setup to do if we're using ChunkyString directly
#endif
    
    signal(SIGALRM, timeout_handler);   // Call this when the timer expires
    alarm(10);                          // set the timer at 10 seconds

    // Add calls to your tests here...
    
    affirm(exampleTest());
    affirm(emptyTest());
    affirm(pushBackTest());
    affirm(concatenateTest());
    affirm(sameConcatenateTest());    
    affirm(emptyConcatenateTest());
    affirm(emptyEqualityTest());    
    affirm(inequalityTest());    
    //affirm(printTest());  
    affirm(smallerTest()); 
    affirm(incrementTest());
    affirm(decrementTest());
    affirm(beginTest());
    affirm(endTest());
    affirm(dereferenceTest());
    affirm(insertTest()); 
    affirm(insertToFullChunkTest());
    affirm(sameOneElementTest());
    affirm(sameEmptyTest());
    affirm(notSameTest());
    affirm(utilizationHardInsertTest());
    affirm(eraseTest()); 
   // affirm(utilizationHardEraseTest()); 
   // affirm(utilizationEasyTest());

    if (alltests.summarize(true)) {
        return 0;       // Error code of 0 == Success!
    } else {
        return 2;       // Arbitrarily chosen exit code of 2 means tests failed.
    }
}




