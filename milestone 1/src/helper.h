#pragma once
//lang::Cpp

#include "serial.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

/** Helper class providing some C++ functionality and convenience
 *  functions. This class has no data, constructors, destructors or
 *  virtual functions. Inheriting from it is zero cost.
 */
class Sys : public Serialize{
 public:

  // Printing functions
  Sys& p(char* c) { std::cout << c; return *this; }
  Sys& p(bool c) { std::cout << c; return *this; }
  Sys& p(float c) { std::cout << c; return *this; }  
  Sys& p(double c) { std::cout << c; return *this; }  
  Sys& p(int i) { std::cout << i;  return *this; }
  Sys& p(size_t i) { std::cout << i;  return *this; }
  Sys& p(const char* c) { std::cout << c;  return *this; }
  Sys& p(char c) { std::cout << c;  return *this; }
  Sys& pln() { std::cout << "\n";  return *this; }
  Sys& pln(int i) { std::cout << i << "\n";  return *this; }
  Sys& pln(char* c) { std::cout << c << "\n";  return *this; }
  Sys& pln(bool c) { std::cout << c << "\n";  return *this; }  
  Sys& pln(char c) { std::cout << c << "\n";  return *this; }
  Sys& pln(float x) { std::cout << x << "\n";  return *this; }
  Sys& pln(double x) { std::cout << x << "\n";  return *this; }
  Sys& pln(size_t x) { std::cout << x << "\n";  return *this; }
  Sys& pln(const char* c) { std::cout << c << "\n";  return *this; }

  // Copying strings
  char* duplicate(const char* s) {
    char* res = new char[strlen(s) + 1];
    strcpy(res, s);
    return res;
  }
  char* duplicate(char* s) {
    char* res = new char[strlen(s) + 1];
    strcpy(res, s);
    return res;
  }

  // Function to terminate execution with a message
  void exit_if_not(bool b, char* c) {
    if (b) return;
    p("Exit message: ").pln(c);
    exit(-1);
  }
  
  // Definitely fail
//  void FAIL() {
  void myfail(){
    pln("Failing");
    exit(1);
  }

  // Some utilities for lightweight testing
  void OK(const char* m) { pln(m); }
  void t_true(bool p) { if (!p) myfail(); }
  void t_false(bool p) { if (p) myfail(); }

  //substring of char*
  char* substring(char* st, int pos, int len) {
    char* pointer = new char[len];
    for (int c = 0; c < len; c++) {
      *(pointer+c) = *(st+pos);
      st++;
    }
    *(pointer+len) = '\0';
    return pointer;
  }

  int parseUntilSeperator(char* c, int start_pos) {
    int y = 0;
    while (c[start_pos + y] != '|') {
        y += 1;
    }
    return y;
  }

  int parseUntilClassSeperator(char* c, int start_pos) {
    int y = 0;
    int lcount = 0;
    int rcount = 0;
    while (c[start_pos + y] != '}' || lcount - 1 != rcount) {
      if (c[start_pos + y] == '}') {
        rcount += 1;
      }
      if (c[start_pos + y] == '{') {
        lcount += 1;
      }
      y += 1;
    }
    return y + 1;
  }


  int parseUntilBracketSeperator(char* c, int start_pos) {
    int y = 0;
    while (c[start_pos + y] != ']') {
        y += 1;
    }
    return y;
  }
};
