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
 *      File:   l_operand_symbol.c
 *      Author: Stolen by Richard E. Hank from John C. Gyllenhaal, Wen-mei Hwu
 *      Creation Date:  1996
\*****************************************************************************/

/* 10/29/02 REK Adding config.h */
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Lcode/l_main.h>
#include "l_operand_symbol.h"
#include <library/l_alloc_new.h>

L_Alloc_Pool *OPERAND_Symbol_Table_pool = NULL;
L_Alloc_Pool *OPERAND_Symbol_pool = NULL;

/***
 *** 
 *** OPERAND routines
 ***
 ***/
int
OPERAND_Hash_Value (L_Operand * opd)
{
  int operand_type_data, operand_value;

  /* Get index into hash table to use for this name */
  operand_type_data = opd->ptype;
  operand_type_data = (operand_type_data << 8) + opd->ctype;
  operand_type_data = (operand_type_data << 8) + opd->type;
  operand_value = opd->value.r;

  return (operand_value);
}

/* Create and initialize OPERAND_Symbol_Table.
 * Creates a hash table of initial size 2 * expected_size rounded up
 * to the closest power of two.  (Min hash size 32)
 */
OPERAND_Symbol_Table *
OPERAND_new_symbol_table (char *name, int expected_size)
{
  OPERAND_Symbol_Table *table;
  OPERAND_Symbol **hash;
  unsigned int min_size, hash_size;
  int i;

  /* If expected size negative, force to be 0 */
  if (expected_size < 0)
    expected_size = 0;

  /* To prevent infinite loop by sizing algorithm (and running out of
   * memory :) ), expected_size must be <= a billion.
   */
  if (expected_size > 1000000000)
    L_punt ("OPERAND_Symbol_Table: unreasonable expected_size (%u)",
            expected_size);

  /* Want a minumum size of at least twice the expected size */
  min_size = expected_size * 2;

  /* Start with a minumum hash size of 32.  
   * (Smaller sizes don't work as well with the hashing algorithm)
   */
  hash_size = 32;

  /* Double hash_size until min_size is reached or exceeded */
  while (hash_size < min_size)
    hash_size = hash_size << 1;


  /* Create new symbol table pool (and symbol pool if necessary) */
  if (OPERAND_Symbol_Table_pool == NULL)
    {
      OPERAND_Symbol_Table_pool = L_create_alloc_pool ("OPERAND_Symbol_Table",
                                                       sizeof
                                                       (OPERAND_Symbol_Table),
                                                       16);
      OPERAND_Symbol_pool =
        L_create_alloc_pool ("OPERAND_Symbol", sizeof (OPERAND_Symbol), 64);
    }

  /* Allocate symbol table */
  table = (OPERAND_Symbol_Table *) L_alloc (OPERAND_Symbol_Table_pool);

  /* Allocate array for hash */
  hash = (OPERAND_Symbol **) malloc (hash_size * sizeof (OPERAND_Symbol *));
  if (hash == NULL)
    {
      L_punt ("OPERAND_new_symbol_table: Out of memory, hash array size %i.",
              hash_size);
    }

  /* Initialize hash table */
  for (i = 0; i < hash_size; i++)
    hash[i] = NULL;

  /* Initialize fields */
  table->name = strdup (name);
  table->hash = hash;
  table->hash_size = hash_size;
  table->hash_mask = hash_size - 1;   /* AND mask, works only for power of 2 */
  /* Resize when count at 75% of hash_size */
  table->resize_size = hash_size - (hash_size >> 2);
  table->head_symbol = NULL;
  table->tail_symbol = NULL;
  table->symbol_count = 0;

  return (table);
}

/* Frees the symbol table, and optionally frees the data pointed to */
void
OPERAND_delete_symbol_table (OPERAND_Symbol_Table * table,
                             void (*free_routine) (void *))
{
  OPERAND_Symbol *symbol, *next_symbol;

  /* For all the symbols in the table, free each one */
  for (symbol = table->head_symbol; symbol != NULL; symbol = next_symbol)
    {
      /* Get the next symbol before deleting this one */
      next_symbol = symbol->next_symbol;

      /* If free routine specified, free data */
      if (free_routine != NULL)
        free_routine (symbol->data);

      /* Free symbol structure */
      L_free (OPERAND_Symbol_pool, symbol);
    }

  /* Free the hash array and table name */
  free (table->hash);
  free (table->name);

  /* Free the table structure */
  L_free (OPERAND_Symbol_Table_pool, table);
}

/* Doubles the symbol table hash array size */
void
OPERAND_resize_symbol_table (OPERAND_Symbol_Table * table)
{
  OPERAND_Symbol **new_hash, *symbol, *hash_head;
  int new_hash_size;
  unsigned int new_hash_mask, new_hash_index;
  int i;

  /* Double the size of the hash array */
  new_hash_size = table->hash_size * 2;

  /* Allocate new hash array */
  new_hash =
    (OPERAND_Symbol **) malloc (new_hash_size * sizeof (OPERAND_Symbol *));
  if (new_hash == NULL)
    {
      L_punt ("OPERAND_resize_symbol_table: Out of memory, new size %i.",
              new_hash_size);
    }

  /* Initialize new hash table */
  for (i = 0; i < new_hash_size; i++)
    new_hash[i] = NULL;

  /* Get the hash mask for the new hash table */
  new_hash_mask = new_hash_size - 1; /* AND mask, works only for power of 2 */

  /* Go though all the symbol and add to new hash table.
   * Can totally disreguard old hash links.
   */
  for (symbol = table->head_symbol; symbol != NULL;
       symbol = symbol->next_symbol)
    {
      L_Operand *opd;

      new_hash_index = symbol->hash_val & new_hash_mask;

      opd = (L_Operand *) symbol->value;

      /* Add symbol to head of linked list */
      hash_head = new_hash[new_hash_index];
      symbol->next_hash = hash_head;
      symbol->prev_hash = NULL;
      if (hash_head != NULL)
        hash_head->prev_hash = symbol;
      new_hash[new_hash_index] = symbol;
    }

  /* Free old hash table */
  free (table->hash);

  /* Initialize table fields for new hash table */
  table->hash = new_hash;
  table->hash_size = new_hash_size;
  table->hash_mask = new_hash_mask;
  /* Resize when count at 75% of new_hash_size */
  table->resize_size = new_hash_size - (new_hash_size >> 2);
}


/* Adds structure to symbol table, data is not copied!!! 
 * Dynamically increases symbol table's hash array.
 * Returns pointer to added symbol.
 */
OPERAND_Symbol *
OPERAND_add_symbol (OPERAND_Symbol_Table * table, L_Operand * opd, void *data)
{
  OPERAND_Symbol *symbol, *hash_head, *check_symbol, *tail_symbol;
  unsigned int hash_index;
  int symbol_count;
  int hash_value;

  /* Increase symbol table size if necessary before adding new symbol.  
   * This will change the hash_mask if the table is resized!
   */
  symbol_count = table->symbol_count;
  if (symbol_count >= table->resize_size)
    {
      OPERAND_resize_symbol_table (table);
    }

  /* Determine the hash value for the operand */
  hash_value = OPERAND_Hash_Value (opd);
  hash_index = hash_value & table->hash_mask;

  /* Get head symbol in current linked list for ease of use */
  hash_head = table->hash[hash_index];

  /* 
   * Check to see if operand is already present in the hash table.
   *
   * If so, return the appropriate symbol.
   */
  for (check_symbol = hash_head; check_symbol != NULL;
       check_symbol = check_symbol->next_hash)
    {
      L_Operand *table_opd = (L_Operand *) check_symbol->value;

      if (check_symbol->hash_val == hash_value &&
          table_opd->type == opd->type &&
          table_opd->ctype == opd->ctype &&
          table_opd->ptype == opd->ptype &&
          table_opd->value.init.u == opd->value.init.u &&
          table_opd->value.init.l == opd->value.init.l)
        {
#if 0
          fprintf (stdout, "Found operand ");
          L_print_operand (stdout, opd);
          fprintf (stdout, " in table with value %d.\n", check_symbol->value);
#endif
          fprintf (stderr, "Operand -> ");
          L_print_operand (stderr, opd, 0);
          fprintf (stderr, "\n");
          L_punt ("%s: cannot add operand, already in table!", table->name);
        }
    }

  /* Allocate a symbol (pool initialized in create table routine) */
  symbol = (OPERAND_Symbol *) L_alloc (OPERAND_Symbol_pool);

  /* Initialize fields */
  symbol->value = (int) opd;
  symbol->hash_val = hash_value;
  symbol->data = data;
  symbol->table = table;

  /* Get tail symbol for ease of use */
  tail_symbol = table->tail_symbol;

  /* Add to linked list of symbols */
  symbol->next_symbol = NULL;
  symbol->prev_symbol = tail_symbol;

  if (tail_symbol == NULL)
    table->head_symbol = symbol;
  else
    tail_symbol->next_symbol = symbol;
  table->tail_symbol = symbol;

  /* Add symbol to head of linked list */
  symbol->next_hash = hash_head;
  symbol->prev_hash = NULL;
  if (hash_head != NULL)
    hash_head->prev_hash = symbol;
  table->hash[hash_index] = symbol;

  /* Update table's symbol count */
  table->symbol_count = symbol_count + 1;

  /* Return symbol added */
  return (symbol);
}

/* Returns a OPERAND_Symbol structure with the desired value, or NULL
 * if the value is not in the symbol table.
 */
OPERAND_Symbol *
OPERAND_find_symbol (OPERAND_Symbol_Table * table, L_Operand * opd)
{
  OPERAND_Symbol *symbol;
  unsigned int hash_index;
  int hash_value;

  /* Get the index into the hash table */
  hash_value = OPERAND_Hash_Value (opd);
  hash_index = hash_value & table->hash_mask;

  /* Search the linked list for matching name */
  for (symbol = table->hash[hash_index]; symbol != NULL;
       symbol = symbol->next_hash)
    {
      L_Operand *table_opd = (L_Operand *) symbol->value;

      if (symbol->hash_val == hash_value &&
          table_opd->type == opd->type &&
          table_opd->ctype == opd->ctype &&
          table_opd->ptype == opd->ptype &&
          table_opd->value.init.u == opd->value.init.u &&
          table_opd->value.init.l == opd->value.init.l)
        {
#if 0
          fprintf (stdout, "Found operand ");
          L_print_operand (stdout, opd);
          fprintf (stdout, " in table with value %d.\n", symbol->value);
#endif
          return (symbol);
        }
    }
  return (NULL);
}

/* 
 * Returns the data for desired value, or NULL
 * if the name is not in the symbol table.
 */
void *
OPERAND_find_symbol_data (OPERAND_Symbol_Table * table, L_Operand * opd)
{
  OPERAND_Symbol *symbol;
  unsigned int hash_index;
  int hash_value;

  /* Get the index into the hash table */
  hash_value = OPERAND_Hash_Value (opd);
  hash_index = hash_value & table->hash_mask;

  /* Search the linked list for matching name */
  for (symbol = table->hash[hash_index]; symbol != NULL;
       symbol = symbol->next_hash)
    {
      L_Operand *table_opd = (L_Operand *) symbol->value;

      if (table_opd->type == opd->type &&
          table_opd->ctype == opd->ctype &&
          table_opd->ptype == opd->ptype &&
          table_opd->value.init.u == opd->value.init.u &&
          table_opd->value.init.l == opd->value.init.l)
        {
#if 0
          fprintf (stdout, "Found operand ");
          L_print_operand (stdout, opd);
          fprintf (stdout, " in table with value %d.\n", symbol->value);
#endif
          return (symbol->data);
        }
    }
  return (NULL);
}

/* Deletes symbol and optionally deletes the data using the free routine */
void
OPERAND_delete_symbol (OPERAND_Symbol * symbol, void (*free_routine) (void *))
{
  OPERAND_Symbol_Table *table;
  OPERAND_Symbol *next_hash, *prev_hash, *next_symbol, *prev_symbol;
  unsigned int hash_index;
  int hash_value;

  /* Get the table the symbol is from */
  table = symbol->table;

  /* Get the hash index from the symbol's value */
  hash_value = OPERAND_Hash_Value ((L_Operand *) symbol->data);
  hash_index = hash_value & table->hash_mask;

  /* Remove symbol from hash table */
  prev_hash = symbol->prev_hash;
  next_hash = symbol->next_hash;
  if (prev_hash == NULL)
    table->hash[hash_index] = next_hash;
  else
    prev_hash->next_hash = next_hash;

  if (next_hash != NULL)
    next_hash->prev_hash = prev_hash;

  /* Remove symbol from symbol list */
  prev_symbol = symbol->prev_symbol;
  next_symbol = symbol->next_symbol;
  if (prev_symbol == NULL)
    table->head_symbol = next_symbol;
  else
    prev_symbol->next_symbol = next_symbol;

  if (next_symbol == NULL)
    table->tail_symbol = prev_symbol;
  else
    next_symbol->prev_symbol = prev_symbol;


  /* If free routine specified, free symbol data */
  if (free_routine != NULL)
    free_routine (symbol->data);

  /* Free symbol structure */
  L_free (OPERAND_Symbol_pool, symbol);

  /* Decrement table symbol count */
  table->symbol_count--;
}


/* Prints out the symbol table's hash table (debug routine) */
void
OPERAND_print_symbol_table_hash (FILE * out, OPERAND_Symbol_Table * table)
{
  OPERAND_Symbol *symbol;
  int hash_index, lines;

  /* Count lines used in table */
  lines = 0;
  for (hash_index = 0; hash_index < table->hash_size; hash_index++)
    {
      if (table->hash[hash_index] != NULL)
        lines++;
    }
  fprintf (out, "%s has %i entries (hash size %i, used %i):\n",
           table->name, table->symbol_count, table->hash_size, lines);

  /* For each hash_index in hash table */
  for (hash_index = 0; hash_index < table->hash_size; hash_index++)
    {
      /* Skip empty lines */
      if (table->hash[hash_index] == NULL)
        continue;

      fprintf (out, "%4i:", hash_index);
      for (symbol = table->hash[hash_index]; symbol != NULL;
           symbol = symbol->next_hash)
        {
          L_print_operand (out, (L_Operand *) symbol->value, 0);
        }
      fprintf (out, "\n");
    }
}
