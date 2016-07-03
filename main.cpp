//#define _ATL_APARTMENT_THREADED


#include <iostream>
#include <string>
#include <ttspeak.h>

using namespace std;

int main(int argc, char** argv)
{
	string text = "";
	ttspeak speechFemale = { 256, ttspeak::FEMALE };
	ttspeak speechMale = { 256, ttspeak::MALE };
	do {
		getline(cin, text);
		speechFemale.comSpeak(text);
		getline(cin, text);
		speechMale.comSpeak(text);
	} while (text != "quit");

	speechMale.comSpeak("Goodbye User!");
	speechFemale.comSpeak("Goodbye User!");
	return EXIT_SUCCESS;
}