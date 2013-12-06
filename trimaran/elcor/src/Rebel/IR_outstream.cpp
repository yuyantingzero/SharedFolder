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
//      File:           IR_outstream.cpp
//      Authors:        Greg Snider, Scott A. Mahlke
//      Created:        August 1995
//      Description:    stream output class for Elcor Text IR Reader/Writer
//
/////////////////////////////////////////////////////////////////////////////

#include "IR_outstream.h"
#include "IR_token.h"
#include <stdlib.h>
#include <string.h>
#include "ir_assert.h"
#include "el_io.h"




IR_outstream::IR_outstream(char *file_name)
{
    // Constructor;

    file_ptr = NULL;

    if (! strcmp(file_name, "cout")) { 
        out = &cout;
    }
    else if (! strcmp(file_name, "stdout")) {
        out = &cout;
    }
    else if (! strcmp(file_name, "cerr")) {
        out = &cerr;
    }
    else if (! strcmp(file_name, "stderr")) {
        out = &cerr;
    }
    else if (! strcmp(file_name, "clog")) {
        out = &clog;
    }
    else {
        file_ptr = new ofstream(file_name, ios::out);
        out = file_ptr;
    }
 
    indent_level = 0;
    last_token = PUNCTUATION;
    outbuf.precision(16);
}


IR_outstream::~IR_outstream()
{
    // Destructor;

    ptr_map.clear();
    printed.clear();

    // RCJ: this core dumps under aCC.  My guess is that out is being
    // closed, and file_ptr is deleted or corrupted at that point, causing
    // the core dump here.

    if (file_ptr!=NULL)
      delete file_ptr;
}


IR_outstream &
IR_outstream::operator<<(bool b)
{
    // Write an boolean to the output stream;

    if (last_token != PUNCTUATION)
        outbuf << " ";
    // ntclark
    outbuf.setf(ios::dec, ios::basefield);
    outbuf << b;
    //outbuf << dec << b;
    last_token = NUMBER;
    return *this;
}

IR_outstream &
IR_outstream::operator<<(int i)
{
    // Write an integer to the output stream;
    if (last_token != PUNCTUATION)
	outbuf << " ";
    // ntclark
    outbuf.setf(ios::dec, ios::basefield);
    outbuf << i;
    //outbuf << dec << i;
    last_token = NUMBER;
    return *this;
}

IR_outstream &
IR_outstream::operator<<(DType dt)
{

    // Write an DType to the output stream;
    char* c;
    c = dt.put_to_string();

    if (last_token != PUNCTUATION)
	outbuf << " ";
    outbuf << c;
    delete [] c;
    last_token = NUMBER;
    return *this;
}


IR_outstream &
IR_outstream::operator<<(Spatial_ref sr)
{
    // Write an Spatial_ref to the output stream;
    unsigned int count = sr.indices.size();

    char* c = new char[count+1];;

    for (unsigned int i = 0; i < count; i++) {
       c[i] = sr.indices[i].first == RELATIVE ? 'r' : 'a';
    }
    c[count] = '\0';

    if (last_token != PUNCTUATION)
	outbuf << " ";
    outbuf << c;
    delete c;

    for (unsigned int i = 0; i < count; i++) {
       outbuf << " ";
       outbuf << sr.indices[i].second;
    }

    last_token = NUMBER;
    return *this;
}

// Rajiv: for Physical for binding attribute
IR_outstream &
IR_outstream::operator<<(Hash_set<eString> pr)
{
    // Write an physical register file bindings to the output stream;
    for (Hash_set_iterator<eString> hiter(pr); hiter!=0; hiter++)
        *this << *hiter;
    return *this;
}

IR_outstream &
IR_outstream::operator<<(unsigned int i)
{
    // Write an unsigned integer to the output stream;

    if (last_token != PUNCTUATION)
	outbuf << " ";
    // ntclark
    outbuf.setf(ios::dec, ios::basefield);
    outbuf << i;
    //outbuf << dec << i;
    last_token = NUMBER;
    return *this;
}


IR_outstream &
IR_outstream::operator<<(char c)
{
    // Write a character to the output stream;

    outbuf << c;
    switch (c) {
      case '(':  case '[':  case '{':  case '<':
      case ' ':  case '\t':  case ':':  case '-':  
      case '"':  case '?': case '!': case ',':
	last_token = PUNCTUATION;
	break;
      default:
	last_token = IR_EOF;
	break;
    }
    return *this;
}


IR_outstream &
IR_outstream::operator<<(const char *s)
{
    // Write a string to the output stream;

    if (last_token != PUNCTUATION)
	outbuf << " ";
    outbuf << s;
    last_token = IDENTIFIER;
    return *this;
}

IR_outstream &
IR_outstream::operator<<(eString s)
{
    // Write a string to the output stream;

    if (last_token != PUNCTUATION)
	outbuf << " ";
    outbuf << s;
    last_token = IDENTIFIER;
    return *this;
}


IR_outstream &
IR_outstream::operator<<(float f)
{
    // Write a float to the output stream;

    if (f == -0.0)
	f = 0.0f;
    if (last_token != PUNCTUATION)
	outbuf << " ";
    outbuf.precision(32);
    outbuf << f;
    last_token = NUMBER;
    return *this;
}


IR_outstream &
IR_outstream::operator<<(double d)
{
    // Write a double to the output stream;

    if (d == -0.0)
	d = 0.0;
    if (last_token != PUNCTUATION)
	outbuf << " ";
    outbuf.precision(64);
    outbuf << d;
    last_token = NUMBER;
    return *this;
}


IR_outstream &
IR_outstream::operator<<(void *ptr)
{
    // Write a pointer to the output stream.  Pointers get translated to
    // small integers preceded by the '^' character, such that there is a
    // 1-1 mapping between pointer values and the small integers they're
    // mapped to.

    if (ptr == NULL) {
	cerr << "IR_outstream: attempt to write null pointer" << endl;
	exit(1);
    }

    // Map pointer to pointer id.  If no mapping exists, create one on the
    // fly;

    int id = map_pointer(ptr);

    if (last_token != PUNCTUATION)
	outbuf << " ";
    outbuf << "^" << id;
    last_token = POINTER;
    return *this;
}


int
IR_outstream::map_pointer(void *ptr)
{
    // Map a pointer.  Pointers get translated to small integers such
    // that there is a 1-1 mapping between pointer values and the
    // small integers they're mapped to;

    int id;
    int result = ptr_map.lookup(ptr, id);
    if (!result) {
	int new_id = ptr_map.entries();
	ptr_map.install(ptr, new_id);
    }
    result = ptr_map.lookup(ptr, id);
    ir_assert(result);
    return id;
}


void
IR_outstream::mark_printed(void *ptr)
{
    // Mark a pointer as having been printed.
    // small integers they're mapped to;

    int id;
    int result = printed.lookup(ptr, id);
    if (!result) {
	int new_id = 1;
	printed.install(ptr, new_id);
    }
}


int
IR_outstream::has_been_printed(void *ptr)
{
    // Mark a pointer as having been printed.
    // small integers they're mapped to;

    int id;
    return printed.lookup(ptr, id);
}


IR_outstream &
IR_outstream::operator<<(IR_outstream& (*function)(IR_outstream&))
{
    // Manipulator interface.  This allows manipulator functions to
    // be inserted in a sequence of << operators just like ostream;

    return function(*this);
}


void
IR_outstream::indent()
{
    // Increments indentation level;

    indent_level++;
}


void
IR_outstream::outdent()
{
    // Decrements indentation level;

    indent_level--;
    if (indent_level < 0) {
	cerr << "unmatched indenting, outdenting in IR_outstream" << endl;
	exit(1);
    }
}

void
IR_outstream::endline()
{
    int index;

    // Endline. Write the output buffer to the output file, breaking into
    // multiple lines if necessary.  Be careful not to break strings!;

    //    char line_buffer[LINE_BUF_SIZE];
    char *line_buffer = new char[outbuf.str().size()+1];

    //    if (outbuf.str().size() >= LINE_BUF_SIZE) {
    //      El_punt("IR_outstream: Cannot handle line longer than %d characters %d", LINE_BUF_SIZE, outbuf.str().size());
    //    }

    while (outbuf.str().size() > (unsigned) El_output_max_line_width) {
        strcpy(line_buffer, outbuf.str().c_str());

	// Line is too long.  Break the line at a space character that
	// maximizes the length of the broken line.
	enum StringState {IN_STRING, OUTSIDE_STRING};
        //char *temp_buffer = new char[strlen(line_buffer)+1];

	// if the current line is a comment, don't break it up
        if ((line_buffer[0] == '/') && (line_buffer[1] == '/')) {
	    break;
	}

	//strcpy(temp_buffer, line_buffer);
// 	StringState state = OUTSIDE_STRING;
// 	for (int i = El_output_max_line_width+1;
// 	     (unsigned) i < strlen(line_buffer);  i++)
// Instead of counting quote marks from here to the end, count from beginning to
// here.  This should give the same answer, and no longer results in n^2
// runtime.  -KF 9/2006
        StringState state = (line_buffer[0] == QUOTE) ? IN_STRING : OUTSIDE_STRING;
        for (int i = 1; i <= El_output_max_line_width; i++)
          if ((line_buffer[i] == QUOTE) && (line_buffer[i-1] != BSLASH))
            state = (state == IN_STRING) ? OUTSIDE_STRING : IN_STRING;
	bool found = false;
	for (index = El_output_max_line_width;
	     index > ((indent_level+1)*El_indent_width);
	     index--)
	{
	    if ((line_buffer[index]==QUOTE) && (line_buffer[index-1]!=BSLASH))
		state = (state == IN_STRING) ? OUTSIDE_STRING : IN_STRING;
	    else if ((line_buffer[index] == ' ') && (state == OUTSIDE_STRING)){
		line_buffer[index] = '\0';
		found = true;
		break;
	    }
	}
	if (found == true) {
	    // Write line fragment;
	    (*out) << line_buffer << endl;
            outbuf.str("");

	    // Indent for continuation and copy rest of line into line buffer;
	    indent();
	    tab();
	    outdent();
            //outbuf << &temp_buffer[index+1];
            outbuf << &line_buffer[index+1];
	} else {
	    // Line could not be fragmented.  Just write the thing out.
	    (*out) << line_buffer << endl;
            outbuf.str("");
	}
        //delete [] temp_buffer;  
    }
    (*out) << outbuf.str() << endl;
    outbuf.str("");
    last_token = PUNCTUATION;
 
    delete [] line_buffer;
}


void
IR_outstream::tab()
{
    // Normally called after endline(), this inserts
    // indent_level * El_indent_width spaces in the output stream;

    for (int level = 0;  level < indent_level;  level++)
	for (int space = 0;  space < El_indent_width;  space++)
	    outbuf << ' ';
    last_token = PUNCTUATION;
}

void IR_outstream::reset_maps()
{
    ptr_map.clear();
    printed.clear();
}


//--------------------------------------------------------------------
// Manipulators;

IR_outstream &indent(IR_outstream &out)
{
    out.indent();
    return out;
}
 
   
IR_outstream &outdent(IR_outstream &out)
{
    out.outdent();
    return out;
}


IR_outstream &endline(IR_outstream &out)
{
    out.endline();
    return out;
}


IR_outstream &tab(IR_outstream &out)
{
    out.tab();
    return out;
}


#ifdef _TEST_IR_OUTSTREAM_


main()
{
    IR_outstream out("junk");
    out << indent << endline;
    out << "******************************************" << indent << cut 
	<< "------------------------------------------" << endline;
}

#endif
