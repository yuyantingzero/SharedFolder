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
//      File:           IR_map.cpp
//      Authors:        Greg Snider, Scott A. Mahlke
//      Created:        August 1995
//      Description:    implements a 1 to 1 mapping (write-once)
//
/////////////////////////////////////////////////////////////////////////////

#include "IR_map.h"
#include <stdlib.h>
#include "ir_assert.h"


template <class Key, class Value>
IR_map<Key, Value>::IR_map() : total_buckets(1000)
{
    // Constructor.

    buckets = new Entry<Key, Value> *[total_buckets];
    for (int bucket = 0;  bucket < total_buckets;  bucket++)
	buckets[bucket] = NULL;
    total_entries = 0;
}


template <class Key, class Value>
IR_map<Key, Value>::IR_map(int hash_buckets) : total_buckets(hash_buckets)
{
    // Constructor.

    buckets = new Entry<Key, Value> *[total_buckets];
    for (int bucket = 0;  bucket < total_buckets;  bucket++)
	buckets[bucket] = NULL;
    total_entries = 0;
}


template <class Key, class Value>
IR_map<Key, Value>::~IR_map()
{
    // Destructor

    clear();
    delete [] buckets ;
}


template <class Key, class Value>
void
IR_map<Key, Value>::clear()
{
    // Clear out the map;

    Entry<Key, Value> *entry;
    for (int bucket = 0;  bucket < total_buckets;  bucket++) {
	while ((entry = buckets[bucket])) {
	    entry = buckets[bucket];
	    buckets[bucket] = entry->next;
	    delete entry;
	}
	buckets[bucket] = NULL;
    }
    total_entries = 0;
}


template <class Key, class Value>
int
IR_map<Key, Value>::lookup(Key key, Value &value)
{
    // Given a key, lookup value.  Returns true if found, else false;

    int bucket = hash(key);
    Entry<Key, Value> *entry;
    for (entry = buckets[bucket];  entry;  entry = entry->next) {
	if (entry->key == key) {
	    value = entry->value;
	    return 1;
	}
    }
    return 0;
}


template <class Key, class Value>
void 
IR_map<Key, Value>::install(Key key, Value value)
{
    // Insert a (key, value) pair into the map;

    Value dummy;
    ir_assert(lookup(key, dummy) == 0);
    Entry<Key, Value> *entry = new Entry<Key, Value>;
    entry->key = key;
    entry->value = value;
    int bucket = hash(key);
    entry->next = buckets[bucket];
    buckets[bucket] = entry;
    total_entries++;
}


template <class Key, class Value>
int 
IR_map<Key, Value>::hash(Key key)
{
    // Simpleton hash function, should be OK for fundamental types;

    int k = int(key);
    unsigned int bucket =
	k ^ (k >> 3) ^ (k >> 6) ^ (k >> 9) ^ (k >> 12) ^ (k >> 15);
    bucket %= total_buckets;
    return bucket;
}


//------------------------ test code ----------------------------------

#ifdef TEST_MAP

#include <iostream>

const ENTRIES = 5;
void *keys[ENTRIES] = {
    (void *) 1234, (void *) 2345, (void *) 3456, (void *) 4567, (void *) 5678
};
int values[ENTRIES] = { 5, 4, 3, 2, 1 };

main()
{
    // Black box testing of the map class;

    IR_map<void *, int> map;
    int val;
    for (int e = 0;  e < ENTRIES;  e++)
	ir_assert(map.lookup(keys[e], val) == 0);
    ir_assert(map.entries() == 0);
    map.install(keys[0], values[0]);
    ir_assert(map.entries() == 1);
    int result = map.lookup(keys[0], val);
    ir_assert(result == 1);
    ir_assert(val = values[0]);
    for (e = 1;  e < ENTRIES;  e++)
	ir_assert(map.lookup(keys[e], val) == 0);
    for (e = 1;  e < ENTRIES;  e++)
	map.install(keys[e], values[e]);
    ir_assert(map.entries() == ENTRIES);
    for (e = 0;  e < ENTRIES;  e++) {
	result = map.lookup(keys[e], val);
	ir_assert(result == 1);
	ir_assert(val = values[e]);
    }
    ir_assert(map.entries() == ENTRIES);
    cout << "IR_map test passed" << endl;
}

#endif 
