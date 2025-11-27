#ifndef DBOP_H
#define DBOP_H

#include <stdio.h>

// Enable debug prints by uncommenting the desired line below
#define DEBUG_INFO_Main
//#define DEBUG_INFO_Led
//#define DEBUG_INFO_Button

#ifdef DEBUG_INFO_Main
  // All debug prints for Exercise_6A come out in the same format
  #define C_Main(msg) printf("%*s Dbg-Main: %s\n", 15, msg)
#else
  // If DEBUG_INFO_Main is not defined, C_Main becomes an empty macro
  #define C_Main(msg)
#endif

#ifdef DEBUG_INFO_Led
  // All debug prints for Led come out in the same format
  #define C_Led(msg) printf("%*s Dbg-Led: %s\n", 15, msg)
#else
  // If DEBUG_INFO_Led is not defined, C_Led becomes an empty macro
  #define C_Led(msg)
#endif

#ifdef DEBUG_INFO_Button
  // All debug prints for Button come out in the same format
  #define C_Button(msg) printf("%*s Dbg-PushButton: %s\n", 15, msg)
#else
  // If DEBUG_INFO_PushButton is not defined, C_Button becomes an empty macro
  #define C_Button(msg)
#endif

#endif // DBOP_H
