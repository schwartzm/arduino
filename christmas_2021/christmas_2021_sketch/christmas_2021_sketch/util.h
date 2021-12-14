#ifndef H_UTIL
#define H_UTIL



// freeMemory code to get current free dynamic ram.
// Just call freeMemory.
// Source: https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory();

#endif
