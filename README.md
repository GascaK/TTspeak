# TTspeak
Text-to-Speech header with Microsoft SAPI use.

Background:
    The Microsoft Speech Aplication Program Interface (SAPI) is convoluted and contains little to no tutorials w/ documentation. So here's my attempt to shorten that learning curve, or make your code cleaner. Whatever comes first.

This is written in C++ (11) and the GCC and GNU library. Make sure to keep your
repositories up to date.

ttspeak.h is commented (//) with things that may help you understand the dynamics of
working with Microsoft's SAPI and its limitations.

To call class ttspeak simply assign an object name and initialize the class with 
a MAX_ARR_SIZE int and a ttspeak enum, MALE or FEMALE.
EX:)
    ttspeak speechFemale = {256, ttspeak::FEMALE}
  Where ttspeak is the main class name
  speechFemale is an ObjectName.
  256 is the MAX_ARR_SIZE for the string input. Be weary and limit size as it may
  cause performance issues.
  ttspeak::FEMALE is an enum type for clarity.
 
