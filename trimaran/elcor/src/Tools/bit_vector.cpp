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
//      File:           bit_vector.cpp
//      Authors:        Sadun Anik, Richard Johnson, Santosh Abraham
//      Created:        December 1994
//      Description:    Bit Vector class declaration
//
/////////////////////////////////////////////////////////////////////////////

#include "bit_vector.h"

Bitvector::Bitvector()
{
  num_bits = 0;
  num_chunks = 0;
  chunks = 0;
}

Bitvector::Bitvector(int vector_size, bool bit_val)
{
  num_bits = ELCOR_MAX(vector_size, 0);
  if (num_bits > 0) {
    num_chunks = ((num_bits - 1 ) >> CHUNK_SIZE_LOG ) + 1 ;
    chunks = new unsigned long[num_chunks] ;
    for (int i = 0 ; i<num_chunks ; i++) {
      chunks[i] = (bit_val ? ~0 : 0) ;
    }
  } else {
    num_chunks = 0;
    chunks = 0;
  }
}

Bitvector::Bitvector(const Bitvector& bv)
{
  num_bits = bv.num_bits ;
  num_chunks = bv.num_chunks ;
  chunks = new unsigned long[num_chunks] ;
  for (int i = 0 ; i<num_chunks ; i++) {
    chunks[i] = bv.chunks[i] ;
  }
}

Bitvector::~Bitvector()
{
  delete [] chunks ;
}

unsigned int
Bitvector::size() const
{
  return num_bits;
}

Bitvector&
Bitvector::clear()
{
  for (int i = 0 ; i < num_chunks ; i++) {
    chunks[i] = 0 ;
  }
  return *this ;
}

Bitvector&
Bitvector::set()
{
  for (int i = 0 ; i < num_chunks ; i++) {
    chunks[i] = (unsigned long)~0 ;
  }
  return *this ;
}

Bitvector&
Bitvector::set_from_mask(unsigned long mask)
{
  for (int i = 0 ; i < num_chunks ; i++) {
    chunks[i] = mask ;
  }
  return *this ;
}

Bitvector&
Bitvector::complement()
{
  for (int i = 0 ; i < num_chunks ; i++) {
    chunks[i] = ~chunks[i] ;
  }
  return *this ;
}

// resize Vector, copy old elements where possible
Bitvector&
Bitvector::resize(int nbits, bool bit_val)
{
   // set or reset garbage bits
   if ((num_bits & CHUNK_SIZE_MASK) != 0) {
      unsigned long mask = ~((~0) << (num_bits & CHUNK_SIZE_MASK));
      if (bit_val) {
         chunks[num_chunks-1] |= ~mask;
      } else {
         chunks[num_chunks-1] &= mask;
      }
   }
   // allocate new chunks
   int n_bits = ELCOR_MAX(nbits, 0);
   int n_chunks = 0;
   unsigned long* new_chunks = 0;
   if (n_bits > 0) {
      n_chunks = ((n_bits - 1 ) >> CHUNK_SIZE_LOG ) + 1 ;
      new_chunks = new unsigned long[n_chunks];
      for (int i=0 ; i<n_chunks ; i++) {
         new_chunks[i] = (bit_val ? ~0 : 0) ;
      }
   }
   
   // copy existing elements into new Vector where possible
   for (int i=0; i<ELCOR_MIN(num_chunks, n_chunks); i++) {
      new_chunks[i] = chunks[i];
   }
   delete [] chunks;
   num_bits = n_bits;
   num_chunks = n_chunks;
   chunks = new_chunks;
   return *this;
}

Bitvector&
Bitvector::operator=(const Bitvector& bv)
{
  // sizes must agree unless current size is zero
  if ((num_bits == 0) || (num_bits != bv.num_bits)) {
     if ((num_bits != 0) && (num_bits != bv.num_bits)) {
	delete [] chunks ;
	chunks = NULL ;
     }
    num_bits = bv.num_bits;
    num_chunks = bv.num_chunks;
    if (num_bits > 0) {
      chunks = new unsigned long[num_chunks] ;
      for (int i = 0 ; i<num_chunks ; i++) {
	chunks[i] = bv.chunks[i] ;
      }
    }
  } else {
    assert(num_bits == bv.num_bits) ;
    if (chunks != bv.chunks) {
      for (int i = 0 ; i<num_chunks ; i++) {
	chunks[i] = bv.chunks[i] ;
      }
    }
  }
  return *this ;
}

Bitvector&
Bitvector::operator|=(const Bitvector& bv)
{
  int prev_num_bits = num_bits;
  // first create space
  resize(num_bits+bv.num_bits, false);
  // now copy the second bitvector
  for (Bit_iterator iter(bv); iter!=0; iter++)
    set_bit(prev_num_bits+(*iter));

  return *this;
}


bool
Bitvector::operator==(const Bitvector& bv) const
{
  if (num_bits != bv.num_bits) {
    return false;
  }
  // compare all but last chunks
  for (int i = 0 ; i<num_chunks-1 ; i++) {
    if (chunks[i] != bv.chunks[i]) return false ;
  }
  // only compare valid bits of last chunk
  unsigned long mask = (unsigned long)~0; // all ones, in case num_bits == CHUNK_SIZE
  if ((num_bits & CHUNK_SIZE_MASK) != 0) {
    mask = ~((~0) << (num_bits & CHUNK_SIZE_MASK));
  }
  if (num_chunks > 0) {
     if ((chunks[num_chunks-1] & mask) != (bv.chunks[num_chunks-1] & mask)) {
        return false;
     }
  }
  return true ;
}

bool
Bitvector::operator!=(const Bitvector& bv) const
{
  return (!(*this == bv));
}
   
Bitvector&
Bitvector::operator+=(const Bitvector& bv)
{
  // sizes must agree unless current size is zero
  if (num_bits == 0) {
    *this = bv;
  }

  assert(num_bits == bv.num_bits) ;
  for (int i = 0 ; i<num_chunks ; i++) {
    chunks[i] |= bv.chunks[i] ;
  }
  return *this ;
}

Bitvector&
Bitvector::operator-=(const Bitvector& bv)
{
  // sizes must agree unless current size is zero
  if (num_bits == 0) {
    *this = bv;
  }

   assert(num_bits == bv.num_bits) ;
   for (int i = 0 ; i<num_chunks ; i++) {
      chunks[i] &= ~(bv.chunks[i]) ;
   }
   return *this ;
}

Bitvector
Bitvector::operator-(const Bitvector& bv) const
{
  assert(num_bits == bv.num_bits) ;
  Bitvector new_bv(*this);
  new_bv -= bv;
  return new_bv;
}

Bitvector&
Bitvector::operator*=(const Bitvector& bv)
{
  // sizes must agree unless current size is zero
  if (num_bits == 0) {
    *this = bv;
  }

   assert(num_bits == bv.num_bits) ;
   for (int i = 0 ; i<num_chunks ; i++) {
      chunks[i] &= bv.chunks[i] ;
   }
   return *this ;
}

Bitvector
Bitvector::operator*(const Bitvector& bv) const
{
  assert(num_bits == bv.num_bits) ;
  Bitvector new_bv(*this);
  new_bv *= bv;
  return new_bv;
}

bool 
Bitvector::intersects(const Bitvector& bv) const 
{
  assert(num_bits == bv.num_bits);
  for(int i = 0; i < num_chunks; i++) {
    if(chunks[i] & bv.chunks[i])
      return true;
  }
  return false;
}

Bitvector&
Bitvector::set_bit(unsigned int pos)
{
   assert(pos < (unsigned)num_bits) ;
   int offset = pos & CHUNK_SIZE_MASK ;
   int chunk_index = pos >> CHUNK_SIZE_LOG ;
   unsigned long mask = 1 ;
   mask = mask << offset ;
   chunks[chunk_index] |= mask ;
   return (*this) ;
}

Bitvector&
Bitvector::reset_bit(unsigned int pos)
{
   assert(pos < (unsigned)num_bits) ;
   unsigned int offset = pos & CHUNK_SIZE_MASK ;
   unsigned int chunk_index = pos >> CHUNK_SIZE_LOG ;
   unsigned long mask = 1 ;
   mask = mask << offset ;
   mask = ~mask ;
   chunks[chunk_index] &= mask ;
   return (*this) ;
}

bool
Bitvector::bit(unsigned int pos) const
{
   assert(pos < (unsigned)num_bits) ;
   unsigned int offset = pos & CHUNK_SIZE_MASK ;
   unsigned int chunk_index = pos >> CHUNK_SIZE_LOG ;
   unsigned long chunk = chunks[chunk_index] ;
   unsigned long mask = 1 ;
   mask = mask << offset ;
   chunk &= mask ;
   if (chunk > 0) return true ;
   else return false ;
}

int
Bitvector::ones_count() const
{
  // mask off garbage bits
  if ((num_bits & CHUNK_SIZE_MASK) != 0) {
    unsigned long mask = ~((~0) << (num_bits & CHUNK_SIZE_MASK));
    chunks[num_chunks-1] &= mask;
  }

  int count = 0;
  for (int i=0 ; i<num_chunks ; i++) {
    unsigned long chunk = chunks[i];

    // ntc 7/25/05: This is copied from a book on "magic
    // algorithms". I don't understand why it works, but it's a lot
    // faster.
    if(CHUNK_SIZE == 32) {
      chunk -= ((chunk >> 1) & 0x55555555);
      chunk = (((chunk >> 2) & 0x33333333) + (chunk & 0x33333333));
      chunk = (((chunk >> 4) + chunk) & 0x0f0f0f0f);
      chunk += (chunk >> 8);
      chunk += (chunk >> 16);
      count += (chunk & 0x0000003f);
    } else {
      for (int j=0; j<CHUNK_SIZE; j++) {
	if (chunk == 0) break;
	if (chunk & 1) {
	  count++;
	}
	chunk = chunk >> 1;
      }
    }
  }
  return count;
}

int
Bitvector::zeros_count() const
{
  return size() - ones_count();
}

int
Bitvector::first_one() const {
  Bit_iterator bi(*this);
  if (bi!=0)
    return (*bi);
  else
    return -1;
}

int
Bitvector::last_one() const {
  Bit_rev_iterator bi(*this);
  if (bi!=0)
    return (*bi);
  else
    return -1;
}

int
Bitvector::next_bit(bool bit, unsigned int start) const
{
  assert(start < (unsigned) num_bits);
  // adjust to initial position
  unsigned int offset = start & CHUNK_SIZE_MASK ;
  unsigned int chunk_index = start >> CHUNK_SIZE_LOG ;
  unsigned long mask = 1 ;
  mask = mask << offset ;

  do {
    unsigned long chunk = chunks[chunk_index] ;
    do {
      unsigned long result = chunk & mask;
      if ((start == (unsigned)num_bits) || (bit && result) || (!bit && !result)) return start;
      mask = mask << 1; start++; offset++;
    } while (offset<CHUNK_SIZE);
    offset = 0; mask = 1; chunk_index++;
  } while (chunk_index<(unsigned)num_chunks);
  // we should never reach here
  assert(0);
  return num_bits;
}

bool
Bitvector::is_allclear() const
{
  bool allclear = true;
  // when there are no chunks, return true;
  if (num_chunks <= 0) return allclear;

  // check if first (num_chunks-1) chunks are all zero
  for (unsigned int i = 0; i < (unsigned)(num_chunks-1); i++) {
    if (chunks[i] != 0) {
      allclear = false;
      break;
    }
  }

  // check the last chunk
  if ((num_bits & CHUNK_SIZE_MASK) != 0) {
    // check only the relevant low-order bits
    unsigned long mask = ~((~0) << (num_bits & CHUNK_SIZE_MASK));
    if ((chunks[num_chunks-1] & mask) != 0) allclear = false;
  } else {
    // last chunk is fully used, check all bits
    if (chunks[num_chunks-1] != 0) allclear = false;
  }
  return (allclear);
}


bool
Bitvector::is_allset() const
{
  bool allset = true;
  // when there are no chunks, return true;
  if (num_chunks <= 0) return allset;

  // check if all but last chunk are all ones
  for (int i = 0; i < num_chunks-1; i++) {
    if (chunks[i] != (unsigned) (~0)) {
      allset = false;
      break;
    }
  }

  // check the last chunk
  if ((num_bits & CHUNK_SIZE_MASK) != 0) {
    // check only the relevant low-order bits
    unsigned long mask = (~0) << (num_bits & CHUNK_SIZE_MASK);
    if ((chunks[num_chunks-1] | mask) != (unsigned) (~0)) allset = false;
  } else {
    // last chunk is fully used, check all bits
    if (chunks[num_chunks-1] != (unsigned) (~0)) allset = false;
  }
  return (allset);
}

ostream&
operator << (ostream& os, const Bitvector& bv) {
  if(bv.num_bits==0 && bv.chunks==NULL) {
    os << "NULL VECTOR" << endl;
  }
  for(int i=0; i< bv.num_bits; i++) {
    os << (bv.bit(i) ? "1" : ".");
  }
  os << endl;
  return os;
}

Bitvector
operator | (const Bitvector& lbv, const Bitvector& rbv)
{
  Bitvector result(lbv);
  result |= rbv;
  return result;
}

unsigned hash_bitvector(Bitvector& bv) {
  int foo = 0;

  for (int i = 0 ; i<bv.num_chunks ; i++) {
    foo |= (int)bv.chunks[i] ;
  }

  return hash_int(foo);
}

/*---------------------------------------------------------------------------
   Bit iterator class implementation
-----------------------------------------------------------------------------*/

Bit_iterator::Bit_iterator()
{
  bv = 0;
  pos = -1;
}

Bit_iterator::Bit_iterator(const Bitvector& bvec)
{
  bv = &bvec;
  if (bvec.num_chunks == 0) bv = 0 ;
  pos = -1;
  advance();
}

Bit_iterator::Bit_iterator(const Bit_iterator& bi)
{
  bv = bi.bv;
  pos = bi.pos;
}

Bit_iterator& Bit_iterator::operator=(const Bit_iterator& bi)
{
  bv = bi.bv;
  pos = bi.pos;
  return *this; //return to make CC happy
} 

bool Bit_iterator::operator==(const Bit_iterator& bi) const
{
  if ((bv == bi.bv) && (pos == bi.pos)) return true ;
  else return false ;
}


void Bit_iterator::operator()(const Bitvector& bvec)
{
  bv = &bvec;
  if (bvec.num_chunks == 0) bv = 0 ;
  pos = -1;
  advance();
}

Bit_iterator::~Bit_iterator()
{
}

bool Bit_iterator::operator==(const void* x) const
{
  return (x == (void*)(bv)) ;
}

bool Bit_iterator::operator!=(const void* x) const
{
  return (x != (void*)(bv)) ;
}


void Bit_iterator::advance()
{
  if (bv != 0) {
    do {
      pos++;
      if (!(pos & CHUNK_SIZE_MASK)) {
	int chi = pos >> CHUNK_SIZE_LOG;
	while ((chi < (bv->num_chunks - 1)) && (bv->chunks[chi] == 0)) {
	  chi++;
	  pos += CHUNK_SIZE;
	}
      }
      if (pos == (int)bv->size()) {
	bv = 0;
	break;
      }
    } while (!bv->bit(pos));
  }
}

void Bit_iterator::operator++() 
{
  advance();
}

void Bit_iterator::operator++(int) 
{
  advance();
}

int Bit_iterator::operator*() 
{
   if (bv == 0) {
      assert(0) ;
   }
   return (pos) ;
}

/*---------------------------------------------------------------------------
   Bit reverse iterator class implementation
-----------------------------------------------------------------------------*/

Bit_rev_iterator::Bit_rev_iterator()
{
  bv = 0;
  pos = -1;
}

Bit_rev_iterator::Bit_rev_iterator(const Bitvector& bvec)
{
  bv = &bvec;
  pos = bv->size();
  advance();
}

Bit_rev_iterator::Bit_rev_iterator(const Bit_rev_iterator& bi)
{
  bv = bi.bv;
  pos = bi.pos;
}

Bit_rev_iterator& Bit_rev_iterator::operator=(const Bit_rev_iterator& bi)
{
  bv = bi.bv;
  pos = bi.pos;
  return *this; //return to make CC happy
} 

void Bit_rev_iterator::operator()(const Bitvector& bvec)
{
  bv = &bvec;
  pos = bv->size();
  advance();
}

Bit_rev_iterator::~Bit_rev_iterator()
{
}

bool Bit_rev_iterator::operator==(const void* x) const
{
  return (x == (void*)(bv)) ;
}

bool Bit_rev_iterator::operator!=(const void* x) const
{
  return (x != (void*)(bv)) ;
}


void Bit_rev_iterator::advance()
{
  if (bv != 0) {
    do {
      pos--;
      if (pos == -1) {
	bv = 0;
	break;
      }
    } while (!bv->bit(pos));
  }
}

void Bit_rev_iterator::operator++() 
{
  advance();
}

void Bit_rev_iterator::operator++(int) 
{
  advance();
}

int Bit_rev_iterator::operator*() 
{
   if (bv == 0) {
      assert(0) ;
   }
   return (pos) ;
}

// end of bit_vector.cpp
