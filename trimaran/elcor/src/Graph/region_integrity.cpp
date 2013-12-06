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
//      File:           region_integrity.cpp
//      Authors:        Marnix Arnold
//      Created:        July 1997
//      Description:    Region integrity checker
//
/////////////////////////////////////////////////////////////////////////////

#include "region_integrity.h"
#include "op.h"
#include "edge.h"
#include "edge_utilities.h"
#include "dbg.h"
#include "iterators.h"
#include "opcode_properties.h"

//
// check incoming and outgoing CONTROL0 edges and parent
//
bool Op_check_integrity(Op *theop)
{
  bool status = true;
  Edge *inedge = get_incoming_CONTROL0_edge(theop);
  Edge *outedge = get_outgoing_CONTROL0_edge(theop);
  Op *prevop = (inedge ? inedge->src() : (Op*)NULL);
  Op *nextop = (outedge ? outedge->dest() : (Op*)NULL);
  Compound_region *theparent = theop->parent();
  int opid = theop->id();
  int parentid = theparent->id();
  int previd, nextid;

  if(inedge)
    {
      if(prevop == NULL)
	{
	  cdbg << "Warning: Op " << opid << " has inedge without source.\n";
	  status = false;
	}
      else if (prevop->parent() != theparent)
	{
	  previd = prevop->id();
	  if (!is_control_merge(theop))
	    {
	      cdbg << "Warning: non-control merge op " << opid 
		   << " has predecessor op " << previd << " with different parent.\n";
	    }
	  if (!theparent->is_entry_op(theop))
	    {
	      cdbg << "Warning: op " << opid << " has predecessor op " << previd 
		   << "\n         with different parent but is not an entry op of region " 
		   << parentid << ".\n";
	    }
	  if (!theparent->is_entry_edge(inedge))
	    {
	      cdbg << "Warning: op-" << opid << " has predecessor op-" << previd 
		   << "\n         with different parent but edge is not an entry edge of region-"
		   << parentid << ".\n";
	    }
	  status = false;
	}
    }
  else
    {
      cdbg << "Warning: Op " << opid << " has no incoming CONTROL0 edge.\n";
      status = false;
    }
  
  if(outedge)
    {
      if(nextop == NULL)
	{
	  cdbg << "Warning: Op " << opid << " has outedge without destination.\n";
	  status = false;
	}
      else if (nextop->parent() != theparent)
	{
	  nextid = nextop->id();
	  if (!is_branch(theop))
	    {
	      cdbg << "Warning: non-branch op " << opid 
		   << " has successor op " << nextid << " with different parent.\n";
	    }
	  if (!theparent->is_exit_op(theop))
	    {
	      cdbg << "Warning: op " << opid << " has successor op " << nextid 
		   << "\n         with different parent but is not an exit op of region " 
		   << parentid << ".\n";
	    }
	  if (!theparent->is_exit_edge(outedge))
	    {
	      cdbg << "Warning: op-" << opid << " has successor op-" << nextid 
		   << "\n         with different parent but edge is not an exit edge of region-"
		   << parentid << ".\n";
	    }
	  status = false;
	}
    }
  else
    {
      cdbg << "Warning: Op " << opid << " has no outgoing CONTROL0 edge.\n";
      status = false;
    }

  if( !theparent->subregions().is_member(theop) )
    {
      cdbg << "Warning: Op " << opid << " is not a subregion of its parent.\n";
      status = false;
    }

  return status;
}

//
// check reciprocity of incoming edges and ops at BB/HB/Proc level
//
bool Inedge_check_integrity(Edge *inedge, Compound_region *thecompound)
{
  bool status = true;
  Op *srcop, *destop;
  srcop = inedge->src();
  destop = inedge->dest();
  int compid = thecompound->id();
  Compound_region *srcparent;

  if(srcop)
    {
      srcparent = srcop->parent();
      if(!srcparent->is_exit_op(srcop))
	{
	  cdbg << "Warning: in region " << compid << ": source op " 
	       << srcop->id() << " of entry edge is not an exit op\n"
	       << "         of region " << srcparent->id() << ".\n";
	  status = false;
	}
      if(!srcparent->is_exit_edge(inedge))
	{
	  cdbg << "Warning: entry edge of region " << compid 
	       << " is not an exit edge of region " << srcparent->id()
	       << ",\n         which is parent of edge source op.\n";
	  status = false;
	}
    }
  else
    {
      cdbg << "Warning: region " << compid
	   << " has an entry edge without a source op.\n";
      status = false;
    }
  
  if(destop)
    {
      if(!thecompound->is_entry_op(destop))
	{
	  cdbg << "Warning: in region " << compid << ": destination op " 
	       << destop->id() << " of entry edge is not an entry op.\n";
	  status = false;
	}
    }
  else
    {
      cdbg << "Warning: region " << thecompound->id() 
	   << " has an entry edge without a destination op.\n";
      status = false;
    }

  return status;
}

//
// check reciprocity of outgoing edges and ops at BB/HB/Proc level
//
bool Outedge_check_integrity(Edge *outedge, Compound_region *thecompound)
{
  bool status = true;
  Op *srcop, *destop;
  srcop = outedge->src();
  destop = outedge->dest();
  int compid = thecompound->id();
  Compound_region *destparent;

  if(destop)
    {
      destparent = destop->parent();
      if(!destparent->is_entry_op(destop))
	{
	  cdbg << "Warning: in region " << compid << ": destination op " 
	       << destop->id() << " of exit edge is not an entry op\n"
	       << "         of region " << destparent->id() << ".\n";
	  status = false;
	}
      if(!destparent->is_entry_edge(outedge))
	{
	  cdbg << "Warning: exit edge of region " << compid 
	       << " is not an entry edge of region " << destparent->id()
	       << ",\n         which is parent of edge destination op.\n";
	  status = false;
	}
    }
  else
    {
      cdbg << "Warning: region " << compid
	   << " has an exit edge without a destination op.\n";
      status = false;
    }
  
  if(srcop)
    {
      if(!thecompound->is_exit_op(srcop))
	{
	  cdbg << "Warning: in region " << compid << ": source op " 
	       << srcop->id() << " of exit edge is not an exit op.\n";
	  status = false;
	}
    }
  else
    {
      cdbg << "Warning: region " << thecompound->id() 
	   << " has an exit edge without a source op.\n";
      status = false;
    }

  return status;
}


//
// check entry/exit edges/ops and hierarchy
//
bool Compound_check_integrity(Compound_region *thecompound)
{
  bool status = true;

  // check entry/exit edge integrity
  for(Region_entry_edges ined(thecompound); ined != 0; ined++)
    Inedge_check_integrity(*ined, thecompound);

  for(Region_exit_edges outed(thecompound); outed != 0; outed++)
    Outedge_check_integrity(*outed, thecompound);

  // recurse for all subregions
  for(Region_subregions subit(thecompound); subit != 0; subit++)
    status = status && Region_check_integrity(*subit);

  return status;
}

//
// Check the structural integrity of a region, recursively
//
bool Region_check_integrity(Region *theregion)
{
  bool status = true;

  if(theregion->is_op())
    return Op_check_integrity((Op*)theregion);

  if(theregion->is_compound()) // BB or HB or Procedure
    return Compound_check_integrity((Compound_region*)theregion);

  return status;
}
