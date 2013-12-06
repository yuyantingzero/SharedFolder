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
//      File:           rr_life.h
//      Authors:        Alexandre Eichenberger
//      Created:        June 1994
//      Description:    lifetime analysis class definition
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ROTREG_LIFE_H  
#define _ROTREG_LIFE_H

// for rotating registers
#include "reference.h"


////////////////////////////////////////////////////
// class RR_ref_data
//   contains the data associated with a define or use
//   used by class RR_lifetime to generate start_time...
////////////////////////////////////////////////////

class RR_ref_data {
  friend ostream& operator<< (ostream&, RR_ref_data&);
public:
  RR_ref_data();
  RR_ref_data(Op*, Operand*, Port_type, Port_num, Port_dir);
  RR_ref_data(const RR_ref_data&); 
  ~RR_ref_data();
  RR_ref_data& operator= (const RR_ref_data&);
  bool operator== (const RR_ref_data&) const;
  bool operator!= (const RR_ref_data&) const;

  void  Init_times(const int II);
  void  Init_open_loop_times();
  void 	Commit(const int II, const int location, const int reg_num);

// not redundant, because of the exception done for BRTOP 
// and because of the loop-carried references
  int		sched_time, issue_time, flow_time, anti_time;
  int 		evr;  	

// data
  Reference     ref;
  Port_dir      ref_dir;
  Operand*	operand_ptr;
  bool          predicated_def;  // valid for def op only
  Hash_set<int>	reaching_def;	// vaild for use op only

// results
  int		iter_apart; 
  int		mc_loc;

private:
  void Init();
};


////////////////////////////////////////////////////
// class RR_lifetime
//   contains the data associated to each lifetime within
//   a loop. 
////////////////////////////////////////////////////

class RR_lifetime {
  friend ostream& operator<< (ostream&, RR_lifetime&);
public:
  RR_lifetime();
  ~RR_lifetime();
  RR_lifetime& operator= (const RR_lifetime&); 
  bool 	operator== (const RR_lifetime&) const;
  bool 	operator!= (const RR_lifetime&) const;

  void  Init_open_loop();
  void 	Process_input_data(int II, int SC, int theta, int PS, int ES);
  void  Process_open_loop_data(int II, int SC, int theta, int PS, int ES);
  void	Commit(const int, const int);

// input data 
  Vector<RR_ref_data> 	defs;
  Vector<RR_ref_data> 	uses;
  int			defs_num, uses_num;
  int           	liveout_omega;   // max omega among all liveouts 
  /* RMR { flag is set to true when the lifetime is defined by a scalar-to-vector move op (e.g., SMOVE) */
  bool svmove_def;
  /* } RMR */


// processed data 
  int			base_evr;
  int 			start_time;
  int 			stop_time;
  int 			omega;
  int           	alpha;
  int 			first, k_first;
  int 			last, k_last;
  bool                  leading_blade;
  bool                  trailing_blade;
  bool                  all_predicated_defs; 

// results
  int 			location;
  bool 			allocated;

private: 
  void	Init();  
};

#endif





