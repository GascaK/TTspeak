#include <iostream>
#include <string>
// ttspeak header file. Include in your local path.
#include <ttspeak.h>

using namespace std;

int main(int argc, char** argv)
{
	string text = "";
	// ttspeak constructor of size 256 and voice of type
	// female or male.
	ttspeak speechFemale = { 100, ttspeak::FEMALE };
	ttspeak speechMale = { 100, ttspeak::MALE };
	do {
		// input and assignment.
		getline(cin, text);
		
		speechFemale.comSpeak(text);
		getline(cin, text);
		speechMale.comSpeak(text);
	} while (text != "quit");

	speechMale.comSpeak("Goodbye User!");
	speechFemale.comSpeak("Goodbye User!");
	return EXIT_SUCCESS;
}
