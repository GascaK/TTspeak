#include <iostream>
#include <string>

// Main include header
#include "ttspeak.h"

int main(int argc, char** argv)
{
	std::string saved;
	// Class constructor called with object speechOut
	ttspeak speechOut;
	// Loads the ComSpeak function with max array size of 100
	// and gender enum of type FEMALE
	speechOut.loadComSpeak(100, FEMALE);
	// String to speech synthesis
	speechOut.comSpeak("Hello World.");

	// Class constructor called with object speechIn
	ttspeak speechIn;
	// Loads Voice Input with default paramaters
	speechIn.loadVoiceInput();
	// Waits for full phrase input and will alter saved string to
	// that input phrase. 
	speechIn.voiceInput(saved);
	// Input comparison example.
	if (saved == "Hello world")
		std::cout << "Successful Comparison\n";

	return 0;
}
