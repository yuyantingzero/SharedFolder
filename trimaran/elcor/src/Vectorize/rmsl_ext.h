/***********************************************************************

Copyright (c) 2007 Massachusetts Institute of Technology

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

***********************************************************************/



//===========================================================================
//
//   FILE: rmsl_ext.h:
//   
//   Author: Sam Larsen
//   Date: Wed Apr 20 21:38:21 2005
//
//   Function:  Copied from rmsl.h and extended to allow for removal of
//		operations from bins.
//
//===========================================================================

#ifndef _RMSL_EXT_H_
#define _RMSL_EXT_H_ 

#include "mdes.h"
#include "vector.h"

class Rmsl_ext {
public:
  Rmsl_ext(MDES* md_in);
  ~Rmsl_ext();

  void reset();
  void nextop_io(char* opcode, char* io);
  void remove_io(char* opcode, char* io);
  int value(double&);

  void checkpoint() { cp_vect = cum_vect; }
  void restore() { cum_vect = cp_vect; }

  void print();
  char* most_used();

private:
  MDES* md;
  Vector<int> cum_vect;
  Vector<int> cp_vect;
};

#endif
