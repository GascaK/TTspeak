/********************************************************************** 
ttspeak.h Information:
	This header allows for use of Microsoft Speech API (SAPI)
	without the convuluted use of their function calls. ttspeak
	initializes the COM objects and sets up the API for ease of
	use, for people who just want a quick and easy way to get 
	Text-to-Speech and Voice Recognition into their applications.
Language:  C++ 14+
Revision:  2.0.0 (7.22.16)
Author:	   Gasca, K.
************************************************************************/

// COM include and _Module loader. sphelper has deprecated functions
// disabled warning 4996 for temporary solution.
// TODO: Make Microsoft(c) update sphelper.h..
#pragma warning (disable:4996)
#include <atlbase.h>
#include <atlcom.h>
#include <sapi.h>
#include <sphelper.h>
extern CComModule _Module;

// Global Enum for Gender type
enum Speech_Gender { MALE, FEMALE };

// Main ttspeak class with constructor and destructor.
class ttspeak
{
private:
	unsigned int MAX_ARR_SIZE;
	wchar_t* localText;
	HRESULT hr = NULL;
	ISpVoice* pVoice = nullptr;
	CComPtr<ISpRecognizer>	cpRecog;
	CComPtr<ISpObjectToken> objTok;
	CComPtr<ISpRecoContext> recoCon;
	CComPtr<ISpRecoGrammar> recoGram;
	CComPtr<ISpRecoResult>  recoRes;
	void checkOut(const HRESULT&);
public:
	ttspeak();
	~ttspeak();
	void loadComSpeak(int, Speech_Gender);
	void comSpeak(std::string);
	void loadVoiceInput();
	int voiceInput(std::string&);
};

/* Constructor
		Takes in no arguments only sets up the COM objects */
ttspeak::ttspeak()
{
	// Check for COM object Initialization failure. Throws exception with
	// output of error.
	if (FAILED(::CoInitialize(NULL)))
		throw std::exception("Unable to Initilize COM object.");
}

// Destructor for class ttspeak. COM object unitialize. Releases SAPI's 
// object names and sets class pointer to NULL. Deletes allocated memory
// in heap. Programmer may call to delete that ttspeak instance they created.
ttspeak::~ttspeak()
{
	::CoUninitialize();
	if(pVoice != NULL)
		pVoice->Release();
	pVoice = NULL;
	delete[] localText;
	return;
}

/* Load function with two paramaters, integer inArrSize and
    public enum with identifier Gender of type int.      */
void ttspeak::loadComSpeak(int inArrSize, Speech_Gender newGender)
{
	// MAX_ARR_SIZE is defined as the maximum input string literal your
	// users can input. Users will be unable to exceed this length.
	MAX_ARR_SIZE = inArrSize;
	localText = new wchar_t[MAX_ARR_SIZE];

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
	// output to desired gender if located in registry.
	// TODO: create more options for different voice casts. ie) age, speed 
	//		 and setdefaults.
	hr = pVoice->SetVoice(objTok);
	checkOut(hr);
}

// Main interface function for speech output. One parameter of type string. 
//	Will output the string literal as Speech from DEFAULT audio source.
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
	LPWSTR ptr = localText;
	// Output of voice with possible XML-like commands. Search MSDN SAPI 
	// documentation for further.
	hr = pVoice->Speak(ptr, SPF_IS_XML, NULL);
	checkOut(hr);

	return;
}

// Load function for Speech Recognition with default Inputs and default 
// language set to 409 | English. Other languages to come? No..
void ttspeak::loadVoiceInput()
{
	// CreateInstance with a Shared Recognizer. This recognizer is the
	// same recognizer other applications can use at the same time.
	hr = cpRecog.CoCreateInstance(CLSID_SpSharedRecognizer);
	checkOut(hr);
	// Finds the best recognizer of language 409 | English
	hr = SpFindBestToken(SPCAT_RECOGNIZERS, L"language=409", NULL, &objTok);
	checkOut(hr);
	// Sets the Recognizer to the Object Token
	hr = cpRecog->SetRecognizer(objTok);
	checkOut(hr);
	// Activates the recognition engine state
	hr = cpRecog->SetRecoState(SPRST_ACTIVE);
	checkOut(hr);
	// Creates the Recognition engine context
	hr = cpRecog->CreateRecoContext(&recoCon);
	checkOut(hr);
	// Sets the Event Handle as Console Window Event
	hr = recoCon->SetNotifyWin32Event();
	checkOut(hr);
	// Sets the Engine interests as Speech Recognized.
	hr = recoCon->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION));
	checkOut(hr);
	// Creates the grammar that is recognizable
	hr = recoCon->CreateGrammar(0, &recoGram);
	checkOut(hr);
	// Loads the diction that it can recognize
	hr = recoGram->LoadDictation(NULL, SPLO_STATIC);
	checkOut(hr);
	// sets the diction state to active for input
	hr = recoGram->SetDictationState(SPRS_ACTIVE);
	checkOut(hr);

	return;
}

// Main interface for speech recognition, takes string address and returns
// value as type string.
int ttspeak::voiceInput(std::string& saved)
{
	// handle for speech recognized events.
	HANDLE hSpeechEvent = recoCon->GetNotifyEventHandle();
	// returns 1 if invalid handle was passed through EventHandle()
	if (hSpeechEvent == INVALID_HANDLE_VALUE)
		return 1;
	// Sets up the Events and the Wait function.
	CSpEvent spEvent;
	LPWSTR coMemResult = NULL;
	HANDLE rghEvents[] = { hSpeechEvent };
	DWORD dMessage = WaitForMultipleObjects(sp_countof(rghEvents),
		rghEvents, FALSE, INFINITE);

	// If wait was successful 
	if (dMessage == WAIT_OBJECT_0)
	{
		// Get the Event
		spEvent.GetFrom(recoCon);
		// Set Event
		recoRes = spEvent.RecoResult();
		// Get text from recognized string
		hr = recoRes->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE,
			TRUE, &coMemResult, NULL);
		checkOut(hr);
	}
	else
		return 1;
		
	// Place holder for string return
	char* temp = NULL;
	// Sets the LPWSTR to type char[]
	int bytSize = WideCharToMultiByte(CP_ACP, 0, coMemResult,
		-1, 0, 0, 0, 0);
	temp = new char[bytSize];
	int retr = WideCharToMultiByte(CP_ACP, 0, coMemResult, -1,
		temp, bytSize, 0, 0);

	// Sets address of saved equal to the placeholder effectively returning
	// the recognized string.
	if (retr != 0)
	{
		temp[bytSize - 1] = 0;
		saved = temp;
	}

	// Deletes temp allocated memory, cause you know c++.
	delete[] temp;
	return 0;

}

// Error handling segment. Checks input address of HRESULTS refrenced against
// list of common failures. Defualt to common IDK response.
void ttspeak::checkOut(const HRESULT& hrVal)
{
	std::string retVal = "";
	switch (hrVal)
	{
	case S_OK: return;

	case E_ABORT: retVal = "Process was aborted."; break;

	case E_ACCESSDENIED: retVal = "Access was denied."; break;

	case E_FAIL: retVal = "Process failed."; break;

	case E_HANDLE: retVal = "Handle was invalid."; break;

	case E_INVALIDARG: retVal = "Argument was invalid."; break;

	case E_NOINTERFACE:	retVal = "No such interface supported."; break;

	case E_NOTIMPL:	retVal = "Function not implemented."; break;

	case E_OUTOFMEMORY:retVal = "Out of memory."; break;

	case E_POINTER:	retVal = "Invlaid pointer.";break;

	case E_UNEXPECTED:retVal = "Unexpected failure.";break;

	default: retVal = "Unknown Error: " + std::to_string(hrVal);
	}
	// Throws exception with output string for clarity and easy debugging.
	throw std::exception(retVal.c_str());
	return;
}
