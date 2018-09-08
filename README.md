# TTspeak
Text to Speech and Speech Recognition header with Microsoft SAPI(5.4) use. Version 2.0

## Getting Started
The Microsoft Speech Aplication Program Interface (SAPI) is convoluted and contains little to no tutorials w/ documentation. So here's my attempt to shorten that learning curve. TTspeak.h provides a simple API linker that will allow you to use Voice Synthesis and Voice Recognition significantly easier. Allowing quick prototype development without significant time investment. 

### Prerequisites
This is written in C++ (14) and the GCC and GNU library. Make sure to keep your repositories up to date.
A Microsoft Operating System: Windows Vista or later.

### Installing
Download a copy of the ttspeak.h header file. An example (main.cpp) provided as a small guide as to how to use the software.

## How to Use
Link the ttspeak header file on your IDE of choice. Project completed in Visual Studio 2015.
```
#include "ttspeak.h"
```
Construct the speech object(s) from TTSpeak class
```
ttspeak speechOut;
ttspeak speechIn;
```
Give the loadComSpeak function parameters for your specific project:
Options FEMALE / MALE.
```
speechOut.loadComSpeak(200,FEMALE);
```
Load the Voice Input for your speech object:
```
speechIn.loadVoiceInput();
```
Let them speak
```
speechOut.comSpeak("Hello World!");
speechIn.voiceInput(std::string saved); // requires datatype string as parameter. 
```

## Version
* 2.0: Implemented voice detection and comparison.
* 1.0: Voice output through Windows SAPI.

## Authors
* **Kevin Gasca** - *Initial work* - [GascaK](https://github.com/GascaK)

## License
This project is licensed under MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Thank you stackoverflow for the long nights and comforting handholding.
