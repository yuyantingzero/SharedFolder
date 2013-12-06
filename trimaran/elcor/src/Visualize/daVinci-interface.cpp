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
//      File:           daVinci-interface.cpp
//      Authors:        Richard Johnson, Santosh Abraham
//      Created:        December 1994
//      Description:    Interface implementation for daVinci tool
//
/////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/resource.h>
#include <sys/signal.h>
#include <sys/types.h>
#include "defs.h"
#include "daVinci-interface.h"

//ntclark
extern ostream& cdbg;


daVinci::daVinci ()
{
  debug(false);
  connected = false;
}

daVinci::~daVinci ()
{
 if (connected) {
  quit ();
  
  close(pin);
  close(pout);
  }
}

daVinci&
daVinci::operator<< (char *foo)
{
  pout_str(foo);
  return *this;
}

daVinci&
daVinci::operator<< (int foo)
{
  pout_int(foo);
  return *this;
}

daVinci&
daVinci::initconnect ()
{
  connect ();
  connected = true;

  bool ok = false;
  do {
    int buf_posit = 0;
    do {
      buf_posit += read(pin, &(buffer[buf_posit]), 1);
    } while((buffer[buf_posit -1] != 0xa /* newline */) && 
	    (buffer[buf_posit -1] != (char)0x4 /* EOT symbol*/));

    buffer[buf_posit - 1] = 0;
    if(strcmp(buffer, "ok") == 0)
      ok = true;
  } while(!ok);
  
  font_size (8);
  gap_x (8);
  gap_y (4);
  return *this;
}

char *
daVinci::get_msg ()
{
  int buf_posit = 0;
  do {
    buf_posit += read(pin, &(buffer[buf_posit]), 1);
  } while((buffer[buf_posit - 1] != '\n') && (buffer[buf_posit - 1] != (char)0x4 /* EOT symbol*/));
  
  buffer[buf_posit -1] = 0;
  return buffer;
}


void
daVinci::pout_str(char* foo) 
{
  int size = 0;
  for(int i = 0; foo[i] != 0; size++, i++);

  int bytes_written = 0;
  while(bytes_written < size) {
    bytes_written += write(pout, &(foo[bytes_written]), size - bytes_written);
  } 
}

void
daVinci::pout_int(int foo)
{
  char the_int[100];
  sprintf(the_int, "%i", foo);

  int size = 0;
  for(int i = 0; the_int[i] != 0; size++, i++);

  int bytes_written = 0;
   while(bytes_written < size) {
    bytes_written += write(pout, &(the_int[bytes_written]), size - bytes_written);
  }
}


//----------------------------------------------------------------------------
daVinci&
daVinci::message (char *msg)
{
  pout_str("show_message(");
  pout_str(msg);
  pout_str(")\n");

  return *this;
}

daVinci&
daVinci::status (char *status)
{
  pout_str("show_status(");
  pout_str(status);
  pout_str(")\n");

  //(*pout) << "show_status(" << status << ")" << endl;
  return *this;
}


//----------------------------------------------------------------------------
daVinci&
daVinci::load_graph (char *path)
{
  
  pout_str("load_graph(");
  pout_str(path);
  pout_str(")\n");

  //  (*pout) << "load_graph(" << path << ")" << endl;
  return *this;
}

daVinci&
daVinci::load_graph_placed (char *path)
{
  
  pout_str("load_graph_placed(");
  pout_str(path);
  pout_str(")\n");

  //(*pout) << "load_graph_placed(" << path << ")" << endl;
  return *this;
}

daVinci&
daVinci::save_graph (char *path)
{
  
  pout_str("save_graph(");
  pout_str(path);
  pout_str( ")\n");

  //  (*pout) << "save_graph(" << path << ")" << endl;
  return *this;
}


//----------------------------------------------------------------------------
daVinci&
daVinci::load_status (char *path)
{
  
  pout_str("load_status(");
  pout_str(path);
  pout_str(")\n");

  //  (*pout) << "load_status(" << path << ")" << endl;
  return *this;
}

daVinci&
daVinci::save_status (char *path)
{
  
  pout_str("save_status(");
  pout_str(path);
  pout_str(")\n");

  //  (*pout) << "save_status(" << path << ")" << endl;
  return *this;
}


//----------------------------------------------------------------------------
daVinci&
daVinci::save_postscript (char *path)
{
  
  pout_str("save_postscript(");
  pout_str(path);
  pout_str(")\n");

  //  (*pout) << "save_postscript(" << path << ")" << endl;
  return *this;
}


//----------------------------------------------------------------------------
daVinci&
daVinci::scale (int percentage)
{
  
  pout_str("set_scale(");
  pout_int(percentage);
  pout_str(")\n");

  //  (*pout) << "set_scale(" << percentage << ")" << endl;
  return *this;
}


//----------------------------------------------------------------------------
daVinci&
daVinci::select_nodes (char *str)
{
  
  pout_str("select_nodes_labels(");
  pout_str(str);
  pout_str(")\n");

  //  (*pout) << "select_nodes_labels(" << str << ")" << endl;
  return *this;
}

daVinci&
daVinci::node_color (char *node, char *color)
{
  
  pout_str("change_node_color(");
  pout_str(node);
  pout_str( ",");
  pout_str( color);
  pout_str(")\n");

  //  (*pout) << "change_node_color(" << node << "," << color << ")" << endl;
  return *this;
}


//----------------------------------------------------------------------------
daVinci&
daVinci::focus_node (char *node)
{
  
  pout_str("focus_node(");
  pout_str(node);
  pout_str(")\n");

  //  (*pout) << "focus_node(" << node << ")" << endl;
  return *this;
}

daVinci&
daVinci::focus_node_smooth (int step, char *node)
{
  
  pout_str("focus_node_animated(");
  pout_int(step);
  pout_str(",");
  pout_str(node);
  pout_str(")\n");

  //  (*pout) << "focus_node_animated(" << step << "," << node << ")" << endl;
  return *this;
}


//----------------------------------------------------------------------------
daVinci&
daVinci::place_all ()
{
  
  pout_str("place_all_nodes");

  //  (*pout) << "place_all_nodes" << endl;
  return *this;
}

daVinci&
daVinci::place_visible ()
{
  
  pout_str("place_visible_nodes");

  //  (*pout) << "place_visible_nodes" << endl;
  return *this;
}

daVinci&
daVinci::compact_graph ()
{
  // ntclark
  pout_str("compact_graph");

  // (*pout) << "compact_graph" << endl;
  return *this;
}

daVinci&
daVinci::toggle_subgraph ()
{
  
  pout_str("hide_or_show_subgraph");

  //  (*pout) << "hide_or_show_subgraph" << endl;
  return *this;
}

daVinci&
daVinci::hide_subgraph (char *subgraph)
{
  
  pout_str( "hide_subgraphs(");
  pout_str(subgraph);
  pout_str(")\n");

  //  (*pout) << "hide_subgraphs(" << subgraph << ")" << endl;
  return *this;
}

daVinci&
daVinci::show_subgraph (char *subgraph)
{
  
  pout_str("show_subgraphs(");
  pout_str(subgraph);
  pout_str(")\n");

  //  (*pout) << "show_subgraphs(" << subgraph << ")" << endl;
  return *this;
}

daVinci&
daVinci::restore_subgraphs ()
{
  
  pout_str("restore_subgraphs");

  //  (*pout) << "restore_subgraphs" << endl;
  return *this;
}

daVinci&
daVinci::toggle_edges ()
{
  
  pout_str("hide_or_show_edges");

  //  (*pout) << "hide_or_show_edges" << endl;
  return *this;
}

daVinci&
daVinci::restore_edges ()
{
  
  pout_str("restore_edges");

  //  (*pout) << "restore_edges" << endl;
  return *this;
}

//----------------------------------------------------------------------------
daVinci&
daVinci::accuracy_phase_1 (int acc)
{
  
  pout_str("set_accuracy_phase_1(");
  pout_int(acc);
  pout_str(")\n");

  //  (*pout) << "set_accuracy_phase_1(" << acc << ")" << endl;
  return *this;
}

daVinci&
daVinci::accuracy_phase_2 (int acc)
{
  
  pout_str("set_accuracy_phase_2(");
  pout_int(acc);
  pout_str(")\n");

  //  (*pout) << "set_accuracy_phase_2(" << acc << ")" << endl;
  return *this;
}
   
//----------------------------------------------------------------------------
daVinci&
daVinci::font_size (int size)
{
  
  pout_str("set_font_size(");
  pout_int(size);
  pout_str(")\n");

  //  (*pout) << "set_font_size(" << size << ")" << endl;
  return *this;
}

daVinci&
daVinci::gap_y (int gap)
{
  
  pout_str("set_gap_height(");
  pout_int(gap);
  pout_str(")\n");

  //  (*pout) << "set_gap_height(" << gap << ")" << endl;
  return *this;
}

daVinci&
daVinci::gap_x (int gap)
{
  
  pout_str("set_gap_width(");
  pout_int(gap);
  pout_str(")\n");

  //(*pout) << "set_gap_width(" << gap << ")" << endl;
  return *this;
}

daVinci&
daVinci::edge_gap (int gap)
{
  
  pout_str("set_multiple_edge_gap(");
  pout_int( gap);
  pout_str(")\n");

  //(*pout) << "set_multiple_edge_gap(" << gap << ")" << endl;
  return *this;
}

daVinci&
daVinci::edge_radius (int r)
{
  
  pout_str("set_self_refering_edge_radius(");
  pout_int(r);
  pout_str(")\n");

  //(*pout) << "set_self_refering_edge_radius(" << r << ")" << endl;
  return *this;
}

//----------------------------------------------------------------------------
daVinci&
daVinci::window_title (char *title)
{
  
  pout_str("set_window_title(");
  pout_str(title);
  pout_str(")\n");

  //(*pout) << "set_window_title(" << title << ")" << endl;
  return *this;
}

daVinci&
daVinci::window_xy (int x, int y)
{
  
  pout_str("set_window_position(");
  pout_int( x);
  pout_str( ",");
  pout_int(y);
  pout_str(")\n");

  //(*pout) << "set_window_position(" << x << "," << y << ")" << endl;
  return *this;
}

daVinci&
daVinci::window_size (int size)
{
  
  pout_str( "set_window_size(");
  pout_int(size);
  pout_str(")\n");

  //(*pout) << "set_window_size(" << size << ")" << endl;
  return *this;
}

daVinci&
daVinci::window_open ()
{
  // ntclark
  pout_str( "open_window\n");

  //  (*pout) << "open_window" << endl;
  return *this;
}

daVinci&
daVinci::window_close ()
{
  
  pout_str("close_window");

  //(*pout) << "close_window" << endl;
  return *this;
}

//----------------------------------------------------------------------------
daVinci&
daVinci::deactivate ()
{
  
  pout_str("deactivation\n");

  //(*pout) << "deactivation" << endl;
  return *this;
}

daVinci&
daVinci::activate ()
{
  
  pout_str("activation\n");

  //(*pout) << "activation" << endl;
  return *this;
}

daVinci&
daVinci::quit ()
{
  
  pout_str("quit\n");

  //(*pout) << "quit" << endl;
  return *this;
}

daVinci&
daVinci::nothing ()
{
  
  pout_str("nothing\n");

  //(*pout) << "nothing" << endl;
  return *this;
}

//----------------------------------------------------------------------------
void
daVinci::connect ()
/* This funtion is connecting daVinci by forking a child process, creating
 * two pipes for the transmission of commands to daVinci and the receiption of
 * answers from daVinci, redirecting the stdin, stdout and stderr of the parent
 * and the child process to the corresponding end of the pipe and calling
 * daVinci from the child process. */
{
  int   i;
  /*	
   *                 [0]                           [1]
   *    child reads:  |========= pipe_io[0] ========| <- parent writes
   *   pipe_io[0][0]                                     pipe_io[0][1]
   *
   *    parent reads: |========= pipe_io[1] ========| <- child writes
   *   pipe_io[1][0]                                     pipe_io[1][1]
   *
   * The parent process reads the output of the child process by reading
   * pipe_io[1][0] because the child is writing to pipe_io[1][1].
   * The child process gets its input from pipe_io[0][0] because the
   * parent writes to pipe_io[0][1].  Thus, one process is reading from
   * one end of the pipe while the other is writing at the other end.
   */

  // Get the path and filename of daVinci.
  // Internally, *path must be "/car/azrael/elcor/daVinci_V1.4_hp/daVinci";

  char *path = getenv ("DAVINCIEXE");
  strcat (path, "/daVinci");

  // Check if the filename exists. Exit if not
  ifstream fin(path);
  if (!fin) {
    cout << "application: cannot find daVinci under " << path << endl;
    return;
  };
  fin.close();
  
  pipe (pipe_io[0]);		// Create pipe for parent->child communication.
  pipe (pipe_io[1]);		// Create pipe for child->parent communication.
  
  /* Fork child process. Up from here the code is
     executed from the child AND the parent process! */
  switch (pid = (int) fork ())  
    {
    case -1:			// Unable to fork process! Close both pipes.
      close (pipe_io[0][0]);
      close (pipe_io[0][1]);
      close (pipe_io[1][0]);
      close (pipe_io[1][1]);
      perror ("fork failed");
      _exit (1);
    case  0:
      {				// This is program code for the child process!
	/* Redirect child's stdin (0), stdout (1) and
	   stderr (2) to the corresponding pipe ends. */
	
	struct rlimit r_limit;
	
	dup2(pipe_io[0][0], 0);
	dup2(pipe_io[1][1], 1);
	/* Close all filedescriptors of the child process
	   except stdin, stdout and stderr, because the
	   child inherits the parents descriptors. */

	for (i = getrlimit(RLIMIT_NOFILE,&r_limit); i > 1; i--)
 	  (void) close(i);
	
	/* Reinstate all signals of the child process to
	   their default value, because the child inherits 
	   the parents signal settings. */
	for (i = 0; i < NSIG; i++)
	  (void) signal (i, SIG_DFL);
	
	// Start daVinci.
	execl(path, "daVinci", "-pipe", NULL);
	
	// Disaster has occured if we get here!
	if (errno == ENOENT)
	  cout << "error: can't start program 'receiver'." << endl;
	else
	  perror ("error: problems with program 'receiver'");
	_exit (-1);
      }
    default:			// This is program code for the parent process!
      // Close unused portions of the pipes
      close (pipe_io[0][0]); 
      close (pipe_io[1][1]);
      
      // open descriptors as streams
      // stdg++ 3.0 doesn't allow this, so the descriptors are merely used as 
      // descriptors now.


      pin = pipe_io[1][0];
      pout = pipe_io[0][1];

      //pin = new ifstream (pipe_io[1][0]);
      //pout = new ofstream (pipe_io[0][1]);
    }
}

daVinci&
daVinci::test ()
{

  char buf[] = "new_term([l(\"Object A\",n(\"anything\",[a(\"OBJECT\",\"Object A\")],[e(\"anything\",[],l(\"Object C\",n(\"anything\",[a(\"OBJECT\",\"Object C\")],[e(\"anything\",[],l(\"Object E\",n(\"anything\",[a(\"OBJECT\",\"Object E\")],[e(\"anything\",[],r(\"Object D\"))])))]))),e(\"anything\",[],l(\"Object B\",n(\"anything\",[a(\"OBJECT\",\"Object B\")],[e(\"anything\",[],l(\"Object D\",n(\"anything\",[a(\"OBJECT\",\"Object D\")],[])))])))])),l(\"Object F\",n(\"anything\",[a(\"OBJECT\",\"Object F\")],[e(\"anything\",[],r(\"Object B\"))]))])\n";

  

  pout_str(buf);

  //  (*pout) << buf << flush;
  return *this;
}

daVinci&
daVinci::debug (bool flag)
{
  debug_mode = flag;
  return *this;
}

// end of daVinci-interface.cpp
