# TTspeak
Text-to-Speech header with Microsoft SAPI use.

Background:
    I am by no means an experienced programmer. I am simply uploading something
    I made that I thought was pretty cool. The microsoft SAPI is convuluted and
    contains little to no tutorials w/ documentation. So here's my attempt to 
    shorten that learning curve, or make your code cleaner. Whichever comes first.

This is written in C++ (11) and the GCC and GNU library. Make sure to keep your
repositories up to date.

ttspeak.h is commented (//) with things that may help you understand the dynamics of
working with Microsoft's SAPI and its limitations.

To call class ttspeak simply assign an object name and initialize the class with 
a MAX_ARR_SIZE int and a ttspeak enum, MALE or FEMALE.
EX:)
    ttspeak speechMale = {256, ttspeak::FEMALE}
  Where ttspeak is the main class name
  speechMale is an ObjectName.
  256 is the MAX_ARR_SIZE for the string input. Be weary and limit size as it may
  cause performance issues.
  ttspeak::FEMALE is an enum type for clarity.
 
