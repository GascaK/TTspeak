# TTspeak
Text-to-Speech header with Microsoft SAPI(5.4) use.

Background:
    The Microsoft Speech Aplication Program Interface (SAPI) is convoluted and contains little to no tutorials w/ documentation. So here's my attempt to shorten that learning curve, or make your code cleaner. Whatever comes first.

This is written in C++ (14) and the GCC and GNU library. Make sure to keep your
repositories up to date.

ttspeak.h is commented (//) with things that may help you understand the dynamics of
working with Microsoft's SAPI and its limitations.

ChangeLOG
v1.0: Speech Synthesis only.
v2.0: Speech Recognition added with default paramaters and English(409) as acceptable input language. Will return entire phrase.
