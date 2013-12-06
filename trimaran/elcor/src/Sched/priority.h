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
//      File:           priority.h
//      Authors:        Santosh G. Abraham, Vinod Kathail
//			Brian Dietrich, Srini Mantripragada
//      Created:        April 1994
//      Description:    Priority class and derived classes for op priority
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _PRIORITY_
#define _PRIORITY_

#include "defs.h"
#include "filter.h"
#include "graph_filters.h"
#include "region.h"
#include "relax_dep_constr.h"
#include "sched_filters.h"
#include "slist.h"
#include "op_info.h"

#include "el_io.h"
const int NUM_DELAY_SLOTS = 2;
//	#define DELAY_SLOT_STATS 1
//	#define NO_FILL_DELAY_SLOTS 1

class Priority {
public:
 Priority(Compound_region* region, const Relax_dep_constr* relax_dep = NULL, 
    const Op_filter* ofilter = ALL_OPS, const Edge_filter* efilter = ALL_EDGES,
    bool delete_flag = true);
 virtual ~Priority();
 virtual void initialize();

 virtual bool is_early() const;
 virtual bool is_list() const;
 virtual bool is_dep_pri() const;
 virtual bool is_dep_pri_bt() const;
 virtual bool is_pri_bt() const;
 virtual bool is_slack() const;
 virtual bool is_topo() const;

 virtual bool operator==(const void*) const = 0;
 virtual bool operator!=(const void*) const = 0;

 virtual void operator++() = 0;
 virtual Op* operator*() = 0;

 virtual void reschedule(Op* op, Compound_region* reg) = 0;
 const Relax_dep_constr* get_relax_constr_function();

protected:
 Compound_region* reg;
 const Relax_dep_constr* relax_rep;
 const Op_filter* ofilter_rep;
 const Edge_filter* efilter_rep;
 bool delete_rep;
 List<Op*> oplist;
 List<Op*> ready_list;
 List_iterator<Op*> iter;
 Op* nextop;
 int time;
 void get_ready_list(const List<Op*>& op_list, 
                          const Op_filter* filter = ALL_OPS, int time = 0);
 void create_op_list(Compound_region* region, const Op_filter* filter = ALL_OPS);
 void op_list_print(List<Op*> plist);
 void op_slist_print(Slist<Stuple<double, Op* > > plist);
};

class Early : public Priority {
public:
 Early(Compound_region* region, const Relax_dep_constr* relax_dep = NULL, 
   const Op_filter* ofilter = ALL_OPS, const Edge_filter* efilter = ALL_EDGES,
   bool delete_flag = true);
 ~Early();
 void initialize();

 bool is_early() const;
 bool operator==(const void*) const ;
 bool operator!=(const void*) const ;

 void operator++();
 Op* operator*();
 void reschedule(Op* op, Compound_region* reg);
private:
 void get_next_ready_op();
};

class Listt : public Priority {
public:
 Listt(Compound_region* region, const Relax_dep_constr* relax_dep = NULL, 
   const Op_filter* ofilter = ALL_OPS, const Edge_filter* efilter = ALL_EDGES,
   const Edge_filter* eoutfilter = ALL_EDGES,
   bool delete_flag = true);
 ~Listt();
 void initialize();

 bool is_list() const;
 bool operator==(const void*) const ;
 bool operator!=(const void*) const ;

 void operator++();
 Op* operator*();
 void reschedule(Op* op, Compound_region* reg);
private:
 void create_op_list(Compound_region* region);
 void get_ready_list();
 void get_next_ready_op();
 void update_info();
 Slist<Stuple<double, Op*> > op_slist; 
 Slist<Stuple<double, Op*> > ready_slist;
 Slist<Stuple<double, Op*> > data_ready_slist;
 const Edge_filter* eoutfilter_rep;
};

class Dep_pri : public Priority {
public:
 Dep_pri(Compound_region* region, const Relax_dep_constr* relax_dep = NULL, 
   const Op_filter* ofilter = ALL_OPS, const Edge_filter* efilter = ALL_EDGES,
   const Edge_filter* eoutfilter = ALL_EDGES,
   bool delete_flag = true);
 ~Dep_pri();
 void initialize();

 bool is_dep_pri() const;
 bool operator==(const void*) const ;
 bool operator!=(const void*) const ;

 void operator++();
 Op* operator*();
 void reschedule(Op* op, Compound_region* reg);
protected:
 Slist<Stuple<double, Op*> > ready_slist;
 double max_stat_priority;
 double calculate_dyn_priority (Op* op, OpInfo* info);
 const Edge_filter* eoutfilter_rep;
};

class Dep_pri_bt : public Dep_pri {
public:
 Dep_pri_bt(Compound_region* region, const Relax_dep_constr* relax_dep = NULL, 
   const Op_filter* ofilter = ALL_OPS, const Edge_filter* efilter = ALL_EDGES,
   const Edge_filter* eoutfilter = ALL_EDGES,
   bool delete_flag = true);
 ~Dep_pri_bt();
 void initialize();

 bool is_dep_pri() const;
 bool is_dep_pri_bt() const;
 void operator++();
 void reschedule(Op* op, Compound_region* reg);
};

class Pri_bt : public Priority {
public:
 Pri_bt(Compound_region* region, const Relax_dep_constr* relax_dep = NULL, 
   const Op_filter* ofilter = ALL_OPS, const Edge_filter* efilter = ALL_EDGES,
   const Edge_filter* eoutfilter = ALL_EDGES,
   bool delete_flag = true);
 ~Pri_bt();
 void initialize();

 bool is_pri_bt() const;
 bool operator==(const void*) const ;
 bool operator!=(const void*) const ;

 void operator++();
 Op* operator*();
 void reschedule(Op* op, Compound_region* reg);
private:
 Slist<Stuple<double, Op*> > unsched_slist;
 const Edge_filter* eoutfilter_rep;
};

class Slack : public Priority {
public:
 Slack(Compound_region* region, const Relax_dep_constr* relax_dep = NULL, 
   const Op_filter* ofilter = ALL_OPS, const Edge_filter* efilter = ALL_EDGES,
   bool delete_flag = true);
 ~Slack();
 void initialize();

 bool is_slack() const;
 bool operator==(const void*) const ;
 bool operator!=(const void*) const ;

 void operator++();
 Op* operator*();
 void reschedule(Op* op, Compound_region* reg);
private:
 void get_next_ready_op();
};

class Topo : public Priority {
public:
 Topo(Compound_region* region, const Relax_dep_constr* relax_dep = NULL, 
   const Op_filter* ofilter = ALL_OPS, const Edge_filter* efilter = ALL_EDGES,
   bool delete_flag = true);
 ~Topo();
 void initialize();

 bool is_topo() const;
 bool operator==(const void*) const;
 bool operator!=(const void*) const;

 void operator++();
 Op* operator*();
 void reschedule(Op* op, Compound_region* reg);
private:
 void forward();
};

#endif
