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




///////////////////////////////////////////////////////////////////////////
//
//      File:           dep_len.cpp
//      Authors:        Suren Talla
//      Created:        June 1997
//      Description:    definitions for DepLen class members
//
///////////////////////////////////////////////////////////////////////////



#include <fstream>
#include "dep_len.h"
#include "attributes.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "intf.h"
#include "opcode.h"
#include "dbg.h"
#include "el_data_cpr_init.h"

using namespace std;

//######################################################################
//       DepLen_OpInfo
//######################################################################
inline 
DepLen_OpInfo::DepLen_OpInfo(int dim){ 
  int v = ELCOR_MAX_INT; 
  if (dim>0) m_Lstart.resize(dim, v); 
  if (dim>0) m_Slack.resize(dim, v); 
  m_Estart = -1; 
}

int DepLen_OpInfo::min_lstart(){
  int s = ELCOR_MAX_INT;
  for(unsigned i=0; i<m_Lstart.size(); i++){
    if (m_Lstart[i] == -1) continue;
    s = (s < m_Lstart[i]) ? s : m_Lstart[i];
  }
  return s;
}

inline void 
DepLen_OpInfo::set_slack(int slack){
  //m_Slack = slack;
}


ostream& operator<<(ostream& os, DepLen_OpInfo& dp)
{
    os << "estart = " << dp.m_Estart << endl;
    os << "lstart = <";
    for(unsigned i=0; i<(dp.m_Lstart.size() -1); i++)
	os << dp.m_Lstart[i] << ",";
    os << dp.m_Lstart[dp.m_Lstart.size() -1] << ">\n";
    os << "slack = <";
    for(unsigned i=0; i<(dp.m_Slack.size() -1); i++)
	os << dp.m_Slack[i] << ",";
    os << dp.m_Slack[dp.m_Slack.size() -1] << ">\n";
    return os;

}
//######################################################################
//       OpPtr_Double
//######################################################################
bool OpPtr_Double::operator==(const OpPtr_Double& opp_dbl) const {
    return ((op==opp_dbl.op) && (deplen==opp_dbl.deplen));
}

bool OpPtr_Double::operator!=(const OpPtr_Double& opp_dbl) const {
    return !operator==(opp_dbl);
}


//######################################################################
//       DepLen
//######################################################################
DepLen::DepLen() 
{  
}
//----------------------------------------------------------------------
DepLen::~DepLen()
{
}

//----------------------------------------------------------------------
void 
DepLen::init_opinfo_map(Compound_region* cr, int num_exitops, 
			Hash_map<Op*, DepLen_OpInfo*>& hOpInfo)
{
    //for(Region_ops_C0_order ops(cr); ops != 0; ops++){
    for(Region_all_ops ops(cr); ops != 0; ops++){
	Op *op = *ops;
	if (!hOpInfo.is_bound(op)){
	    DepLen_OpInfo* cinfo = new DepLen_OpInfo(num_exitops);
	    hOpInfo.bind(op, cinfo);
	}
    }  
}

//----------------------------------------------------------------------
void 
DepLen::clear_opinfo_map(Compound_region* cr,
			 Hash_map<Op*, DepLen_OpInfo*>& hOpInfo)
{
    //for(Region_ops_C0_order ops(cr); ops != 0; ops++){
    for(Region_all_ops ops(cr); ops != 0; ops++){
	Op *op = *ops;
	if (hOpInfo.is_bound(op)){
	    DepLen_OpInfo* d = hOpInfo.value(op);
	    delete d;
	    hOpInfo.unbind(op);
	}
    }  
}
//----------------------------------------------------------------------
void 
DepLen::get_estart(Compound_region* cr, Hash_map<Op*, DepLen_OpInfo*>& hOpInfo)
{
    int firstop = true;
    DepLen_OpInfo* cinfo;

    int num_exitops = 0;
    for(Region_exit_ops eops(cr); eops != 0; eops++){
	num_exitops++;
    }

    //init the hash map structure;
    //for(Region_ops_C0_order ops1(cr); ops1 != 0; ops1++){
    for(Region_all_ops ops1(cr); ops1 != 0; ops1++){
	Op *op = *ops1;
	if (!hOpInfo.is_bound(op)){
	    DepLen_OpInfo* cinfo = new DepLen_OpInfo(num_exitops);
	    hOpInfo.bind(op, cinfo);
	}
    }  

    for(Region_ops_C0_order ops(cr); ops != 0; ops++){
	Op *op = *ops;

	if (firstop){
	    if (!is_control_merge(op))
		El_punt("Found a non-control merge entry op");
	    //skip the first op since its estart will always be = 0;
	    //and we assume there is only a single entry;
	    cinfo = hOpInfo.value(op);
	    cinfo->m_Estart = 0;
	    firstop = false;
	    continue;
	}

	int max = -1;
	//the Op_inedges_rel iterates over the Op_inedges as well as inedges;
	//of related ops of the op;
        Op_inedges_rel  iedges(op, NON_CONTROL0_INEDGES);

        // 8-24-05 mchu
        // changed to set the estart as 0 if there are no inedges
        if(iedges == 0 && !is_pseudo(op)) 
          max = 0;

	for(; iedges != 0; iedges++){
	    Edge *edge = *iedges;
	    Op* src_op = real_op(edge->src());
	    //int tmax = m_OpInfo.value(src_op)->m_Estart + edge->latency();
	    int tmax = hOpInfo.value(src_op)->m_Estart + edge->latency();
	    max = (max > tmax) ? max : tmax;
	}
	
	//cinfo = m_OpInfo.value(op);
	cinfo =hOpInfo.value(op);
	cinfo->m_Estart = max;
    }
}

//----------------------------------------------------------------------
void 
DepLen::get_slack(Compound_region* cr, Hash_map<Op*, DepLen_OpInfo*>& hOpInfo)
{
    int num_exitops = 0;
    for(Region_exit_ops eops(cr); eops != 0; eops++)
	num_exitops++;

    for (Region_ops_C0_order ops(cr); ops != 0; ops++){
	Op* op = *ops;
	//int minl = hOpInfo.value(op)->min_lstart(num_exitops); 
	//assert(minl>=0);
	//int slack = minl - hOpInfo.value(op)->m_Estart;
	int estart = hOpInfo.value(op)->m_Estart;
	for(int i=0; i<num_exitops; i++){
	    int slack = hOpInfo.value(op)->m_Lstart[i] - estart;
	    assert((slack >= 0) && 
		   "you probably forgot to draw dataflow edges if you're here");
	    hOpInfo.value(op)->m_Slack[i] = slack;
	}
    }
  
}

//----------------------------------------------------------------------
int 
DepLen::init_exitops_vector(Compound_region* cr, int& num_exitops,
			    Vector<Op*>* exitops_vec)
{
    num_exitops = 0;
    for(Region_exit_ops ops(cr); ops != 0; ops++){
	num_exitops++;
    }
    exitops_vec->resize(num_exitops); 

    num_exitops = 0;
    for(Region_exit_ops eops(cr); eops != 0; eops++){
	Op* op = *eops;
	(*exitops_vec)[num_exitops] = op;
	num_exitops++;
    }
    
    return num_exitops;
}

//----------------------------------------------------------------------
void 
DepLen::get_lstart(Compound_region* cr,
		   Hash_map<Op*, DepLen_OpInfo*>& hOpInfo)
{

    //for each exitop do - compute lstart times with respect to that;
    //exit op for all ops in region;
    
    DepLen_OpInfo* cinfo;
    int num_exitops = 0;
    Vector<Op*>* exitops_vec = new Vector<Op*>;
    int max_estart = 0;

    for(Region_exit_ops ops(cr); ops != 0; ops++)
	num_exitops++;
    exitops_vec->resize(num_exitops); 

    num_exitops = 0;
    for(Region_exit_ops eops(cr); eops != 0; eops++){
	Op* op = *eops;
	(*exitops_vec)[num_exitops] = op;
	num_exitops++;
    }
    for(Region_all_ops ops1(cr); ops1 != 0; ops1++){
	Op *op = *ops1;
        if(hOpInfo.value(op)->m_Estart > max_estart) {          
          max_estart = hOpInfo.value(op)->m_Estart;
        }
    }  

    for(int exitop_idx=0; exitop_idx<num_exitops; exitop_idx++){
	Op* exitop = (*exitops_vec)[exitop_idx];
	for(Region_ops_reverse_C0_order ops(cr); ops != 0; ops++){
	    Op *op = *ops;
	    cinfo = hOpInfo.value(op);
	    
	    if (exitop==op){
		cinfo->m_Lstart[exitop_idx] = hOpInfo.value(exitop)->m_Estart;
		continue;
	    }
	    
	    int tmin = ELCOR_MAX_INT, lmin = ELCOR_MAX_INT;

            // 8-24-05 mchu
            // changed to set the lstart as the max_estart if there are no outedges
            Op_outedges_rel oe(op, NON_CONTROL0_OUTEDGES);
            if(oe == 0 && !is_pseudo(op)) 
              lmin = max_estart;
	    
	    for(; oe != 0; oe++){
		Op* dest_op = real_op((*oe)->dest());
		if (hOpInfo.is_bound(dest_op)){
		    tmin = hOpInfo.value(dest_op)->m_Lstart[exitop_idx] - (*oe)->latency();
		    lmin = (tmin < lmin) ? tmin : lmin;
		} else {
		    cdbg << "DepLen::get_lstart dest_op_" << dest_op->id()
			 << "  not bound??" << endl;
		}
	    }
      
	    if (lmin < ELCOR_MAX_INT){
		cinfo->m_Lstart[exitop_idx] = lmin;
	    }
	}
    }
    delete exitops_vec;
}

//----------------------------------------------------------------------
double 
DepLen::max_dep_len(Compound_region* cr, 
		    List<OpPtr_Double>& exitops_deplen)
{
   
    Hash_map<Op*, DepLen_OpInfo*>  hOpInfo(hash_op_ptr, INIT_HASH_MAP_SIZE);

    double mdep_len = .0;

    for(Region_ops_C0_order ops(cr); ops != 0; ops++){
	Op *op = *ops;
	if (!hOpInfo.is_bound(op)){
	    //the Lstart Vector is not needed for max_dep_len calcn;
	    DepLen_OpInfo* cinfo = new DepLen_OpInfo(0);
	    hOpInfo.bind(op, cinfo);
	}
    }  
    
    DepLen::get_estart(cr, hOpInfo);

    mdep_len = -1;
    for(Region_exit_ops eops(cr); eops != 0; eops++){
	Op* op = *eops;
	double cur_val = hOpInfo.value(op)->m_Estart;
	mdep_len = (mdep_len < cur_val) ? cur_val : mdep_len;
    }
    
    for(Region_ops_C0_order cops(cr); cops != 0; cops++){
	Op *op = *cops;
	if (!hOpInfo.is_bound(op)){
	    hOpInfo.unbind(op);
	}
    }  
    return mdep_len;
}


//----------------------------------------------------------------------
double 
DepLen::weighted_dep_len(Compound_region* cr,
			 List<OpPtr_Double>& exitops_deplen)
{

    Hash_map<Op*, DepLen_OpInfo*>  hOpInfo(hash_op_ptr, INIT_HASH_MAP_SIZE);
    
    double wdep_len = .0;
    
    for(Region_ops_C0_order ops(cr); ops != 0; ops++){
	Op *op = *ops;
	if (!hOpInfo.is_bound(op)){
	    //the Lstart Vector is not needed for max_dep_len calcn;
	    DepLen_OpInfo* cinfo = new DepLen_OpInfo(0);
	    hOpInfo.bind(op, cinfo);
	}
    }  
    
    DepLen::get_estart(cr, hOpInfo);
    double cum_dep_len = .0;
    double total_freq = 0;

    for(Region_exit_edges exit_edges(cr); exit_edges != 0; exit_edges++){
	Edge* e = *exit_edges;
	Op* op = e->src();
	Control_flow_freq *cfreq = get_control_flow_freq(e);
	total_freq += cfreq->freq;
	cum_dep_len += (hOpInfo.value(op)->m_Estart * cfreq->freq);
    }
    
    wdep_len = (total_freq>0) ? (cum_dep_len/total_freq) : 0;
    
    DepLen::clear_opinfo_map(cr, hOpInfo);
    return wdep_len;
}



//----------------------------------------------------------------------
double 
DepLen::dep_len_estimate(Compound_region* rgn)
{
    // right now the method is a fixed constant 0 denoting that;
    // edge latency was used as the method to calculate the ;
    // dependence distances and not some other measure;

    List<OpPtr_Double> exitops_deplen;
    switch(El_deplen_estimate_method){
      case MAX_DEP:
	return max_dep_len(rgn, exitops_deplen);
      case WEIGHTED_DEP:
	return weighted_dep_len(rgn, exitops_deplen);
      default:
	break;
    }
    return 0;
}

//---------------------------------------------------------------;
double 
DepLen::dep_len_estimate(Compound_region* rgn, 
			 List<OpPtr_Double>& exitops_deplen)
{
    // right now the method is a fixed constant 0 denoting that;
    // edge latency was used as the method to calculate the ;
    // dependence distances and not some other measure;

    switch(El_deplen_estimate_method){
      case MAX_DEP:
	return max_dep_len(rgn, exitops_deplen);
      case WEIGHTED_DEP:
	return weighted_dep_len(rgn, exitops_deplen);
      default:
	break;
    }
    return 0;

}
