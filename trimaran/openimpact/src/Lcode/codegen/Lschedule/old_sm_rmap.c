/*****************************************************************************\
 *
 *		      Illinois Open Source License
 *                     University of Illinois/NCSA
 *                         Open Source License
 *
 * Copyright (c) 2004, The University of Illinois at Urbana-Champaign.
 * All rights reserved.
 *
 * Developed by:             
 *
 *		IMPACT Research Group
 *
 *		University of Illinois at Urbana-Champaign
 *
 *              http://www.crhc.uiuc.edu/IMPACT
 *              http://www.gelato.org
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal with the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimers.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimers in
 * the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the names of the IMPACT Research Group, the University of
 * Illinois, nor the names of its contributors may be used to endorse
 * or promote products derived from this Software without specific
 * prior written permission.  THE SOFTWARE IS PROVIDED "AS IS",
 * WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS WITH THE SOFTWARE.
 *
\*****************************************************************************/
/*****************************************************************************\
 *      File:   old_sm_rmap.c (an early and very preliminary version of the
 *              new SM framework provided to allow current scheduler to 
 *              use .lmdes2) 
 *      Author: John C. Gyllenhaal
 *      Creation Date:  March 1996
 *      Copyright (c) 1996 John C. Gyllenhaal, Wen-mei Hwu and
 *                         The Board of Trustees of the University of Illinois.
 *                         All rights reserved.
 *      The University of Illinois software License Agreement
 *      specifies the terms and conditions for redistribution.
\*****************************************************************************/
/* 10/29/02 REK Adding config.h */
#include <config.h>
#include <stdio.h>
#include "old_sm.h"
#include <machine/mdes2.h>
#include <library/l_alloc_new.h>

#define DEBUG_RMAP_RESIZE 0
#define DEBUG_RMAP 0
#define TAKE_STATS 0

static L_Alloc_Pool *OLD_SM_Cb_pool = NULL;

/* Prototypes */
void OLD_SM_commit_choices (unsigned int *map, SM_Table *table,
			unsigned short *choices_made);

OLD_SM_Cb *OLD_SM_new_cb (Mdes2 *mdes2, OLD_SM_Func *sm_func, L_Cb *cb)
{
    OLD_SM_Cb *sm_cb;

    
    /* Alloc new sm_cb structure (initialize if necessary) */
    if (OLD_SM_Cb_pool == NULL)
	OLD_SM_Cb_pool = L_create_alloc_pool ("OLD_SM_Cb", sizeof (OLD_SM_Cb), 16);
    sm_cb = (OLD_SM_Cb *) L_alloc (OLD_SM_Cb_pool);

    /* Initialize fields */
    sm_cb->sm_mdes = mdes2->sm_mdes;
    sm_cb->cb = cb;
    sm_cb->sm_func = sm_func;


    /* Initialize resource map fields */
    sm_cb->map_array = NULL;	/* Algorithm assumes set to NULL initially */

    /* The rest of the fields don't need to be intialized here */
    
    /* Return new created sm_cb structure */
    return (sm_cb);
}

void OLD_SM_delete_cb (OLD_SM_Cb *sm_cb)
{
    /* Free the map array if necessary */
    if (sm_cb->map_array != NULL)
	free (sm_cb->map_array);

    /* Free the sm_cb structure */
    L_free (OLD_SM_Cb_pool, sm_cb);
}

void OLD_SM_check_map (OLD_SM_Cb *sm_cb)
{
    int min_init_offset, max_init_offset, map_start_offset;
    unsigned int *map_array;
    int offset, errors;

    min_init_offset = sm_cb->min_init_offset;
    max_init_offset = sm_cb->max_init_offset;
    map_array = sm_cb->map_array;
    map_start_offset = sm_cb->map_start_offset;

    errors = 0;
    for (offset = min_init_offset; offset <= max_init_offset; offset++)
    {
	if (map_array[offset - map_start_offset] != ((unsigned int) offset))
	{
	    printf ("Map mismatch, map_array[%i] = %i not %i!\n",
		    offset, map_array[offset - map_start_offset], offset);
	    errors++;
	}
    }

    if (errors != 0)
	L_punt ("%i map mismatches!", errors);
}
void OLD_SM_expand_map_end_offset (OLD_SM_Cb *sm_cb, int max_init_offset)
{
    int new_map_array_size, malloc_size;
    int map_start_offset, start_index, end_index;
    unsigned int *old_map_array, *new_map_array;
    int index1;

    /* Get the map_start_offset of speed and ease of use */
    map_start_offset = sm_cb->map_start_offset;
    
    /* Double the map array size until covers max_init_offset.*/
    new_map_array_size = sm_cb->map_array_size << 1;
    while ((map_start_offset + new_map_array_size) <= max_init_offset)
	new_map_array_size = new_map_array_size << 1;

#if DEBUG_RMAP
    printf ("    Expanding map end offset from %i to %i (total size %i)\n",
	    sm_cb->map_end_offset, 
	    sm_cb->map_start_offset + new_map_array_size - 1,
	    new_map_array_size);
#endif

    /* Malloc the new array */
    malloc_size = new_map_array_size * sizeof(unsigned int);
    if ((new_map_array = (unsigned int *) malloc(malloc_size)) == NULL)
	L_punt ("OLD_SM_expand_map_end_offset: Out of memory (size %i)",
		malloc_size);


    /* Copy the contents of the old map array to the new map.
     * Calculate the index bounds using the map start offset
     */
    old_map_array = sm_cb->map_array;
    start_index = sm_cb->min_init_offset - map_start_offset;
    end_index = sm_cb->max_init_offset - map_start_offset;

#if DEBUG_RMAP
    printf ("      Transfering contents from %i to %i (start offset %i)\n",
	    start_index, end_index, map_start_offset);
#endif

    /* Do 4 copies per iteration to speed up the transfer.  Set up 
     * algorithm so that there is always a multiple of 4 elements 
     * that have been initialized!
     */
    for (index1 = start_index; index1 <= end_index; index1 += 4)
    {
	new_map_array[index1] = old_map_array[index1];
	new_map_array[index1+1] = old_map_array[index1+1];
	new_map_array[index1+2] = old_map_array[index1+2];
	new_map_array[index1+3] = old_map_array[index1+3];
    }
    
    /* Sanity check, make sure my multiple of 4 assumption has not
     * been broken!  (Comment out later for speed).
     */
    if ((index1 - 1) != end_index)
    {
	L_punt ("OLD_SM_expand_map_end_offset:\n"
		"expect (end_index(%i) - start_index(%i)) = %i\n"
		"to be (multiple of 4)-1!",
		end_index, start_index, end_index-start_index);
    }
    
    
    /* Free the old map array */
    free (old_map_array);

    /* Point sm_cb at new array with old contents (if any) copied over */
    sm_cb->map_array = new_map_array;
    sm_cb->map_array_size = new_map_array_size;

    /* Update the map end offset (the start offset didn't change) */
    sm_cb->map_end_offset = map_start_offset + new_map_array_size -1;
    
#if DEBUG_RMAP_RESIZE
    OLD_SM_check_map (sm_cb);
#endif
}

void OLD_SM_expand_map_start_offset (OLD_SM_Cb *sm_cb, int min_init_offset)
{
    int new_map_array_size, malloc_size, map_end_offset;
    int old_map_start_offset, new_map_start_offset, start_index, end_index;
    unsigned int *old_map_array, *new_map_array, *adjusted_new_map_array;
    int index1;

    /* Get the map_end_offset of speed and ease of use */
    map_end_offset = sm_cb->map_end_offset;
    
    /* Double the map array size until covers min_init_offset.*/
    new_map_array_size = sm_cb->map_array_size << 1;
    while (((map_end_offset + 1) - new_map_array_size) > min_init_offset)
	new_map_array_size = new_map_array_size << 1;

    /* Calculate the new map_start_offset */
    new_map_start_offset = (map_end_offset + 1) - new_map_array_size;

#if DEBUG_RMAP
    printf ("    Expanding map start offset from %i to %i (total size %i)\n",
	    sm_cb->map_start_offset, 
	    sm_cb->map_end_offset + 1 - new_map_array_size,
	    new_map_array_size);
#endif

    /* Malloc the new array */
    malloc_size = new_map_array_size * sizeof(unsigned int);
    if ((new_map_array = (unsigned int *) malloc(malloc_size)) == NULL)
	L_punt ("OLD_SM_expand_map_end_offset: Out of memory (size %i)",
		malloc_size);


    /* Copy the contents of the old map array to the new map.
     * Calculate the index bounds using the map start offset
     */
    old_map_start_offset = sm_cb->map_start_offset;
    old_map_array = sm_cb->map_array;
    start_index = sm_cb->min_init_offset - old_map_start_offset;
    end_index = sm_cb->max_init_offset - old_map_start_offset;

    /* Adjust the new_map_array pointer so can use same index as
     * old map array for copy.
     */
    adjusted_new_map_array = &new_map_array[old_map_start_offset - 
					    new_map_start_offset];

#if DEBUG_RMAP
    printf ("      Transfering contents from %i to %i (old start offset %i new %i diff %i)\n",
	    start_index, end_index, old_map_start_offset, new_map_start_offset,
	    old_map_start_offset - new_map_start_offset);
#endif

    /* Do 4 copies per iteration to speed up the transfer.  Set up 
     * algorithm so that there is always a multiple of 4 elements 
     * that have been initialized!
     */
    for (index1 = start_index; index1 <= end_index; index1 += 4)
    {
	adjusted_new_map_array[index1] = old_map_array[index1];
	adjusted_new_map_array[index1+1] = old_map_array[index1+1];
	adjusted_new_map_array[index1+2] = old_map_array[index1+2];
	adjusted_new_map_array[index1+3] = old_map_array[index1+3];
    }
    
    /* Sanity check, make sure my multiple of 4 assumption has not
     * been broken!  (Comment out later for speed).
     */
    if ((index1 - 1) != end_index)
    {
	L_punt ("OLD_SM_expand_map_start_offset:\n"
		"expect (end_index(%i) - start_index(%i)) = %i\n"
		"to be (multiple of 4)-1!",
		end_index, start_index, end_index-start_index);
    }
    
    
    /* Free the old map array */
    free (old_map_array);

    /* Point sm_cb at new array with old contents (if any) copied over */
    sm_cb->map_array = new_map_array;
    sm_cb->map_array_size = new_map_array_size;

    /* Update the map start offset (the end offset didn't change) */
    sm_cb->map_start_offset = new_map_start_offset;

#if DEBUG_RMAP_RESIZE
    OLD_SM_check_map (sm_cb);
#endif
}

void OLD_SM_init_for_max_usage (OLD_SM_Cb *sm_cb, int max_usage_offset)
{
    unsigned int *map_array;
    int old_max_init_offset, new_max_init_offset, map_end_offset;
    int map_start_offset;
    int start_index, end_index, index1;

    /* Calculate how far to intialize the map to, want to extend past present
     * need.  Want to initialize at least 4 elements beyond max_usage_offset. 
     * 
     * In order to do some hand unrolling optimizations of the resize
     * and intialization routines, always make the number of elements
     * initialized to a multiple of 4.  Thus make new_max_init_offset a 
     * (multiple of 4) - 1 (since numbering starts at a multiple of 4).
     * 
     * To accomplish this, zero the lower bits of max_usage offset 
     * (in effect, subtract up to 3).  Now have a multiple of 4.
     * Add 7, so the new_max_init_offset is somewhere between 4 and 7 entries
     * larger than max_usage_offset, and is a (multiple of 4) - 1.
     */
    new_max_init_offset = (max_usage_offset - (max_usage_offset & 3)) + 7;

    /* Do we need to resize the map_array? 
     * If max_usage_offset would fall withing the current array, don't
     * resize, just extend initialization as far as possible.
     */
    map_end_offset = sm_cb->map_end_offset;
    if (map_end_offset < new_max_init_offset)
    {
	/* Try to scale back new_max_init_offset if possible to avoid resize */
	if (max_usage_offset <= map_end_offset)
	{
	    /* Since map_end_offset always a (multiple of 4)-1, unrolling
	     * optimizations will still hold after setting to map_end_offset.
	     */
	    new_max_init_offset = map_end_offset;
	}

	/* Otherwise expand the map_end_offset to handle new_max_init_offset */
	else
	{
	    OLD_SM_expand_map_end_offset (sm_cb, new_max_init_offset);
	}
    }

    /* Get sm_cb fields for speed and ease of use */
    map_start_offset = sm_cb->map_start_offset;
    old_max_init_offset = sm_cb->max_init_offset;

    /*
     * First calculate offset range into map array, using map start offset
     * to adjust offset (map_array[0] represents map_start_offset).
     */
    start_index = (old_max_init_offset + 1) - map_start_offset;
    end_index = new_max_init_offset - map_start_offset;

#if DEBUG_RMAP
    printf ("  Expanding max init offset from %i to %i, index %i to %i\n",
	    sm_cb->max_init_offset, new_max_init_offset, start_index, 
	    end_index);
#endif


    /* Initialize the necessary portions of the map.
     * Do 4 intializations per iteration to speed up the initialization.
     * I set up algorithm so that there is always a multiple of 4 elements 
     * need to be initialized!
     */
    map_array = sm_cb->map_array;
    for (index1 = start_index; index1 <= end_index;
	 index1 += 4)
    {
	map_array[index1] = 0;
	map_array[index1+1] = 0;
	map_array[index1+2] = 0;
	map_array[index1+3] = 0;

#if DEBUG_RMAP_RESIZE
	map_array[index1] = map_start_offset + index1 ;
	printf ("Setting map_array[%i] to %i\n", index1, map_array[index1]);
	map_array[index1+1] = map_start_offset + index1 +1;
	map_array[index1+2] = map_start_offset + index1 +2;
	map_array[index1+3] = map_start_offset + index1 +3;
#endif
    }
    
    /* Set the new max_init_offset */
    sm_cb->max_init_offset = new_max_init_offset;

#if DEBUG_RMAP_RESIZE
    OLD_SM_check_map (sm_cb);
#endif
}

void OLD_SM_init_for_min_usage (OLD_SM_Cb *sm_cb, int min_usage_offset)
{
    unsigned int *map_array;
    int old_min_init_offset, new_min_init_offset, map_start_offset;
    int start_index, end_index, index1;

    /* Calculate how far to intialize the map to, want to extend before present
     * need.  Want to initialize at least 4 elements before min_usage_offset. 
     * 
     * In order to do some hand unrolling optimizations of the resize
     * and intialization routines, always make the number of elements
     * initialized to a multiple of 4.  Thus make new_min_init_offset a 
     * (multiple of 4, since want numbering to start at a multiple of 4).
     * 
     * To accomplish this, zero the lower bits of min_usage offset 
     * (in effect, subtract up to 3).  Now have a multiple of 4.
     * Subtract 4, so the new_min_init_offset points somewhere between 
     * 4 and 7 entries before min_usage_offset, and is a multiple of 4.
     */
    new_min_init_offset = (min_usage_offset - (min_usage_offset & 3)) - 4;

    /* Do we need to resize the map_array? 
     * If min_usage_offset would fall withing the current array, don't
     * resize, just extend initialization as far as possible.
     */
    map_start_offset = sm_cb->map_start_offset;
    if (new_min_init_offset < map_start_offset)
    {
	/* Try to scale back new_min_init_offset if possible to avoid resize */
	if (map_start_offset <= min_usage_offset)
	{
	    /* Since map_start_offset always a multiple of 4, unrolling
	     * optimizations will still hold after setting to map_start_offset.
	     */
	    new_min_init_offset = map_start_offset;
	}

	/* Otherwise expand the map_start_offset to handle 
	 * new_min_init_offset 
	 */
	else
	{
	    /* Changes the map start offset, so reload after expansion */
	    OLD_SM_expand_map_start_offset (sm_cb, new_min_init_offset);
	    map_start_offset = sm_cb->map_start_offset;
	}
    }

    /* Get sm_cb fields for speed and ease of use */
    old_min_init_offset = sm_cb->min_init_offset;

    /*
     * First calculate offset range into map array, using map start offset
     * to adjust offset (map_array[0] represents map_start_offset).
     */
    start_index = new_min_init_offset - map_start_offset;
    end_index = (old_min_init_offset -1) - map_start_offset;

#if DEBUG_RMAP
    printf ("  Expanding min init offset from %i to %i, index %i to %i\n",
	    sm_cb->min_init_offset, new_min_init_offset, end_index, 
	    start_index);
#endif


    /* Initialize the necessary portions of the map.
     * Do 4 intializations per iteration to speed up the initialization.
     * I set up algorithm so that there is always a multiple of 4 elements 
     * need to be initialized!
     */
    map_array = sm_cb->map_array;
    for (index1 = start_index; index1 <= end_index;
	 index1 += 4)
    {
	map_array[index1] = 0 ;
	map_array[index1+1] = 0;
	map_array[index1+2] = 0;
	map_array[index1+3] = 0;

#if DEBUG_RMAP_RESIZE
	map_array[index1] = map_start_offset + index1 ;
	printf ("Setting map_array[%i] to %i\n", index1, map_array[index1]);
	map_array[index1+1] = map_start_offset + index1 +1;
	map_array[index1+2] = map_start_offset + index1 +2;
	map_array[index1+3] = map_start_offset + index1 +3;
#endif
    }
    
    /* Set the new min_init_offset */
    sm_cb->min_init_offset = new_min_init_offset;

#if DEBUG_RMAP_RESIZE
    OLD_SM_check_map (sm_cb);
#endif
}

void OLD_SM_create_map (OLD_SM_Cb *sm_cb, int min_usage_offset, int max_usage_offset)
{
    int map_start_offset, map_array_size, malloc_size, min_init_offset;
    unsigned int *map_array;

    /* Start the map about 4-7 elements before the min_usage_offset, so
     * map can grow for a while in either direction (biased slightly
     * towards growing towards larger offsets).  
     * For loop optimizations, aways place map_start_offset on a offset that 
     * is a multiple of 4 (Want lower two bits to be zero to prevent
     * messing up the loop bound calculations).  
     */
    map_start_offset = (min_usage_offset - (min_usage_offset & 3)) - 4;

    /* Initially try to make map 32 elements long, grow if necessary.
     * Must be a power of two.  May want to make map initally larger.
     * Map is not intialized until needed, so making it map larger
     * initially decreases the number of resizes needed later.
     */
    map_array_size = 32;
    while ((map_start_offset + map_array_size) <= max_usage_offset)
	map_array_size = map_array_size << 1;



    /* Malloc map array */
    malloc_size = map_array_size * sizeof(unsigned int);
    if ((map_array = (unsigned int *) malloc(malloc_size)) == NULL)
	L_punt ("OLD_SM_create_map: Out of memory (size %i)", malloc_size);    

    /* Initialize sm_cb data structures */
    sm_cb->map_array = map_array;
    sm_cb->map_array_size = map_array_size;
    sm_cb->map_start_offset = map_start_offset;
    sm_cb->map_end_offset = map_start_offset + map_array_size - 1;
    
    /* Set the max_init_offset to be right before min_usage_offset and
     * min_init_offset to be max_init_offset + 1.
     * My loop optimization requires that max_init_offset be a 
     * (multiple of 4) -1! So move back min/max_init_offset appropriately.
     *
     * If min_usage_offset is 0, will get a min_init_offset of 0 and
     * a max_init_offset of -1.  (I.e., nothing initialized!)
     */
    min_init_offset = (min_usage_offset - (min_usage_offset & 3));
    sm_cb->min_init_offset = min_init_offset;
    sm_cb->max_init_offset = min_init_offset - 1;

#if DEBUG_RMAP
    printf ("    Creating map: min %i max %i start %i end %i size %i min init %i max init %i\n", 
	    min_usage_offset, max_usage_offset, map_start_offset, 
	    sm_cb->map_end_offset,
	    map_array_size, min_init_offset, sm_cb->max_init_offset);
#endif

    /* Initialize the map from min_init_offset to max_usage_offset */
    OLD_SM_init_for_max_usage (sm_cb, max_usage_offset);
}

/* To get all the resource map worked out */
int OLD_SM_sched_table (OLD_SM_Cb *sm_cb, SM_Table *table, int time, 
		    unsigned short min_slot, unsigned short max_slot,
		    int commit, Mdes_Stats *stats)
{
    SM_Mdes *sm_mdes;
    int min_usage_offset, max_usage_offset, map_start_offset;
    unsigned int *map_array, *adjusted_map_array;
    unsigned short choice_array[100];
    int slot;
    int start_option_checks, start_usage_checks;
    int net_option_checks, net_usage_checks;

#if TAKE_STATS
    stats->num_table_checks ++;
#endif

    /* Get the SM mdes for ease of use */
    sm_mdes = sm_cb->sm_mdes;

    /* Calculate the min and max offset that will be tested for this table 
     * If more than 32 resources are used, then multiple map words per
     * cycle will be used.  This is handled using a time shift to calculate
     * the usage offsets.
     */
    min_usage_offset = time << sm_mdes->time_shift;
    max_usage_offset = min_usage_offset + table->max_usage_offset;

#if DEBUG_RMAP
    printf ("Sched table at time %i, min offset %i, max offset %i\n", time, 
	    min_usage_offset, max_usage_offset);
#endif

    /* Create the map_array and initialize fields if necessary */
    if (sm_cb->map_array == NULL) 
	OLD_SM_create_map (sm_cb, min_usage_offset, max_usage_offset);

    /* If max_usage_offset extends past where initialized, initialize more */
    if (max_usage_offset > sm_cb->max_init_offset)
	OLD_SM_init_for_max_usage (sm_cb, max_usage_offset);


    /* If min_usage_offset extends below where initialized, initialize more */
    if (min_usage_offset < sm_cb->min_init_offset)
	OLD_SM_init_for_min_usage (sm_cb, min_usage_offset);

    /* Get the pointer to the map array, adjusting it work the schedule
     * time specified and for the map start offset.  The above functions
     * may change both map_array and map_start_offset, so must be
     * reloaded here!
     */
    map_array = sm_cb->map_array;
    map_start_offset = sm_cb->map_start_offset;
    adjusted_map_array = &map_array[min_usage_offset - map_start_offset];
    
#if DEBUG_RMAP
    for (index = 0; index < table->num_choices; index++)
    {
	choice_array[index] = -1;
    }
#endif

#if TAKE_STATS
    /* Update choice distribution */
    increment_check_history (stats->num_choice_dist, 
			     (int)(table->last_choice -
				   table->first_choice) + 1, 1);
    increment_check_history (stats->first_choice_dist, 
			     (int)(table->first_choice->last_option-
				   table->first_choice->first_option)+1, 1);
#endif

    /* Use existing stats to calculate net checks for this table */
    start_option_checks = stats->num_option_checks;
    start_usage_checks = stats->num_usage_checks;

    /* Try to schedule the table at this time */
    slot = OLD_SM_choose_first_avail_options (adjusted_map_array, table,
					  choice_array, min_slot, max_slot,
					  stats);
	
    net_option_checks = stats->num_option_checks - start_option_checks;
    net_usage_checks = stats->num_usage_checks - start_usage_checks;

    /* Were we successful? */
    if (slot >= 0)
    {
#if TAKE_STATS
	/* Yes, update succeed distribution */
	increment_check_history (stats->succeed_option_check_dist,
				 net_option_checks, 1);
	increment_check_history (stats->succeed_usage_check_dist,
				 net_usage_checks, 1);
#endif

	if (commit)
	{
#if DEBUG_RMAP
	    printf ("\n> Scheduling table at time %i in slot %i\n", time, 
		    slot);
	    for (index = 0; index < table->num_choices; index++)
	    {
		printf ("  Choice %i: option %i\n", index, 
			choice_array[index]);
	    }
	    printf ("\n");
#endif
	    
	    OLD_SM_commit_choices (adjusted_map_array, table, choice_array);
	}
	return (slot);
    }
    else
    {

#if TAKE_STATS
	/* No, update fail distribution */
	increment_check_history (stats->fail_option_check_dist,
				 net_option_checks, 1);
	increment_check_history (stats->fail_usage_check_dist,
				 net_usage_checks, 1);
#endif
#if DEBUG_RMAP
	if (commit)
	{
	    printf ("Unable to schedule table at time %i (return value %i)\n", 
		time, slot);
	}
#endif
#if TAKE_STATS
	stats->num_table_checks_failed ++;
#endif
	return (-1);
    }
}


int OLD_SM_choose_first_avail_options (unsigned int *map, SM_Table *table,
				   unsigned short *choices_made, 
				   unsigned int min_slot, 
				   unsigned int max_slot,
				   Mdes_Stats *stats)
{
    SM_Choice *choice_ptr, *first_choice, *last_choice, *slot_choice;
    SM_Option *option_ptr, *first_option, *last_option;
    SM_Usage *usage_ptr, *first_usage, *last_usage;
    unsigned short option_id, *choices_made_ptr;
    unsigned short *slot_options_ptr;
    unsigned int slot = 0;
    unsigned int map_contents, resources_used, map_offset;

    /* Get pointer to choices_made array for ease of use */
    choices_made_ptr = choices_made;

    /* Get the choice that sets the slot option for ease of use & speed */
    slot_choice = table->slot_choice;

    /* Get pointer to the slot options for the slot choice */
    slot_options_ptr = table->slot_options;

    /* Cache the choice array bounds for ease of use & speed */
    first_choice = table->first_choice;
    last_choice = table->last_choice;


    /* For each choice, pick the first option that is available */
    choice_ptr = first_choice;
    while (1)
    {
#if DEBUG_RMAP 
	printf ("    Testing choice %i:\n", (int) (choice_ptr - first_choice));
#endif

	/* Cache the option array bounds for ease of use & speed */
	first_option = choice_ptr->first_option;
	last_option = choice_ptr->last_option;

	/* Initialize the option id used for setting *choices_made_ptr */
	option_id = 0;

	/* Pick the first available option that has its resources available */
	option_ptr = first_option;
	while (1)
	{
#if DEBUG_RMAP
	    printf ("      Testing option %i:\n", 
		    (int) (option_ptr - first_option));
#endif
#if TAKE_STATS
	    stats->num_option_checks ++;
#endif


	    /* Cache the usage array bounds for ease of use & speed */
	    first_usage = option_ptr->first_usage;
	    last_usage = option_ptr->last_usage;
	    
	    /* In order to choice this option, all resources used by
	     * the usage array must be available in the resource map 
	     */
	    usage_ptr = first_usage;
	    while (1)
	    {

		/* Cache the usage contents for ease of use & speed */
		map_offset = usage_ptr->map_offset;
		resources_used = usage_ptr->resources_used;

#if TAKE_STATS
		stats->num_usage_checks ++;
#endif

#if DEBUG_RMAP 
		printf ("        Testing usage %i (offset %i mask %x)\n",
			(int)(usage_ptr - first_usage), map_offset, 
			resources_used);
#endif

		/* Get the contents of the resource map at this offset */
		map_contents = map[map_offset];

		/* Goto next usage while loading map_contents */
		usage_ptr++;

		/* Are the desired resources available (all desired bits 0)? */
		if ((map_contents & resources_used) != 0)
		{
#if DEBUG_RMAP 
		    printf ("          Resources %x not available\n",
			    (map_contents & resources_used));
#endif
#if TAKE_STATS
		    stats->num_usage_checks_failed ++;
		    stats->num_option_checks_failed ++;
#endif

		    /* Resources not available, flag by setting usage_ptr
		     * to NULL and exit loop so can try the next option 
		     */
		    usage_ptr = NULL;
		    break;
		}
		
		/* Stop if have gotten all the desired resources */
		if (usage_ptr > last_usage)
		    break;
	    }
	    
	    /* For the slot choice, the option chosen also determines the
	     * schedule slot.  Test only the slot choice options that
	     * fall within the min_slot and max_slot bounds
	     */
	    if (choice_ptr == slot_choice)
	    {
		if (usage_ptr != NULL)
		{
		    /* Get the slot for this option and update pointer*/
		    slot = *slot_options_ptr;
		    slot_options_ptr++;
		    
#if DEBUG_RMAP
		    printf ("        Testing slot %i (dep allows %i-%i).\n",
			    slot, min_slot, max_slot);
#endif
#if TAKE_STATS
		    stats->num_slot_checks ++;
#endif
		    
		    
		    /* Reject option if slot doesn't fall within required bounds */
		    if ((slot < min_slot) || (slot > max_slot))
		    {
#if DEBUG_RMAP
			printf ("          Slot %i rejected (dep allows %i-%i).\n",
				slot, min_slot, max_slot);
#endif
#if TAKE_STATS
			stats->num_slot_checks_failed ++;

			/* Update usage and option failed stats since
			 * they have failed due to slot constraints.
			 */
			stats->num_usage_checks_failed ++;
			stats->num_option_checks_failed ++;
#endif
			
			/* Update state so we can goto next option */
			option_ptr++;
			option_id++;
			
			/* If we have used up all the options and have not found
			 * one that can be scheduled, return -1
			 */
			if (option_ptr > last_option)
			    return (-1);
			
			/* Goto next option */
			continue;
		    }
		}

		/* If resources are not available, just update pointer */
		else
		{
		    slot_options_ptr++;
		}
	    }

	    /* If option is available (usage_ptr != NULL), set the
	     * option used in the choices made array and exit loop
	     * so can go to next choice.
	     */
	    if (usage_ptr != NULL)
	    {
		
		/* If here, then resources and slot constraints are met! */

#if DEBUG_RMAP
		if (lmdes->mdes2 != NULL)
		{
		    printf ("  Option %i available:\n",
			    (int) (option_ptr - first_option));
		    OLD_SM_print_option (stdout, lmdes->mdes2->sm_mdes, 
				     option_ptr);
		}
#endif				     

		/* Update the choices_made array */
		*choices_made_ptr = option_id;
		
		/* Goto next choice */
		break;
	    }

	    /* All loop update code (below this point) must also be placed
	     * in the slot bound testing code at the beginning of this loop!
	     */

	    /* Update option_ptr and option_id before trying next option */
	    option_ptr++;
	    option_id++;

	    /* If we have used up all the options and have not found
	     * one that can be scheduled, return -1
	     */
	    if (option_ptr > last_option)
		return (-1);
	}

	/* Update choice_ptr and choices_made_ptr before try next choice */
	choice_ptr++;
	choices_made_ptr++;

	/* Stop if we have sucessfully selected an option for each choice */
	if (choice_ptr > last_choice)
	    break;
    }

    /* We have successfully selected the first option available for each
     * choice.  Return the slot choosen to signal success.
     */
    return ((int)slot);
}

void OLD_SM_commit_choices (unsigned int *map, SM_Table *table,
		       unsigned short *choices_made)
{
    SM_Choice *choice_ptr, *first_choice, *last_choice;
    SM_Option *option_ptr, *option_array;
    SM_Usage *usage_ptr, *first_usage, *last_usage;
    unsigned short option_id, *choices_made_ptr;
    unsigned int map_contents, resources_used, map_offset;


    /* Get pointer to choices_made array for clarity */
    choices_made_ptr = choices_made;

    /* Cache the choice array bounds for ease of use & speed */
    first_choice = table->first_choice;
    last_choice = table->last_choice;


    /* Commit all the resources for each specified choice */
    choice_ptr = first_choice;
    while (1)
    {

#if DEBUG_RMAP
        printf ("    Committing choice %i:\n", 
		(int) (choice_ptr - first_choice));
#endif

	/* Load the option id chosen */
	option_id = *choices_made_ptr;

	/* Load the option array for this choice */
	option_array = choice_ptr->first_option;

	/* Get a pointer to the option selected */
	option_ptr = &option_array[option_id];
	
#if DEBUG_RMAP
	printf ("      Committing option %i:\n",
                    (int) (option_ptr - option_array));
#endif

	/* Cache the usage array bounds for ease of use & speed */
	first_usage = option_ptr->first_usage;
	last_usage = option_ptr->last_usage;

	/* In order to choice this option, all resources used by
	 * the usage array must be available in the resource map
	 */
	usage_ptr = first_usage;
	while (1)
	{
	    /* Cache the usage contents for ease of use & speed */
	    map_offset = usage_ptr->map_offset;
	    resources_used = usage_ptr->resources_used;


	    /* Get the contents of the resource map at this offset */
	    map_contents = map[map_offset];

	    /* Mark the resources as used in the map_contents */
	    map_contents = map_contents | resources_used;

#if DEBUG_RMAP
	    printf ("        Committing usage %i (offset %i mask %x). Before %x after %x\n",
		    (int)(usage_ptr - first_usage), map_offset,
		    resources_used, map[map_offset], map_contents);
#endif

	    /* Store the new map contents to the map */
	    map[map_offset] = map_contents;


	    
	    /* Goto next usage */
	    usage_ptr++;

	    /* Stop after committing all the desired resource usages */
	    if (usage_ptr > last_usage)
		break;
	}

	/* Update choice_ptr and choices_made_ptr before committing the next 
	 * choice 
	 */
        choice_ptr++;
        choices_made_ptr++;

        /* Stop if we have committed each choice */
        if (choice_ptr > last_choice)
            break;
    }
}

/* Prints the resource usages for the passed option to the file "out".  */
void OLD_SM_print_option (FILE *out, SM_Mdes *sm_mdes, SM_Option *option)
{
    SM_Usage *usage;
    unsigned short resource_offset, offset_mask;
    SM_Resource *resource;
    unsigned int num_resources, time_shift, usage_time, resources_used;
    int index;
    
    /* Cache some sm_mdes fields for speed and ease of use */
    num_resources = sm_mdes->num_resources;
    time_shift = sm_mdes->time_shift;
    resource = sm_mdes->resource;

    offset_mask = ((unsigned short) sm_mdes->map_width) - 1;

    for (usage = option->first_usage; usage <= option->first_usage;
	 usage++)
    {
	resources_used = usage->resources_used;
	resource_offset = usage->map_offset & offset_mask;
	usage_time = usage->map_offset >> time_shift;
	fprintf (out, " @%2i:", usage_time);
	
	for (index=0; index < num_resources; index++)
	{
	    if ((resource_offset == resource[index].offset) &&
		((resources_used & resource[index].mask) != 0))
		fprintf (out, " %s", resource[index].name);
	}
	fprintf (out, "\n");
    }
}

void OLD_SM_print_map (FILE *out, SM_Mdes *sm_mdes, OLD_SM_Cb *sm_cb)
{
    int start_time, end_time, time, time_shift, num_resources, usage_offset;
    int map_start_offset, offset_per_cycle, empty;
    SM_Resource *resource_array, *resource;
    unsigned int *adjusted_map_array, *map_array;
    int index1, index2;

    /* Get the time shift for the mdes */
    time_shift = sm_mdes->time_shift;
    offset_per_cycle = 1 << time_shift;

    map_array = sm_cb->map_array;
    map_start_offset = sm_cb->map_start_offset;

    start_time = sm_cb->min_init_offset >> time_shift;
    end_time = ((sm_cb->max_init_offset + 1) >> time_shift) -1;

    /* Move start time up until the first cycle resources are used */
    while (start_time <= end_time)
    {
	usage_offset = start_time << time_shift;

	/* Move adjusted_map to the start of this cycle */
	adjusted_map_array = &map_array[usage_offset - map_start_offset];

	empty = 1;
	for (index1 = 0; index1 < offset_per_cycle; index1++)
	{
	    if (adjusted_map_array[index1] != 0)
	    {
		empty = 0;
		break;
	    }
	}
	if (empty)
	{
	    start_time++;
	}
	else
	    break;
    }

    /* Move end time up until the last cycle resources are used */
    while (end_time >= start_time)
    {
	usage_offset = end_time << time_shift;

	/* Move adjusted_map to the start of this cycle */
	adjusted_map_array = &map_array[usage_offset - map_start_offset];

	empty = 1;
	for (index1 = 0; index1 < offset_per_cycle; index1++)
	{
	    if (adjusted_map_array[index1] != 0)
	    {
		empty = 0;
		break;
	    }
	}
	if (empty)
	{
	    end_time--;
	}
	else
	    break;
    }

    num_resources = sm_mdes->num_resources;
    resource_array = sm_mdes->resource;
    
    for (time = start_time; time <= end_time; time++)
    {
	fprintf (out, "%3i:", time);

	usage_offset = time << time_shift;

	/* Move adjusted_map to the start of this cycle */
	adjusted_map_array = &map_array[usage_offset - map_start_offset];

	for (index1=0; index1 < num_resources; index1++)
	{
	    resource = &resource_array[index1];
	    if (adjusted_map_array[resource->offset] & resource->mask)
		fprintf (out, " %s", resource->name);
	    else
	    {
		fprintf (out, " ");
		for (index2 = 0; resource->name[index2] != 0; index2++)
		    fprintf (out, " ");
	    }
	}
	fprintf (out, "\n");
    }
}


void OLD_SM_print_stats (FILE *out, Mdes_Stats *stats)
{
    fprintf (out, "%12i Operation checks.\n", stats->num_oper_checks);
    fprintf (out, "%12i Operation checks failed.\n", 
	     stats->num_oper_checks_failed);

    fprintf (out, "%12i Tables checks.\n", stats->num_table_checks);
    fprintf (out, "%12i Tables checks failed.\n", 
	     stats->num_table_checks_failed);

    fprintf (out, "%12i Slot checks.\n", stats->num_slot_checks);
    fprintf (out, "%12i Slot checks failed.\n", 
	     stats->num_slot_checks_failed);

    fprintf (out, "%12i Usage checks.\n", stats->num_usage_checks);
    fprintf (out, "%12i Usage checks failed.\n", 
	     stats->num_usage_checks_failed);
}
