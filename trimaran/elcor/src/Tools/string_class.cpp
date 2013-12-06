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
//      File:           string_class.cpp
//      Authors:        Sadun Anik, Richard Johnson
//      Created:        December 1994
//      Modernized:     March 1998
//      Description:    Definition of class eString
//
/////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include "defs.h"

#include "string_class.h"

#define STRING_COLLECT

//-----------------------------------------------------------------------------
unsigned
hash_string_table_entry(string_table_entry& ste)
{
   return (hash_char_ptr(ste.str)) ;
}

//Hash_map<string_table_entry, unsigned int>
//  eString::string_table(hash_string_table_entry, 100017);

Hash_map<string_table_entry, unsigned int>* eString::string_table = NULL;


//-----------------------------------------------------------------------------
// eString will eventually maintain its own string table, implemented
// as a hash table that maps strings to reference counts.  Since hash_map
// requires == to be defined on the domain elements, we create a wrapper
// for char* called string_table_entry.
//
// string_table_entry does not allocate or free its string.  When an entry
// is added to the string table, we'll explicitly duplicate the char*.
//-----------------------------------------------------------------------------
string_table_entry::string_table_entry(char* s) {
  str = s;
}

string_table_entry::~string_table_entry() {
  str = NULL;
}

bool
string_table_entry::operator==(const string_table_entry& ste) const {
  return (strcmp(str, ste.str) == 0);
}


//-----------------------------------------------------------------------------
// decrement reference counter for the string pointed to by sptr
void eString::dec_ref() {
  assert (sptr->first.str);
  assert (sptr->second > 0);
  sptr->second -= 1;


/* #ifndef STRING_COLLECT  */
  // if reference counter is zero, remove string from table and delete
  if (sptr->second == 0) {
    // unbind deletes the string_template_element, which also
    // deletes the char*

    // Shivaram, string_table_entry does not delete the char*, it has
    // to be deleted explicitly.
   
    char* estr=(sptr->first.str);
    string_table->unbind(sptr->first);
    delete[] (estr);

  }
/* #endif  */

  sptr = NULL;
}

// add reference to new_s, either by adding new_s to table or by finding
// new_s and incrementing its reference counter
bool eString::add_ref(char* new_s) {

  bool ret_val;
  if (string_table == NULL) {
    string_table = new Hash_map<string_table_entry, unsigned int>(hash_string_table_entry, 100117);
  }

  string_table_entry *ste = new string_table_entry(new_s);
  Pair<string_table_entry, unsigned int>* p = string_table->find_entry(*ste);

  if (p) {
    delete ste;
    ret_val = true;
  } else {
    string_table->bind(*ste, 0);
    p = string_table->find_entry(*ste);
    assert (p != NULL);
    delete ste;
    ret_val = false;
  }

  sptr = p;
  sptr->second += 1;
  assert (sptr->first.str);
  return ret_val;
}

//-----------------------------------------------------------------------------
eString::eString() {
  char *new_s = new char[1];
  new_s[0] = '\0';
  if(add_ref(new_s))
    delete [] new_s;
}

eString::~eString() {
  dec_ref();
}

eString::eString(const eString& s) {
  sptr = s.sptr;
  sptr->second += 1; // increment reference counter

  assert (sptr->first.str);
}

eString::eString(const char * s) {
  assert(s != NULL) ;
  char *new_s = new char[::strlen(s)+1];
  ::strcpy(new_s, s);
  if(add_ref(new_s))
    delete [] new_s;
}

eString::eString(const string& str) {
  char* new_s = new char[str.size()+1];
  ::strcpy(new_s, str.c_str());
  if (add_ref(new_s))
    delete [] new_s;
}

eString::eString(const char c){
  char *new_s = new char[2];
  new_s[0] = c;
  new_s[1] = '\0';
  if(add_ref(new_s))
    delete [] new_s;
}

// makes an empty string of fixed size
eString::eString(const int len) {
  char *new_s = new char[len+1];
  for (int i=0; i<len; i++) {
    new_s[i] = ' ';
  }
  new_s[len] = '\0';
  if(add_ref(new_s))
    delete [] new_s;
}

//-----------------------------------------------------------------------------
eString& eString::operator=(const eString& s) {
  if (sptr != s.sptr) {
    dec_ref();
    sptr = s.sptr;
    sptr->second += 1;
    assert (sptr->first.str);
  }

  return *this;
}

eString& eString::operator=(const char *str) {
  eString s(str);
  if (sptr != s.sptr) {
    dec_ref();
    sptr = s.sptr;
    sptr->second += 1;
    assert (sptr->first.str);
  }

  return *this;
}

bool eString::operator==(const eString& s) const { 
  return (sptr == s.sptr);
}

bool eString::operator!=(const eString& s) const {
  return (sptr != s.sptr);
}
	
bool eString::operator==(const char * str) const {
  if(str == NULL) //hongtaoz: if str==NULL, ::strcmp will core dump
	El_punt("eString::operator== : Can not compare an eString with NULL.");
  return (::strcmp(sptr->first.str, str) == 0);
}
	
bool eString::operator!=(const char * str) const {
  if(str == NULL) //hongtaoz: if str==NULL, ::strcmp will core dump
	El_punt("eString::operator!= : Can not compare an eString with NULL.");
  return (::strcmp(sptr->first.str, str) != 0);
}

bool eString::operator<(const eString& s) const {
  return (::strcmp(sptr->first.str, s.sptr->first.str) < 0);
}
	
bool eString::operator>(const eString& s) const {
  return (::strcmp(sptr->first.str, s.sptr->first.str) > 0);
}
	
//-----------------------------------------------------------------------------
int eString::length() const {
  return (::strlen(sptr->first.str));
}

char eString::operator[](int pos) {
  int len = length();
  assert(0 <= pos && pos < len);
  return (sptr->first.str)[pos];
}

//-----------------------------------------------------------------------------
// appends a character
eString& eString::operator|=(char ch) {
  if (ch == '\0')
    return *this;

  //
  // create new string
  //
  int len = length();
  char* new_s = new char[len+2];
  ::strcpy(new_s, sptr->first.str);
  new_s[len] = ch;
  new_s[len+1] = '\0';

  dec_ref();
  if(add_ref(new_s))
    delete [] new_s;
  return *this;
}

// appends a string
eString& eString::operator|=(const eString& s) {
  int len1 = length();
  int len2 = s.length();
  int new_len = len1+len2;
  char* new_s = new char[new_len+1];
  ::strcpy(new_s, sptr->first.str);
  ::strcpy(&new_s[len1], s.sptr->first.str);

  dec_ref();
  if(add_ref(new_s))
    delete [] new_s;
  return *this;
}

eString::operator char*() const {
  //  char *tmp = new char[length()+1];
  //  ::strcpy(tmp, sptr->first.str);
  //  return tmp;

  return sptr->first.str;
}

eString::operator string() const {
  return string(sptr->first.str); 
}

char* eString::strdup() {
  char* new_s = new char[length()+1];
  ::strcpy(new_s, sptr->first.str);
   return new_s;
}

int eString::strcmp(eString& s) const {
  return(::strcmp(sptr->first.str, s.sptr->first.str));
}
	
// returns -1 if not a substring otherwise returns position
int eString::in_string(eString pat) const {
  int len, pat_len, k, i, diff;
  bool match;
  
  len = length();
  pat_len = pat.length();
  if (pat_len > len)
    return (-1); // longer string cannot be a substring

  diff = len-pat_len;
  for (k=0; k<=diff; k++){
    match = true;
    for(i=0; i<pat_len; i++){
      if((sptr->first.str)[k+i]!=pat[i]) {
	match=false;
	break;
      }
    }
    
    if (match) {
      return k; // matches beginning at position k
    }
  }
  return -1; // fails to match
}

//returns the substring between [begin,end]
eString eString::substr(int begin, int end) const {
  int len, diff, i;
  
  len = length();
  assert(0 <= begin && end < len);
  diff = (begin<=end) ? 1+end-begin : 0;
  char* buf = new char[diff+1];
  
  for(i=begin;i<=end;i++) {
    buf[i-begin] = (sptr->first.str)[i]; // copy the string
  }
  buf[diff] = '\0';

  eString result(buf);
  delete [] buf;
  return result;
}

// returns a string where latex special
// characters are preceded by a back-slash
eString eString::latex_normalize() {
   int i, size, new_size;
   char *buf;
	
   size=length();
   new_size = size ;
   for(i=0; i<size; i++){
      char c = (*this)[i] ;
      if ((c == '#') ||
	  (c == '$') ||
	  (c == '%') ||
	  (c == '&') ||
	  (c == '_') ||
	  (c == '{') ||
	  (c == '}')){
	 new_size++ ; // these characters will be preceded by a backslash
      }
      else if ((c == '~') ||
	       (c == '^')) {
	 new_size += 3 ; // these characters will be preceded by backslash
	 // and followed by {}
      }
      else if (c == '\\') {
	 new_size += 11 ; // replace \ with $\backslash$
      }
   }   
   buf = new char[new_size+1]; //make a string of the right size
   buf[new_size] = '\0';

   int j = 0 ;
   for(i = 0 ; i < size ; i++) {
      char c = (*this)[i] ;
      if ((c == '#') ||
	  (c == '$') ||
	  (c == '%') ||
	  (c == '&') ||
	  (c == '_') ||
	  (c == '{') ||
	  (c == '}')){
	 buf[j] =  '\\' ;
	 j++ ;
	 buf[j] =  (*this)[i]; // copy the string
	 j++ ;
      }
      else if ((c == '~') ||
	       (c == '^')) {
	 buf[j] =  '\\' ;
	 j++ ;
	 buf[j] =  (*this)[i]; // copy the string
	 j++ ;
	 buf[j] =  '{' ;
	 j++ ;
	 buf[j] =  '}' ;
	 j++ ;
      }
      else if (c == '\\') {
	 buf[j] =  '$' ;
	 j++ ;
	 buf[j] =  '\\' ;
	 j++ ;
	 buf[j] =  'b' ;
	 j++ ;
	 buf[j] =  'a' ;
	 j++ ;
	 buf[j] =  'c' ;
	 j++ ;
	 buf[j] =  'k' ;
	 j++ ;
	 buf[j] =  's' ;
	 j++ ;
	 buf[j] =  'l' ;
	 j++ ;
	 buf[j] =  'a' ;
	 j++ ;
	 buf[j] =  's' ;
	 j++ ;
	 buf[j] =  'h' ;
	 j++ ;
	 buf[j] =  '$' ;
	 j++ ;
      }
      else {
	 buf[j] = (*this)[i]; // copy the string
	 j++ ;
      }
   }

   eString result(buf);
   return result;
}

eString& eString::cat(const char *c) {
  *this |= c;
  return *this;
}
	
eString& eString::cat(const eString& s) {
  *this |= s;
  return *this;
}

void eString::setchar(int pos, char ch) {
  int len = length();

  //
  // create new string
  //
  char *new_s = new char[len+1];
  ::strcpy(new_s, this->sptr->first.str);

  assert (0 <= pos && pos < len);
  new_s[pos] = ch;

  dec_ref();
  if(add_ref(new_s))
    delete [] new_s;
}

int eString::charpos(char c, int k) {
  int i;
  int len = length();
  for (i=0; i<len; i++) {
    if ((*this)[i]==c && (--k)==0) break;
  }
  return ((i==len) ? (-1) : i);
}

int eString::charrpos(char c, int k) {
  int i;
  int len = length();
  for (i=len-1; i>=0; i--) {
    if (((sptr->first.str)[i]==c) && ((--k)==0)) break;
  }
  return i;
}

ostream& operator << (ostream& os, const eString& s) {
  os << s.sptr->first.str;
  return(os);
}
	
istream& operator >> (istream& is, eString& s) {
  static char buf[2048];
  is >> buf;
  eString es(buf);
  s = es;

  return is;
}

eString operator | (eString l_arg, eString r_arg) {
  eString result(l_arg);
  result |= r_arg;
  return result;
}

int str_to_int(const eString& s) {
  int result;
  sscanf(s.sptr->first.str, "%d", &result);
  return result;
}

double str_to_double(const eString& s){
  double result;
  sscanf(s.sptr->first.str, "%le", &result);
  return result;
}


eString int_to_str(int s) {
  static char buf[2048];
  snprintf(buf, 2048, "%d", s);

  eString result(buf);
  return result;
}

eString double_to_str(double d) {
  static char buf[2048];
  snprintf(buf, 2048, "%f", d);

  eString result(buf);
  return result;
}

eString float_to_str(float f) {
  static char buf[2048];
  snprintf(buf, 2048, "%f", f);

  eString result(buf);
  return result;
}

unsigned hash_estring(eString& s) {
  return (hash_char_ptr(s.sptr->first.str));
}

//-----------------------------------------------------------------------------

// end of string_class.cpp
