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
//using TestingString = GenericString;
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

bool exampleTest()
{
    // Set up the TestingLogger object
    TestingLogger log("example test");

    TestingString s;
    
    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}


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

    if (alltests.summarize(true)) {
        return 0;       // Error code of 0 == Success!
    } else {
        return 2;       // Arbitrarily chosen exit code of 2 means tests failed.
    }
}