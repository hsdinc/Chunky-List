/**
 * \file message-passer.cpp
 * \author Fill me in!
 *
 *
 * \remarks
 *
 */

#include <iostream>
#include <fstream>
#include <random>
#include "chunkystring.hpp"
#include "noisy-transmission.hpp"

using namespace std;

/**
 * \brief Option Processing
 * \details
 *   Sets various configuration variables by modifying variables passed
 *   by reference.
 *
 *   Will return with an exit error of 2 if receives a usage problem.
 *
 * \param options       Input of options from command line.
 * \param seed          Optional seed value for random number generator
 * \param filename      Name of file to read original message from
 * \param noiseLevel    Likelihood of a character being modified
 */
void processOptions(std::list<std::string> options,
	                string& filename,
                    float& noiseLevel)
{

    // Loop through the options. If they start with a dash
    // try to interpret them as a flag; if not, assume it's
    // the number of insertions.
    string flag;
    string value;
    while (!options.empty()) {

        // Get the name of the next option
        flag = options.front();
        options.pop_front();

        if (options.empty()) {
            std::cerr << "Empty argument" << std::endl;
            exit(2);
        };

        // Get the value for this option
        value = options.front();
        options.pop_front();

        // This flag provides the noise level
        if (flag == "-n" || flag == "--noise") {
            noiseLevel = stod(value);
        } 
        
        // This flag provides the filename
        else if (flag == "-f" || flag == "--filename") {
                filename = value;
        } 
        
        // Unknown flag: print message about how to use this program
        else {
            cerr << "Unrecognized option: " << flag << std::endl;
            cerr << "Usage: ./messagePasser -n noise -f filename" << endl;
            exit(2);
        }
    }

    return;
}



int main(int argc, const char* argv[])
{
    float noiseLevel = 0;
    string fileName;

    // Process the command-line options (provided in argv) 
    std::list<std::string> options(argv + 1, argv + argc);
    processOptions(options, fileName, noiseLevel);

    ifstream fileReader(fileName);

    // The file could not be opened
    if ( !fileReader.is_open() ) {
        cerr << "Unable to read from file" << fileName << endl;
        exit(-1);
    }

    // The file is open: create a ChunkyString that contains
    // the file contents, then transmit the contents.
    else {

        // Build a ChunkyString from the file contents
        string messageLine; 
        ChunkyString message;

        while (getline(fileReader, messageLine)) {
            for (const char c : messageLine) {
                message.push_back(c);
            }
            message.push_back('\n');
        }

        // Transmit the message on a noisy channel 
        NoisyTransmission::NoisyTransmission transmissionLine(noiseLevel);
        transmissionLine.transmit(message);

        // Print the garbled message
        //cout << message << endl;

        return 0;
    }
}
