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
//      File:           partition_graph_bv.cpp
//      Authors:        Richard Johnson
//      Created:        March 1995
//      Description:    Partition graph data structure, bitvector 
//                      implementation 
//
///////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// The partition graph implementation has the following goals/features:
//   o robustness - runs on *any* predicated code. (not yet!)
//   o accuracy - represents predicate relations found in if-converted code.
//       - understands unconditional and OR-style cmpp ops produced
//         by if-conversion.
//       - yields conservative results for conditional, AND-style, and
//         non-disjoint OR-style cmpp code.
//
/////////////////////////////////////////////////////////////////////////////

#include "predicate_map.h"
#include "partition_graph_bv.h"
#include "dbg.h"

//-----------------------------------------------------------------------------
PartitionGraph_bv::PartitionGraph_bv (PredicateMap* pmap)
{
  _sym_count = pmap->sym_count();
  _max_sym = (((_sym_count-1) >> 5) + 1) << 5;  // round up to nearest 32x

  _sym_false = pmap->false_sym();
  _sym_true = pmap->true_sym();

  sw_list = pmap->get_sw_list();
  mg_list = pmap->get_mg_list();

  // resize all bitvectors, bit-matrices
  resize(_max_sym);

  preprocess();
}

PartitionGraph_bv::~PartitionGraph_bv (void)
{
  // delete partitions; everything else gets free'd automatically
  for (List_iterator<Partition*> pi(part_list); pi != 0; pi++) {
    delete *pi;
  }
}

int
PartitionGraph_bv::sym_count (void) const
{
  return _sym_count;
}

int
PartitionGraph_bv::false_sym (void) const
{
  return _sym_false;
}

int
PartitionGraph_bv::true_sym (void) const
{
  return _sym_true;
}

// is z a subset of p ?
bool
PartitionGraph_bv::is_subset(int z, int p) const
{
  return subset[p].bit(z);
}

// are p and q disjoint ?
bool
PartitionGraph_bv::is_disjoint(int p, int q) const
{
  return disjoint[p].bit(q);
}

int
PartitionGraph_bv::lca (int m, int n) const
{
  // compute all common supersets of m and n
  Bitvector ss = superset[m];
  ss *= superset[n];

  // lca(m,n) is superset with max level
  int lca = -1;
  int lvl = -1;
  for (int q=0; q<_sym_count; q++) {
    if (ss.bit(q)) {
      if (level[q] > lvl) {
	lca = q;
	lvl = level[q];
      }
    }
  }
  assert (lvl >= 0);
  return lca;
}

int
PartitionGraph_bv::lca (const Bitvector& bv) const
{
  int lca_bv = false_sym();

  for (Bit_iterator bi(bv); bi != 0; bi++) {
    int p = *(bi);

    if (lca_bv == false_sym()) {
      lca_bv = p;
    } else {
      lca_bv = lca(lca_bv, p);
    }
  }
  return lca_bv;
}

// px = lub (px + zz)
void
PartitionGraph_bv::lub_sum (Bitvector& bv, int z) const
{
  if (bv.bit(z)) return;  // return if z already in bv

  // return if any superset of z already in bv
  Bitvector bc = bv;
  bc *= superset[z];
  if (bc.ones_count()) return;

  // otherwise, add subsets of z and then reduce.
  bv += subset[z];
  reduce(bv);
}

// px = lub (px - zz)
void
PartitionGraph_bv::lub_diff (Bitvector& bv, int z) const
{
  Bitvector bc = bv;
  bv.clear();

  for (Bit_iterator bi(bc); bi != 0; bi++) {
    int p = *(bi);

    if (is_disjoint(p, z)) {
      bv.set_bit(p);
    } else if (is_subset (p, z)) {
      // do nothing
    } else if (is_subset (z, p)) {
      rel_cmpl (z, p, bv);
    } else {
      approx_diff (p, z, bv);
      if (dbg(pqs)) cdbg << "approx!" << endl;
    }
  }
  reduce(bv);
}

// px = glb (px - zz)
// SAM 8-2000, added.  This is a conservative implementation.  Handles
// the case when p and z are disjoint or z is a subset of p.  Otherwise
// assume the diff is false.
void
PartitionGraph_bv::glb_diff (Bitvector& bv, int z) const
{
  Bitvector bc = bv;
  bv.clear();

  for (Bit_iterator bi(bc); bi != 0; bi++) {
    int p = *(bi);

    if (is_disjoint(p, z)) {
      bv.set_bit(p);
    } else if (is_subset (z, p)) {
      rel_cmpl (z, p, bv);
    } else {
      // do nothing (assume the result is false)
    }
  }
  reduce(bv);
}

/* rel_cmpl(p, q, bv) computes relative complement of p wrt q and ORs the
 * result with bv.
 */
void
PartitionGraph_bv::rel_cmpl(int p, int q, Bitvector& bv) const
{
  assert (subset[q].bit(p));  // make sure p is in q's tree

  // find tree path from q down to p, add path siblings to bv
  while (q != p) {
    // find partition of q containing p
    for (List_iterator<Partition*> dni(down[q]); dni != 0; dni++) {
      Partition* pp = *dni;
      int m = pp->part(0); int n = pp->part(1);
      if (subset[m].bit(p)) {
	bv.set_bit(n);
	q = m;
	break;
      } else if (subset[n].bit(p)) {
	bv.set_bit(m);
	q = n;
	break;
      }
    }
  }
}

void
PartitionGraph_bv::approx_diff (int p, int q, Bitvector& bv) const
{
  // there are many choices here; for example, we could return p
  rel_cmpl(q, lca(p, q), bv);
}

//-----------------------------------------------------------------------------
void
PartitionGraph_bv::resize (int len)
{
  up.resize(len);
  down.resize(len);
  leaves.resize(len);
  level.resize(len);

  // resize bit-matrices
  subset.resize(len);
  superset.resize(len);
  disjoint.resize(len);
  for (int i=0; i<len; i++) {
    subset[i].resize(len);
    superset[i].resize(len);
    disjoint[i].resize(len);
  }
}

int
PartitionGraph_bv::sym_name (void)
{
  if (_sym_count == _max_sym) {
    _max_sym += 32;
    resize(_max_sym);
  }
  subset[_sym_count].set_bit(_sym_count);
  superset[_sym_count].set_bit(_sym_count);
  return _sym_count++;
}

// a partitions into b U c
void
PartitionGraph_bv::add_partition(Partition p)
{
  Partition* pp = new Partition (p);

  part_list.add_tail(pp);

  down[pp->whole()].add_tail(pp);
  up[pp->part(0)].add_tail(pp);
  up[pp->part(1)].add_tail(pp);
}

void
PartitionGraph_bv::update_level(int a)
{
  int old_lvl = level[a];

  for (List_iterator<Partition*> upi(up[a]); upi != 0; upi++) {
    Partition* p = *upi;
    level[a] = ELCOR_MAX(level[a], level[p->whole()]+1);
  }
  if (level[a] > old_lvl) {
    for (List_iterator<Partition*> dni(down[a]); dni != 0; dni++) {
      Partition* p = *dni;
      update_level(p->part(0));
      update_level(p->part(1));
    }
  }
}

void
PartitionGraph_bv::update_superset(int a)
{
  Bitvector old_ss = superset[a];
  for (List_iterator<Partition*> upi(up[a]); upi != 0; upi++) {
      Partition* p = *upi;
      superset[a] += superset[p->whole()];
  }
  if (superset[a] != old_ss) {
    for (List_iterator<Partition*> dni(down[a]); dni != 0; dni++) {
      Partition* p = *dni;
      update_superset(p->part(0));
      update_superset(p->part(1));
    }
  }
}

void
PartitionGraph_bv::update_subset(int a)
{
  Bitvector old_ss = subset[a];
  for (List_iterator<Partition*> dni(down[a]); dni != 0; dni++) {
    Partition* p = *dni;
    subset[a] += subset[p->part(0)];
    subset[a] += subset[p->part(1)];
  }
  if (subset[a] != old_ss) {
    for (List_iterator<Partition*> upi(up[a]); upi != 0; upi++) {
      Partition* p = *upi;
      update_subset(p->whole());
    }
  }
}

void
PartitionGraph_bv::add_ready_parents (int i, const Bitvector& mark,
			List<int>& worklist) const
{
  // add ready parents to worklist
  for (List_iterator<Partition*> upi(up[i]); upi != 0; upi++) {
    Partition* p = *upi;
    int j = p->whole();
    
    // if all of j's children are marked, add j to worklist
    int ready = true;
    for (List_iterator<Partition*> ci(down[j]); ci != 0; ci++) {
      Partition* c = (Partition*) (*ci);
      if (mark.bit(c->part(0))==false || mark.bit(c->part(1))==false) {
	ready = false;
	break;
      }
    }
    if (ready) {
      worklist.add_tail(j);
    }
  }
}

void
PartitionGraph_bv::add_ready_children (int i, const Bitvector& mark,
			 List<int>& worklist) const
{
  // add ready children to worklist
  for (List_iterator<Partition*> dni(down[i]); dni != 0; dni++) {
    Partition* p = *dni;

    for (i=0; i<=1; i++) {
      int j = p->part(i);
    
      // if all of j's parents are marked, add j to worklist
      int ready = true;
      for (List_iterator<Partition*> ci(up[j]); ci != 0; ci++) {
	Partition* c = (Partition*) (*ci);
	if (mark.bit(c->whole())==false) {
	  ready = false;
	  break;
	}
      }
      if (ready) {
	worklist.add_tail(j);
      }
    }
  }
}

/* - allocates storage for level Vector & subset, superset, disjoint bit-arrays
 * - computes level numbers (these are used when completing merge partitions)
 * - processes merge partitions so that they all have a parent partition
 * - computes subset, superset, disjoint relations
 */
void
PartitionGraph_bv::preprocess(void)
{
  // process all switch partitions created by PredicateMap
  for (List_iterator<Partition> swi(*sw_list); swi != 0; swi++) {
    Partition p = *swi;
    add_partition(p);
  }

  // process all merge partitions created by PredicateMap
  // - if merge is of disjoint predicates, add partition
  // - otherwise, process merge conservatively
  //     . x = a | b  where a, b are not disjoint
  //     . find w = lca(a,b)
  //     . add partition w = x | x', where x' is a synthetic predicate
  //     . mark partition so that we'll skip it during later processing.
  for (List_iterator<Partition> mgi(*mg_list); mgi != 0; mgi++) {
    Partition p = *mgi;

    /*
    if (disjoint(p.part(0), p.part(1))) {
      add_partition(p);
    } else {
      
    }
    */
    add_partition(p);
  }

  int i;
  Bitvector mark(_max_sym);	// mark Vector, initialized to false
  List<int> worklist;
  
  // form bitvector of leaf nodes
  for (i=0; i<_sym_count; i++) {
    if (down[i].is_empty()) {
      leaves.set_bit(i);
    }
  }

  // compute node levels
  //   - use topological traversal of partition graph
  //   - at each node, level is max of parents' levels + 1
  //
  // add root nodes to worklist, marking them as we go
  mark.clear();
  for (i=0; i<_sym_count; i++) {
    if (up[i].is_empty()) {
      level[i] = 0;
      mark.set_bit(i);
      add_ready_children(i, mark, worklist);
    }
  }
  while (!worklist.is_empty()) {
    int i = worklist.pop();
    
    // i's level is max of parents' level plus one
    for (List_iterator<Partition*> upi(up[i]); upi != 0; upi++) {
      Partition* p = *upi;
      assert (p->part(0) == i || p->part(1) == i);
      level[i] = ELCOR_MAX(level[i], level[p->whole()]);
    }
    level[i] += 1;
    mark.set_bit(i);
    add_ready_children(i, mark, worklist);
  }

  //
  // compute subset relation
  //   - use reverse topological traversal of partition graph
  //   - at each node, its subsets are the union of its children's subsets
  //
  // add leaf nodes to worklist, marking them as we go
  mark.clear();
  for (i=0; i<_sym_count; i++) {
    if (down[i].is_empty()) {
      subset[i].set_bit(i);		// i is a subset of i
      mark.set_bit(i);
      add_ready_parents(i, mark, worklist);
    }
  }
  while (!worklist.is_empty()) {
    int i = worklist.pop();
    // i's subsets are union of children's subsets plus itself
    for (List_iterator<Partition*> dni(down[i]); dni != 0; dni++) {
      Partition* p = *dni;
      assert (p->whole() == i);
      subset[i] += subset[p->part(0)];
      subset[i] += subset[p->part(1)];
    }
    subset[i].set_bit(i);		// i is a subset of i
    mark.set_bit(i);
    add_ready_parents(i, mark, worklist);
  }

  //
  // compute superset relation
  //   - use topological traversal of partition graph
  //   - at each node, its supersets are the union of its parents' supersets
  //
  // add root nodes to worklist, marking them as we go
  mark.clear();
  for (i=0; i<_sym_count; i++) {
    if (up[i].is_empty()) {
      superset[i].set_bit(i);		// i is a superset of i
      mark.set_bit(i);
      add_ready_children(i, mark, worklist);
    }
  }
  while (!worklist.is_empty()) {
    int i = worklist.pop();
    // i's supersets are union of parent's supersets plus itself
    for (List_iterator<Partition*> upi(up[i]); upi != 0; upi++) {
      Partition* p = *upi;
      assert (p->part(0) == i || p->part(1) == i);
      superset[i] += superset[p->whole()];
    }
    superset[i].set_bit(i);		// i is a superset of i
    mark.set_bit(i);
    add_ready_children(i, mark, worklist);
  }

  //
  // compute disjoint relation
  //
  for (List_iterator<Partition*> pi(part_list); pi != 0; pi++) {
    int i;
    Partition* p = *pi;
    int a = p->part(0);
    int b = p->part(1);

    // subsets of a, b are disjoint
    for (i=0; i<_sym_count; i++) {
      if (subset[a].bit(i)) {
	disjoint[i] += subset[b];
      }
    }
    for (i=0; i<_sym_count; i++) {
      if (subset[b].bit(i)) {
	disjoint[i] += subset[a];
      }
    }
  }

  //
  // process merge partitions
  //
  for (List_iterator<Partition> mi(*mg_list); mi != 0; mi++) {
    Partition pt = *mi;
    int p = pt.whole();
    int m = pt.part(0);
    int n = pt.part(1);

    // compute all common supersets of p's children (excluding p)
    Bitvector ss = superset[m];
    ss *= superset[n];
    ss -= superset[p]; // exclude p and supersets of p

    // lca(m,n) is superset with max level
    int lca = -1;
    int lvl = -1;
    for (int q=0; q<_sym_count; q++) {
      if (ss.bit(q)) {
	if (level[q] > lvl) {
	  lca = q;
	  lvl = level[q];
	}
      }
    }
    assert (lvl >= 0);

    //
    // find relative complement of p with respect to lca
    //

    // get partition of lca containing both m, n
    Bitvector rc(_max_sym);
    for (List_iterator<Partition*> ci(down[lca]); ci != 0; ci++) {
      Partition* pp = *ci;
      int rr = pp->part(0);
      int ss = pp->part(1);
      if (subset[rr].bit(m) && subset[ss].bit(n)) {
	rel_cmpl(m, rr, rc);
	rel_cmpl(n, ss, rc);
	break;
      } else if (subset[rr].bit(n) && subset[ss].bit(m)) {
	rel_cmpl(m, ss, rc);
	rel_cmpl(n, rr, rc);
	break;
      }
    }
    reduce(rc);

    int whole = lca;
    int p0 = p;
    int xi = rc.ones_count();  // remaining nodes in rel.cmpl
    int x = -1; // holds current node in rel.cmpl
    List<int> new_nodes;
    assert (xi >= 1);
    do {
      // get next node in rel.cmpl
      do {
	// increment before test, so this will work on subsequent iterations
	x++;
      }	while (!rc.bit(x));
      xi--;

      if (xi > 0) { // if not last node in rel.cmpl.
	int y = sym_name(); // create node for chaining q's together
	new_nodes.add_tail(y);

	Partition p (whole, p0, y);
	add_partition(p);

	whole = y;
	p0 = x;
      } else {
	Partition p (whole, p0, x);
	add_partition (p);
      }
    }
    while (xi);

    // update levels
    update_level(p);
    if (!new_nodes.is_empty()) {
      update_level(new_nodes.head());
    }

    // update supersets
    update_superset(p);
    if (!new_nodes.is_empty()) {
      update_superset(new_nodes.head());
    }

    // update subsets
    if (!new_nodes.is_empty()) {
      update_subset(new_nodes.tail());
    }
    update_subset(lca);

    // update disjoint relation for parts of each new partition
    for (List_iterator<Partition*> pi(part_list); pi != 0; pi++) {
      Partition* p = *pi;
      int a = p->part(0);
      int b = p->part(1);
      
      if (!disjoint[a].bit(b)) {
	int i;

	// subsets of a, b are disjoint
	for (i=0; i<_sym_count; i++) {
	  if (subset[a].bit(i)) {
	    disjoint[i] += subset[b];
	  }
	}
	for (i=0; i<_sym_count; i++) {
	  if (subset[b].bit(i)) {
	    disjoint[i] += subset[a];
	  }
	}
      }
    }
  }

  // adjust all bitvectors and bit-matrices to correct length (no extra bits)
  resize(_sym_count);
}



void
PartitionGraph_bv::test(void) const
{
}

void
PartitionGraph_bv::reduce (Bitvector& bv) const
{
//  cerr << bv;
  // add in parent of marked partitions
  bool change;
  do {
    change = false;
    for (List_iterator<Partition*> pi(part_list); pi != 0; pi++) {
      Partition* p = *pi;
      int n = p->whole();
      if (!bv.bit(n) && bv.bit(p->part(0)) && bv.bit(p->part(1))) {
	bv.set_bit(n);
	change = true;
      }
    }
  } while (change);

  // remove all nodes having some superset marked
  for (int n=0; n<_sym_count; n++) {
    if (bv.bit(n)) {
      bv -= subset[n];
      bv.set_bit(n);  // don't subtract n itself
    }
  }
}

//-----------------------------------------------------------------------------
void
PartitionGraph_bv::print(ostream& os) const
{
  os << "- PartitionGraph-bv = <";
  os << " >" << endl;
}

// end of partition_graph_bv.cpp
