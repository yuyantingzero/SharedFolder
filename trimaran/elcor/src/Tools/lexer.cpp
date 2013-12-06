/******************************************************************************

                    SOFTWARE LICENSE AGREEMENT NOTICE
                   -----------------------------------

IT IS A BREACH OF THIS LICENSE AGREEMENT TO REMOVE THIS NOTICE FROM THE FILE
OR SOFTWARE, OR ANY MODIFIED VERSIONS OF THIS FILE OR SOFTWARE OR DERIVATIVE
WORKS.
___________________________________________________

Copyright Notices/Identification of Licensor(s) of Original Software
in the File


All rights reserved by the foregoing, respectively.
___________________________________________________

Copyright Notices/Identification of Subsequent Licensor(s)/Contributors of
Derivative Works

Copyright 1994 Hewlett-Packard Company

All rights reserved by the foregoing, respectively.
___________________________________________________

The code contained in this file, including both binary and source [if released
by the owner(s)] (hereafter, Software) is subject to copyright by the
respective Licensor(s) and ownership remains with such Licensor(s).  The
Licensor(s) of the original Software remain free to license their respective
proprietary Software for other purposes that are independent and separate from
this file, without obligation to any party.

Licensor(s) grant(s) you (hereafter, Licensee) a license to use the Software
for academic, research and internal business purposes only, without a fee.
"Internal business purposes" means that Licensee may install, use and execute
the Software for the purpose of designing and evaluating products.  Licensee
may submit proposals for research support, and receive funding from private
and Government sponsors for continued development, support and maintenance of
the Software for the purposes permitted herein.

Licensee may also disclose results obtained by executing the Software, as well
as algorithms embodied therein.  Licensee may redistribute the Software to
third parties provided that the copyright notices and this License Agreement
Notice statement are reproduced on all copies and that no charge is associated
with such copies. No patent or other intellectual property license is granted
or implied by this Agreement, and this Agreement does not license any acts
except those expressly recited.

Licensee may modify the Software to make derivative works (as defined in
Section 101 of Title 17, U.S. Code) (hereafter, Derivative Works), as
necessary for its own academic, research and internal business purposes.
Title to copyrights and other proprietary rights in Derivative Works created
by Licensee shall be owned by Licensee subject, however, to the underlying
ownership interest(s) of the Licensor(s) in the copyrights and other
proprietary rights in the original Software.  All the same rights and licenses
granted herein and all other terms and conditions contained in this Agreement
pertaining to the Software shall continue to apply to any parts of the
Software included in Derivative Works.  Licensee's Derivative Work should
clearly notify users that it is a modified version and not the original
Software distributed by the Licensor(s).

If Licensee wants to make its Derivative Works available to other parties,
such distribution will be governed by the terms and conditions of this License
Agreement.  Licensee shall not modify this License Agreement, except that
Licensee shall clearly identify the contribution of its Derivative Work to
this file by adding an additional copyright notice to the other copyright
notices listed above, to be added below the line "Copyright
Notices/Identification of Subsequent Licensor(s)/Contributors of Derivative
Works."  A party who is not an owner of such Derivative Work within the
meaning of U.S. Copyright Law (i.e., the original author, or the employer of
the author if "work of hire") shall not modify this License Agreement or add
such party's name to the copyright notices above.

Each party who contributes Software or makes a Derivative Work to this file
(hereafter, Contributed Code) represents to each Licensor and to other
Licensees for its own Contributed Code that:

(a)  Such Contributed Code does not violate (or cause the Software to
violate) the laws of the United States, including the export control laws of
the United States, or the laws of any other jurisdiction.

(b)  The contributing party has all legal right and authority to make such
Contributed Code available and to grant the rights and licenses contained in
this License Agreement without violation or conflict with any law.

(c)  To the best of the contributing party's knowledge and belief, the
Contributed Code does not infringe upon any proprietary rights or intellectual
property rights of any third party.

LICENSOR(S) MAKE(S) NO REPRESENTATIONS ABOUT THE SUITABILITY OF THE SOFTWARE
OR DERIVATIVE WORKS FOR ANY PURPOSE.  IT IS PROVIDED "AS IS"    WITHOUT
EXPRESS OR IMPLIED WARRANTY, INCLUDING BUT NOT LIMITED TO THE MERCHANTABILITY,
USE OR FITNESS FOR ANY PARTICULAR PURPOSE AND ANY WARRANTY AGAINST
INFRINGEMENT OF ANY INTELLECTUAL PROPERTY RIGHTS.  LICENSOR(S) SHALL NOT BE
LIABLE FOR ANY DAMAGES SUFFERED BY THE USERS OF THE SOFTWARE OR DERIVATIVE
WORKS.

Any Licensee wishing to make commercial use of the Software or Derivative
Works should contact each and every Licensor to negotiate an appropriate
license for such commercial use, and written permission of all Licensors will
be required for such a commercial license.  Commercial use includes (1)
integration of all or part of the source code into a product for sale by or on
behalf of Licensee to third parties, or (2) distribution of the Software or
Derivative Works to third parties that need it to utilize a commercial product
sold or licensed by or on behalf of Licensee.

By using or copying this Contributed Code, Licensee agrees to abide by the
copyright law and all other applicable laws of the U.S., and the terms of this
License Agreement.  Any individual Licensor shall have the right to terminate
this license immediately by written notice upon Licensee's breach of, or
non-compliance with, any of its terms.  Licensee may be held legally
responsible for any copyright infringement that is caused or encouraged by
Licensee's failure to abide by the terms of this License Agreement.

******************************************************************************/




/////////////////////////////////////////////////////////////////////////////
//
//      File:           lexer.cpp
//      Authors:        Richard Johnson
//      Created:        November 1997
//      Description:    Parser/lexer
//
/////////////////////////////////////////////////////////////////////////////

// lexer.cpp

#include "lexer.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const char BACKSLASH = '\\';
const char QUOTE = '\"';


Lexer::Lexer(const char *file_name)
{
  // Create the lexical analyzer;
  
  file_ptr = NULL;
  
  if (! strcmp(file_name, "cin")) {
    in = &cin;
  } else {
    file_ptr = new ifstream(file_name, ios::in);
    in = file_ptr;
  }
  
  line_number = 1;
  stack_full = 0;
}


Lexer::Lexer(istream& instream)
{
  // Create the lexical analyzer from a stream.
  
  file_ptr = NULL;
  in = &instream;
  
  line_number = 1;
  stack_full = 0;
}


Lexer::~Lexer()
{
  // Destroy the lexical analyzer;
  
  if (file_ptr!=NULL)
    delete file_ptr;
  
}

//--------------------- extraction operators ------------------------------


Lexer &
Lexer::get(char &ch)
{
  // Fetch a punctuation token;
  
  read_token();
  if (token_stack.token_type() != _PUNCTUATION)
    syntax_error(token_stack);
  ch = char(token_stack);
  stack_full = 0;
  return *this;
}

Lexer &
Lexer::get(char *buffer)
{
  // Fetch an identifier token;
  
  read_token();
  if (token_stack.token_type() != _IDENTIFIER)
    syntax_error(token_stack);
  strcpy(buffer, (char*)(token_stack));
  stack_full = 0;
  return *this;
}


Lexer &
Lexer::get(int &i)
{
  // Fetch an integer token;
  
  read_token();
  if (token_stack.token_type() != _NUMBER)
    syntax_error(token_stack);
  i = int(token_stack);
  stack_full = 0;
  return *this;
}


Lexer &
Lexer::get(float &f)
{
  // Fetch a float token;
  
  read_token();
  if (token_stack.token_type() != _NUMBER)
    syntax_error(token_stack);
  f = float(token_stack);
  stack_full = 0;
  return *this;
}


Lexer &
Lexer::get(double &d)
{
  // Fetch a double token;
  
  read_token();
  if (token_stack.token_type() != _NUMBER)
    syntax_error(token_stack);
  d = double(token_stack);
  stack_full = 0;
  return *this;
}


Lexer &
Lexer::check(const char ch)
{
  // Fetch a puntuation token and compare it with ch.  If different,
  // we have a syntax error;
  
  read_token();
  if (token_stack.token_type() != _PUNCTUATION)
    if (char(token_stack) != ch)
      syntax_error(token_stack);
  stack_full = 0;
  return *this;
}


Lexer &
Lexer::check(const char *string)
{
  // Fetch an identifier token and compare it with string.  If different,
  // we have a syntax error;
  
  read_token();
  if (token_stack.token_type() != _IDENTIFIER)
    syntax_error(token_stack);
  else if (strcmp((char*)token_stack, string) != 0)
    syntax_error(token_stack);
  stack_full = 0;
  return *this;
}


Lexer &
Lexer::get(Token &token)
{
  // Returns the next token in the input stream, regardless of type;
  
  read_token();
  token = token_stack;
  stack_full = 0;
  return *this;
}


bool
Lexer::end()
{
  // Returns true when there are no more tokens to consume

  if (!stack_full) {
    Token token;
    this->get(token);
    unget_token();
  }
  if (token_stack.token_type() == _END_OF_FILE)
    return true;
  else
    return false;
}    

void
Lexer::read_token()
{
  // If token stack empty, reads the next token in the input stream
  // and puts it into token_stack.  If token_stack is already full, does
  // nothing;
  
  char c;
  if (!stack_full) {
    skip_separators(*in);
    int next_char = in->peek();
    if (next_char == '"') {					// string;
      int length = 0;
      int backslash_count = 0;
      char last_c;
      in->get(c);
      for (;;) {
	last_c = c;  in->get(c);
	if (c == BACKSLASH) {
	  backslash_count++;
	  backslash_count &= 0x00000001;
	}
	if ((c == QUOTE) && (backslash_count == 0))
	  break;
	else
	  string_buffer[length++] = c;
	if (c != BACKSLASH)
	  backslash_count = 0;
      }
      string_buffer[length] = '\0';
      token_stack = string_buffer;
    } else if (isalpha(next_char) || (next_char=='_') || (next_char=='$')){
      int length = 0;					// identifier;
      while (in->get(c)) {
	/* SAM 6-96, added period as legal char in a token */
	if (isalpha(c) || isdigit(c) || (c == '_') || (c == '$') ||
	    (c == '.'))
	  string_buffer[length++] = c;
	else {
	  in->putback(c);
	  break;
	}
      }
      string_buffer[length] = '\0';
      token_stack = string_buffer;
    } else if (isdigit(next_char) || (next_char == '-')) {	// number;
      double d;
      *in >> d;
      if (in->fail())
	syntax_error(token_stack);
      token_stack = d;
    } else if (ispunct(next_char)) {			// punctuation;
      *in >> c;
      if (in->fail())
	syntax_error(token_stack);
      token_stack = c;
    } else if (next_char == EOF) {				// end of file;
      token_stack.make_eof();
    } else {
      assert(0);
    }
  }
}


void
Lexer::skipLine()
{
  // Skip a line of the input.

  skip_line(*in);
}

//------------------------ pushback operator ---------------------------

void
Lexer::unget_token()
{
  if (stack_full == 1) {
    cerr << "Lexer: attempt to push onto full token stack" << endl;
    exit(1);
  } else
    stack_full = 1;
}


//------------------ comments and whitespace -----------------------------

void
Lexer::skip_separators(istream &in)
{
  // Skip over whitespace and comments in the input stream;
  // Comments start with "#", "//", or "/*" and proceed to 
  // end of line, end of line, or "*/" respectively;
  
  char c;
  while (in.get(c)) {
    if (c == '\n')
      line_number++;
    if (isspace(c))
      continue;
    
    // No more space. Still have to check for comments;
    
    if (c == '#')
      skip_line(in);
    else if (c == '/') {
      if (!in.get(c))
	break;
      if (c == '/')
	skip_line(in);
      else if (c == '*')
	skip_c_comment(in);
    } else {
      
      // Not a comment, we have valid, non-comment, non-whitespace stuff;
      
      in.putback(c);
      break;
    }
  }
}


void
Lexer::skip_line(istream &in)
{
  // Strip characters until a newline (or end of file) is encountered;
  
  char c;
  while (in.get(c)) {
    if (c == '\n') {
      line_number++;
      break;
    }
  }
}


void
Lexer::skip_c_comment(istream &in)
{
  // Strip characters until "*/" (or end of file) is encountered;
  
  char c;
  while (in.get(c)) {
    if (c == '\n')
      line_number++;
    if (c == '*') {
      if (!in.get(c))
	break;
      if (c == '/')
	break;
    }
  }
}


//--------------------- errors --------------------------------


void
Lexer::syntax_error(Token &token)
{
  // Prints syntax error message when type of token was not of the
  // type expected;
  
  cerr << "Syntax error, line " << line_number << " at token `" << token
       << "'" << endl;
  exit(1);
}

