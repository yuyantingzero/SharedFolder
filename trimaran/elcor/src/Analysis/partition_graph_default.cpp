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
//      File:          partition_graph_default.cpp
//      Authors:       Richard Johnson 
//      Created:       March 1995
//      Description:   Partition graph data structure, default implementation
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

#include <fstream>
#include "predicate_map.h"
#include "partition_graph_default.h"
#include "dbg.h"

using namespace std;

//-----------------------------------------------------------------------------
PartitionGraph_default::PartitionGraph_default (PredicateMap* pmap)
{
  _pmap = pmap;  // save pointer to pmap, need by remap

  _sym_count = pmap->sym_count();
  _max_sym = (((_sym_count-1) >> 5) + 1) << 5;  // round up to nearest 32x

  _sym_false = pmap->false_sym();
  _sym_true = pmap->true_sym();

  sw_list = pmap->get_sw_list();
  mg_list = pmap->get_mg_list();

  // resize all bitvectors, bit-matrices
  resize(_max_sym);

  preprocess();

  if (dbg(pqs,2)) print(cdbg);

  cache_properties();
}

PartitionGraph_default::~PartitionGraph_default (void)
{
  // delete partitions; everything else gets free'd automatically
  for (List_iterator<Partition*> pi(part_list); pi != 0; pi++) {
    delete *pi;
  }
}

int
PartitionGraph_default::sym_count (void) const
{
  return _sym_count;
}

int
PartitionGraph_default::false_sym (void) const
{
  return _sym_false;
}

int
PartitionGraph_default::true_sym (void) const
{
  return _sym_true;
}

bool
PartitionGraph_default::is_subset (int a, int b) const
{
  if (a == _sym_false || b == _sym_true || a == b) {
    return true;
  } else {
    Bitvector mark(_sym_count);
    return search_ancestors(a, b, mark);
  }
}

bool
PartitionGraph_default::is_disjoint (int a, int b) const
{
   if (a == _sym_false || b == _sym_false) return true;

   if (a == b) return false;
   
   if (ancestor_cache_valid == true) {
      Bitvector mark(cached_ancestors[a]) ;
      return disj_search (b,mark) ;
   }
   else {
      // mark all ancestors of node a
      Bitvector mark(_sym_count);
      mark_ancestors (a, mark);
      
      // search ancestors of node b for partition to prove disjointness w/node a
      return disj_search (b, mark);
   }
}

// search down from root for lca
void
PartitionGraph_default::find_lca (Bitvector& mark, int current_lvl, int current_n,
			      int& lvl, int& n) const
{
  if (mark.bit(current_n)) {
    mark.reset_bit(current_n);
    
    if (current_lvl > lvl) {
      lvl = current_lvl;
      n = current_n;
    }
    for (PG_succs pi(*this, current_n); pi != 0; pi++) {
      int p = *pi;
      find_lca (mark, current_lvl+1, p, lvl, n);
    }
  }
}

// slow implementation
int
PartitionGraph_default::lca (int m, int n) const
{
   Bitvector mark_m(_sym_count);
   
   if (ancestor_cache_valid == true) {
      mark_m = cached_ancestors[m] ;
      Bitvector mark_n = cached_ancestors[n] ;
 
      mark_m *= mark_n;
   }
   else {
      Bitvector mark_n(_sym_count);
      
      mark_ancestors (m, mark_m);
      mark_ancestors (n, mark_n);
      
      mark_m *= mark_n;
   }
   int lca_lvl = 0;
   int lca_mn = _sym_true;
   find_lca (mark_m, lca_lvl, lca_mn, lca_lvl, lca_mn);
   return lca_mn;
}

int
PartitionGraph_default::lca (const Bitvector& bv) const
{
  int lca_bv = _sym_false;

  for (Bit_iterator bi(bv); bi != 0; bi++) {
    int p = *(bi);

    if (lca_bv == _sym_false) {
      lca_bv = p;
    } else {
      lca_bv = lca(lca_bv, p);
    }
  }
  return lca_bv;
}

// px = lub (px + zz)
void
PartitionGraph_default::lub_sum (Bitvector& bv, int z) const
{
  if (is_subset(z, bv)) return;

  int r = sum_reduce(bv, z);
  if (r != z) {
    clear_subtree(bv, r);
    bv.set_bit(r);
  }
}

// px = lub (px - zz)
void
PartitionGraph_default::lub_diff (Bitvector& bv, int z) const
{
  Bitvector bc = bv;
  bv.clear();

  for (Bit_iterator bi(bc); bi != 0; bi++) {
    int p = *(bi);

    if (is_disjoint(p, z)) {
      int r = sum_reduce(bv, p);
      if (r != p) {
	clear_subtree(bv, r);
	bv.set_bit(r);
      }
    } else if (is_subset (p, z)) {
      // do nothing
    } else if (is_subset (z, p)) {
      rel_cmpl (z, p, bv);
    } else {
      approx_diff (p, z, bv);
      if (dbg(pqs)) cdbg << "approx!" << endl;
    }
  }
  check_reduce(bv);
}

// px = glb (px - zz)
// SAM 8-2000, added.  This is a conservative implementation.  Handles
// the case when p and z are disjoint or z is a subset of p.  Otherwise
// assume the diff is false.
void
PartitionGraph_default::glb_diff (Bitvector& bv, int z) const
{
  Bitvector bc = bv;
  bv.clear();

  for (Bit_iterator bi(bc); bi != 0; bi++) {
    int p = *(bi);

    if (is_disjoint(p, z)) {
      int r = sum_reduce(bv, p);
      if (r != p) {
        clear_subtree(bv, r);
        bv.set_bit(r);
      }
    } else if (is_subset (z, p)) {
      rel_cmpl (z, p, bv);
    } else {
      // do nothing (assume the result is false)
    }
  }
  check_reduce(bv);
}

//-----------------------------------------------------------------------------
void
PartitionGraph_default::resize (int len)
{
  up.resize(len);
  down.resize(len);
}

int
PartitionGraph_default::sym_name (void)
{
  if (_sym_count == _max_sym) {
    _max_sym += 32;
    resize(_max_sym);
  }
  return _sym_count++;
}

// a partitions into b U c
void
PartitionGraph_default::add_partition(Partition p)
{
  Partition* pp = new Partition (p);

  part_list.add_tail(pp);

  down[pp->whole()].add_tail(pp);
  up[pp->part(0)].add_tail(pp);
  up[pp->part(1)].add_tail(pp);

  if (dbg(pqs)) {
    cdbg << "partition: " << pp->whole() << '='
	 << pp->part(0) << '|' << pp->part(1) << endl;
  }
}

// merge nodes p and q, so that q inherits all of p's properties

void
PartitionGraph_default::remap(int x, int y) {

  //
  // rename all partition references of x to ref of y
  //
  for (List_iterator<Partition*> pi1(part_list); pi1 != 0; pi1++) {
    Partition *pt = *pi1;
    int p = pt->whole();
    int m = pt->part(0);
    int n = pt->part(1);

    if (p == x) {
      p = y;
      pt->set(p, m, n);
    }
    if (m == x) {
      m = y;
      pt->set(p, m, n);
    }
    if (n == x) {
      n = y;
      pt->set(p, m, n);
    }
  }
  List_iterator<Partition> pi;
  for (pi(*sw_list); pi != 0; pi++) {
    Partition *pt = &(*pi);
    int p = pt->whole();
    int m = pt->part(0);
    int n = pt->part(1);

    if (p == x) {
      p = y;
      pt->set(p, m, n);
    }
    if (m == x) {
      m = y;
      pt->set(p, m, n);
    }
    if (n == x) {
      n = y;
      pt->set(p, m, n);
    }
  }
  for (pi(*mg_list); pi != 0; pi++) {
    Partition *pt = &(*pi);
    int p = pt->whole();
    int m = pt->part(0);
    int n = pt->part(1);

    if (p == x) {
      pt->set(0, m, n);
    }
    if (m == x) {
      m = y;
      pt->set(p, m, n);
    }
    if (n == x) {
      n = y;
      pt->set(p, m, n);
    }
  }

  // remap domain references to x in predicate map to refs of y
  _pmap->remap(x, y);
}

void
PartitionGraph_default::preprocess(void)
{
  if (dbg(pqs)) cdbg << "--- preprocessing ---" << endl;

  // make sure cached results are not used while preprocessing 
  ancestor_cache_valid = false ;
  disjoint_cache_valid = false ;

  // process all switch partitions created by PredicateMap
  for (List_iterator<Partition> swi(*sw_list); swi != 0; swi++) {
    Partition p = *swi;
    add_partition(p);
  }

  // process all merge partitions created by PredicateMap
  // - if merge is of disjoint predicates, process partition
  // - otherwise, process merge conservatively
  for (List_iterator<Partition> mgi(*mg_list); mgi != 0; mgi++) {
    Partition pt = *mgi;
    int p = pt.whole();
    int m = pt.part(0);
    int n = pt.part(1);

    if (p == 0) continue; // skip squashed merge partitions

    if (is_disjoint(m, n)) {
      int lca_mn = lca(m, n);
      
      Bitvector rc(_sym_count);
      rc.set_bit(lca_mn);
      lub_diff(rc, m);
      lub_diff(rc, n);
      
      // handle case where p equals lca_mn
      if (rc.ones_count() == 0) {
	// remap references to p to lca_mn.
	remap(p, lca_mn);

	continue;
      }

      add_partition(pt);

      Bit_rev_iterator bi(rc);
      int x = *bi;
      for (bi++; bi != 0; bi++) {
	int y = *(bi);
	Partition pp (sym_name(), x, y);

	// The partition being added may be a merge partition waiting to
	// be processed.  In this case, we should go ahead and add the
	// merge partition, rather than creating a new partition, since
	// this avoids having redundant partitions.  Because the equivalent
	// merge partition is referenced by the predicate map, it must be
	// added in any case.
	for (List_iterator<Partition> mgi2(mgi); mgi2 != 0; mgi2++) {
	  Partition& qq = *mgi2;
	  if (qq.part(0) == pp.part(0) &&
	      qq.part(1) == pp.part(1)) {
	    pp = qq;
	    // squash merge partition so that it isn't processed again
	    qq.set(0, 0, 0);
	    break;
	  }
	}

	add_partition(pp);
	x = pp.whole();
      }
      Partition pp (lca_mn, p, x);
      add_partition (pp);
    } else {
      // p = m | n  where m, n are not disjoint
      // find w = lca(m,n)
      // add partition w = p | y, where y is a synthetic predicate

      int w = lca(m, n);
      int y = sym_name();  // create node for chaining q's together
      Partition pp (w, p, y);
      add_partition(pp);
    }
  }
  
  // adjust all bitvectors and bit-matrices to correct length (no extra bits)
  resize(_sym_count);

  if (dbg(pqs)) cdbg << "--- preprocessing ---" << endl;
}

void
PartitionGraph_default::cache_properties(void)
{
   int pred1 ;
   Bitvector tmp1(_sym_count) ;

   cached_ancestors.resize(_sym_count,tmp1) ;
   for (pred1 = 0 ; pred1<_sym_count ; pred1++) {
      mark_ancestors(pred1, cached_ancestors[pred1]) ;
   }
   ancestor_cache_valid = true ;
   
   
}


void
PartitionGraph_default::test(void) const
{
}

// reverse depth-first search from node a, looking for node b
bool
PartitionGraph_default::search_ancestors (int a, int b, Bitvector& mark) const {
  if (!mark.bit(a)) {
    mark.set_bit(a);
    if (a==b) { return true; }
    for (PG_preds pi(*this, a); pi != 0; pi++) {
      int p = *pi;
      if (search_ancestors(p, b, mark) == true) {
	return true;
      }
    }
  }
  return false;  // node b not found along this path
}

void
PartitionGraph_default::mark_ancestors (int a, Bitvector& mark) const
{
  if (!mark.bit(a)) {
    mark.set_bit(a);
    for (PG_preds pi(*this, a); pi != 0; pi++) {
      int p = *pi;
      mark_ancestors(p, mark);
    }
  }
}

bool
PartitionGraph_default::disj_search (int b, Bitvector& mark) const 
{
  if (!mark.bit(b)) {  // continue search along current path
    // Don't think it should mark the bitvector..  -KF 11/2005
    //mark.set_bit(b);
    for (List_iterator<Partition*> upi(up[b]); upi != 0; upi++) {
      Partition* p = *upi;
      int m = p->part(0);
      int n = p->part(1);
      int x = p->whole();

      // does this partition prove disjointness?
      //if (mark.bit(m) && mark.bit(n)) return true;
      // Compare b with m,n explicitly since we do not add b to the bitvector
      // anymore.  -KF 11/2005
      if ((b == m && mark.bit(n)) ||
          (b == n && mark.bit(m)))
        return true;

      // was disjointness proven further along this path?
      if (disj_search(x, mark)) return true;
    }
  }
  return false;  // not disjoint along this path
}

/* rel_cmpl(p, q, bv) computes relative complement of p wrt q and ORs the
 * result with bv.
 */
void
PartitionGraph_default::rel_cmpl(int p, int q, Bitvector& bv) const
{
   if (ancestor_cache_valid == true) {
      Bitvector mark = cached_ancestors[p];
      assert (mark.bit(q));  // make sure p is a subset of q
      
      // find tree path from q down to p, add path siblings to bv
      while (q != p) {
         // find partition of q containing p
         for (List_iterator<Partition*> pi(down[q]); pi != 0; pi++) {
            Partition* pp = *pi;
            int m = pp->part(0);
            int n = pp->part(1);
            if (mark.bit(m)) {
               tree_sum_reduce(bv, n);
               q = m;
               break;
            } else if (mark.bit(n)) {
               tree_sum_reduce(bv, m);
               q = n;
               break;
            }
         }
      }
   }
   else { 
      Bitvector mark(_sym_count);
      
      mark_ancestors(p, mark);
      assert (mark.bit(q));  // make sure p is a subset of q
      
      // find tree path from q down to p, add path siblings to bv
      while (q != p) {
         // find partition of q containing p
         for (List_iterator<Partition*> pi(down[q]); pi != 0; pi++) {
            Partition* pp = *pi;
            int m = pp->part(0);
            int n = pp->part(1);
            if (mark.bit(m)) {
               tree_sum_reduce(bv, n);
               q = m;
               break;
            } else if (mark.bit(n)) {
               tree_sum_reduce(bv, m);
               q = n;
               break;
            }
         }
      }
   }
}

void
PartitionGraph_default::check_reduce (Bitvector& bv) const
{
   return ;
  // verify that no reduction is needed
  for (List_iterator<Partition*> pi(part_list); pi != 0; pi++) {
    Partition* pp = *pi;
    int m = pp->part(0);
    int n = pp->part(1);
    if (bv.bit(m) && bv.bit(n)) {
      El_punt("reduce error.");
    }
  }
}

void
PartitionGraph_default::approx_diff (int p, int q, Bitvector& bv) const
{
  // there are many choices here; for example, we could return p
  rel_cmpl(q, lca(p, q), bv);
}

bool
PartitionGraph_default::is_subset (int p, Bitvector& bv) const
{
   if (ancestor_cache_valid == true) {
      Bitvector mark = cached_ancestors[p] ;  //intential copy
      
      mark *= bv ;
      if (mark.is_allclear()) return false ;
      else return true ;      
   }
   else {
      Bitvector mark(_sym_count);
      mark_ancestors(p, mark);
      
      mark *= bv ;
      if (mark.is_allclear()) return false ;
      else return true ;
   }
}

void
PartitionGraph_default::clear_subtree2 (Bitvector& bv, Bitvector& visited,
				    int a) const
{
  bv.reset_bit(a);

  if (!visited.bit(a)) {
    visited.set_bit(a);
    for (PG_succs pi(*this, a); pi != 0; pi++) {
      int p = *pi;
      clear_subtree2(bv, visited, p);
    }
  }
}

void
PartitionGraph_default::clear_subtree (Bitvector& bv, int a) const
{
  Bitvector visited (_sym_count);
  clear_subtree2 (bv, visited, a);
}

int
PartitionGraph_default::sum_reduce (Bitvector& bv, int p) const
{
  // r is highest node reached during reduction; all subsets will be cleared
  int r = p;

  bv.set_bit(p);

  // does this complete a partition?
  for (List_iterator<Partition*> upi(up[p]); upi != 0; upi++) {
    Partition* pp = *upi;
    int m = pp->part(0);
    int n = pp->part(1);
    if (bv.bit(m) && bv.bit(n)) {
      r = sum_reduce(bv, pp->whole());
    }
  }
  return r;
}

void
PartitionGraph_default::tree_sum_reduce(Bitvector& bv, int p) const
{
  bv.set_bit(p);

  // does this complete a partition?
  for (List_iterator<Partition*> upi(up[p]); upi != 0; upi++) {
    Partition* pp = *upi;
    int m = pp->part(0);
    int n = pp->part(1);
    if (bv.bit(m) && bv.bit(n)) {
      bv.reset_bit(m);
      bv.reset_bit(n);
      tree_sum_reduce(bv, pp->whole());
    }
  }
}

//-----------------------------------------------------------------------------
void
PartitionGraph_default::print(ostream& os) const
{
  os << "- PartitionGraph-exp = <" << endl;

  for (List_iterator<Partition*> pi1(part_list); pi1 != 0; pi1++) {
    Partition *pt = *pi1;
    int p = pt->whole();
    int m = pt->part(0);
    int n = pt->part(1);

    os << p << " = " << m << " | " << n << endl;
  }

  os << " end of partition graph >" << endl;
}

// end of partition_graph_default.cpp
