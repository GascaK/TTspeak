/***************************************************************** 
ttspeak.h Information:
	This header allows for use of Microsoft Speech API (SAPI)
	without the convuluted use of their function calls. ttspeak
	initializes the COM objects and sets up the API for ease of
	use, for people who just want a quick and easy way to get 
	Text-to-Speech into their C++ applications.
*******************************************************************/

// COM include and _Module loader sphelper has deprecated functions
// disabled warning 4996 for tempory solution.
// TODO: Make Microsoft(c) update sphelper.h..
#pragma warning (disable:4996)
#include <atlbase.h>
#include <atlcom.h>
#include <sphelper.h>
#include <sapi.h>
extern CComModule _Module;

//* Main ttspeak class with constructor and self cleaning destructor.
class ttspeak
{
private:
	unsigned int MAX_ARR_SIZE;
	wchar_t* localText;
	LPWSTR ptr = NULL;
	HRESULT hr = NULL;
	ISpVoice* pVoice = nullptr;
	ISpObjectToken* objTok = nullptr;
	void checkOut(const HRESULT&);
public:
	enum gender {MALE, FEMALE};
	ttspeak(int in, gender);
	~ttspeak();
	void comSpeak(std::string);
};

/* Constuctor with two paramaters, integer inArrSize and public enum
	with identifier Gender of type int.                              */
ttspeak::ttspeak(int inArrSize, ttspeak::gender newGender)
{
	// MAX_ARR_SIZE is defined as the maximum input string literal your
	// users can input. Users will be unable to exceed this length.
	MAX_ARR_SIZE = inArrSize;
	localText = new wchar_t[MAX_ARR_SIZE];

	// Check for COM object Initialization failure. Throws exception with
	// output of error.
	if (FAILED(::CoInitialize(NULL)))
		throw std::exception("Unable to Initialize COM object.");

	// Instance creation for SpVoice (SAPI).
	hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, 
		IID_ISpVoice, (void **)&pVoice);
	checkOut(hr);

	// Sets objTok to the gender type specified. 
	switch (newGender)
	{
	case MALE:
		SpFindBestToken(SPCAT_VOICES, L"gender = male", L" ", &objTok);
		break;
	case FEMALE:
		SpFindBestToken(SPCAT_VOICES, L"gender = female", L" ", &objTok);
		break;
	default:
		SpFindBestToken(SPCAT_VOICES, L" ", L" ", &objTok);
	}
	
	// Sends objTok to SetVoice (SAPI) which changes the gender of the voice
	// output to desired gender.
	// TODO: create more options for different voice casts. ie) age, speed 
	//		 and setdefaults.
	hr = pVoice->SetVoice(objTok);
	checkOut(hr);
}

// Destructor for class ttspeak. COM object unitialize. Releases SAPI's 
// object names and sets pointer to NULL. Deletes allocated memory in heap.
ttspeak::~ttspeak()
{
	::CoUninitialize();
	pVoice->Release();
	pVoice = NULL;
	delete[] localText;
	return;
}

// Main interface class for ttspeak. One parameter of type string. Will out
// put the string literal as Speech from default AUDIO source.
void ttspeak::comSpeak(std::string incText)
{
	// Check for string input LENGTH >= MAX_ARR_SIZE then returns if failed.
	if (strlen( incText.c_str() ) >= MAX_ARR_SIZE)
	{
		std::cout << "Input too large for MAX_ARR_SIZE: " << MAX_ARR_SIZE;
		return;
	}

	// Sets input string to string literal for assignment. Speak (SAPI) only
	// accepts LPWSTR as input.
	mbstowcs(localText, incText.c_str(), strlen( incText.c_str() ) +1 );
	ptr = localText;
	// Output of voice with possible XML-like commands. Search MSDN SAPI doc
	// umentation for further.
	hr = pVoice->Speak(ptr, SPF_IS_XML, NULL);
	checkOut(hr);

	return;
}

// Error handling segment. Checks input address of HRESULTS refrenced against
// list of common failures. Defualt to common IDK response.
void ttspeak::checkOut(const HRESULT& hrVal)
{

	std::string retVal = "";

	switch (hrVal)
	{
	case S_OK:
		return;
	case E_ABORT:
		retVal = "Process was aborted.";
		break;
	case E_ACCESSDENIED:
		retVal = "Access was denied.";
		break;
	case E_FAIL:
		retVal = "Process failed.";
		break;
	case E_HANDLE:
		retVal = "Handle was invalid.";
		break;
	case E_INVALIDARG:
		retVal = "Argument was invalid.";
		break;
	case E_NOINTERFACE:
		retVal = "No such interface supported.";
		break;
	case E_NOTIMPL:
		retVal = "Function not implemented.";
		break;
	case E_OUTOFMEMORY:
		retVal = "Out of memory.";
		break;
	case E_POINTER:
		retVal = "Invlaid pointer.";
		break;
	case E_UNEXPECTED:
		retVal = "Unexpected failure.";
		break;
	default:
		retVal = "Unknown Error: " + std::to_string(hrVal);
	}

	// Throws exception with output string for clarity and easy debugging.
	throw std::exception(retVal.c_str());
	return;

}
