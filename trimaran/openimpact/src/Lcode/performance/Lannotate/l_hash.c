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
 *      File:   l_hash.c
 *      Author: Teresa Johnson
 *      Creation Date:  1994
 *      Copyright (c) 1994 Teresa Johnson, Wen-mei Hwu and
 *                         The Board of Trustees of the University of Illinois.
 *                         All rights reserved.
 *      The University of Illinois software License Agreement
 *      specifies the terms and conditions for redistribution.
\*****************************************************************************/

#include <config.h>

#ifndef lint
#define lint
static char copyright[] =
"@(#) Copyright (c) 1994 Teresa Johnson, Wen-mei Hwu and The Board of \n\
 Trustees of the University of Illinois. All rights reserved.\n";
#endif

#include <Lcode/l_main.h>
#include <Lcode/l_main.h>
#include "l_hash.h"

/* L_Alloc_Pool's */
L_Alloc_Pool *L_hash_pool = NULL;
L_Alloc_Pool *L_string_hash_pool = NULL;
L_Alloc_Pool *L_position_pool = NULL;

/* Hash Tables */
L_hash_node *op_hash[AN_HASH_SIZE];
L_hash_node *cb_hash[AN_HASH_SIZE];
L_string_hash_node *string_hash[AN_HASH_SIZE];

void L_init_hash(void)
{
	int i;

        if (!L_hash_pool)
           L_hash_pool = 
                L_create_alloc_pool("L_hash_pool",sizeof(L_hash_node),1);
	for (i=0;i<AN_HASH_SIZE;i++) op_hash[i] = NULL;
	for (i=0;i<AN_HASH_SIZE;i++) cb_hash[i] = NULL;
}

void L_hash_insert_op_attr(L_Attr *attr,int id)
{
	int i = id & (AN_HASH_SIZE-1);
	L_hash_node *this;
	L_Attr *cur_attr,*old_attr;

	if (!(cur_attr = L_get_op_attr(id)))
	{
	   if (!op_hash[i])
	   {
	      op_hash[i] = (L_hash_node *) L_alloc(L_hash_pool);
	      this = op_hash[i];
	   }
	   else
	   {
	      this = op_hash[i];
	      while (this->next) this = this->next;
	      this->next = (L_hash_node *) L_alloc(L_hash_pool);
	      this = this->next;
	   }
	   this->attr = attr;
	   this->id = id;
	   this->next = NULL;
	}
	else
	{
	  if ((old_attr = L_find_attr(cur_attr,attr->name)))
	   {
	      old_attr->field[0]->value.i += attr->field[0]->value.i;
	      L_delete_all_attr(attr);
	   }
	   else
	      cur_attr = L_concat_attr(cur_attr,attr);
	}
}

void L_hash_insert_cb_attr(L_Attr *attr,int id)
{
	int i = id & (AN_HASH_SIZE-1);
	L_hash_node *this;
	L_Attr *cur_attr,*old_attr;

	if (!(cur_attr = L_get_cb_attr(id)))
	{
	   if (!cb_hash[i])
	   {
	      cb_hash[i] = (L_hash_node *) L_alloc(L_hash_pool);
	      this = cb_hash[i];
	   }
	   else
	   {
	      this = cb_hash[i];
	      while (this->next) this = this->next;
	      this->next = (L_hash_node *) L_alloc(L_hash_pool);
	      this = this->next;
	   }
	   this->attr = attr;
	   this->id = id;
	   this->next = NULL;
	}
	else
	{
	  if ((old_attr = L_find_attr(cur_attr,attr->name)))
	   {
	      old_attr->field[0]->value.i += attr->field[0]->value.i;
	      L_delete_all_attr(attr);
	   }
	   else
	      cur_attr = L_concat_attr(cur_attr,attr);
	}
}

void L_delete_hash(void)
{
	int i;
	L_hash_node *this,*next;

	for (i=1;i<AN_HASH_SIZE;i++)
	{
	   this = op_hash[i];
	   while (this)
	   {
	      next = this->next;
	      L_delete_all_attr(this->attr);
	      L_free(L_hash_pool,this);
	      this = next;
	   }
	   this = cb_hash[i];
	   while (this)
	   {
	      next = this->next;
	      L_delete_all_attr(this->attr);
	      L_free(L_hash_pool,this);
	      this = next;
	   }
	}
}

L_Attr *L_get_cb_attr(int id)
{
	L_hash_node *node;

	node = cb_hash[id & (AN_HASH_SIZE-1)];
	while (node)
	{
	   if (node->id == id) return node->attr;
	   node = node->next;
	}
	return NULL;
}

L_Attr *L_get_op_attr(int id)
{
	L_hash_node *node;

	node = op_hash[id & (AN_HASH_SIZE-1)];
	while (node)
	{
	   if (node->id == id) 
	   {
		return node->attr;
	   }
	   node = node->next;
	}
	return NULL;
}

L_hash_node *L_get_cb_hash_node(int i)
{
	return cb_hash[i];
}

L_hash_node *L_get_op_hash_node(int i)
{
	return op_hash[i];
}

void L_init_string_hash(void)
{
	int i;

        if (!L_string_hash_pool)
           L_string_hash_pool = 
                L_create_alloc_pool("L_string_hash_pool",
				sizeof(L_string_hash_node),1);
	for (i=0;i<AN_HASH_SIZE;i++) string_hash[i] = NULL;

        if (!L_position_pool)
           L_position_pool = L_create_alloc_pool("L_position_pool",
				sizeof(L_position_node),1);
}

void L_insert_position(char *name,long position)
{
	L_string_hash_node *node;

	if ((node = string_hash[L_string_hash(name)]))
	{
	   for (;node->next;node=node->next) 
		if (!strcmp(node->name,name)) break;
	   if (strcmp(node->name,name))
	   {
	      node->next = (L_string_hash_node *) L_alloc(L_string_hash_pool);
	      node = node->next;
	      node->n = 0;
	      node->position = NULL;
	      node->next = NULL;
	   }
	}
	else
	{
	   node = string_hash[L_string_hash(name)] = 
		(L_string_hash_node *) L_alloc(L_string_hash_pool);
	   node->n = 0;
	   node->position = NULL;
	   node->next = NULL;
	}
	node->name = strdup(name);
	node->position = L_insert_pos(node->position,position);
	node->n++;
}

L_position_node *L_insert_pos(L_position_node *positions,long position)
{
	L_position_node *node,*head;
	
	if ((head = node = positions))
	{
	   for (;node->next;node=node->next) ;
	   node->next = (L_position_node *) L_alloc(L_position_pool);
	   node = node->next;
	}
	else
	{
	   node = head = (L_position_node *) L_alloc(L_position_pool);
	}
	node->position = position;
	node->next = NULL;
	return head;
}

int L_get_num_positions(char *name)
{
	L_string_hash_node *node;

	node = string_hash[L_string_hash(name)];
	if (!node)
	  L_punt("Function '%s' was not found in the index file hash table",
		name);
	return node->n;
}

long L_get_position(char *name,int i)
{
	int n = i;
	L_string_hash_node *node;
	L_position_node *pos;

	node = string_hash[L_string_hash(name)];
	if (n >= node->n) 
		L_punt("L_get_position: Illegal position number %d",n);
	while (node)
	{
	   if (!strcmp(node->name,name)) 
	   {
	      pos = node->position;
	      for  (;n;n--)
		pos = pos->next;
	      return pos->position;
	   }
	   node = node->next;
	}
	L_punt("Function '%s' was not found in the index file hash table",
		name);
        return 0;
}

void L_delete_string_hash(void)
{
	int i;
	L_string_hash_node *this,*next;

	for (i=1;i<AN_HASH_SIZE;i++)
	{
	   this = string_hash[i];
	   while (this)
	   {
	      next = this->next;
	      L_free(L_string_hash_pool,this);
	      this = next;
	   }
	}
}

int L_string_hash(char *str)
{
	int hash = 0;
	char *ptr;

	for (ptr=str;*ptr;ptr++)
	{
	   hash = hash << 1;
	   hash += *ptr;
	}
	return (hash & (AN_HASH_SIZE - 1));
}

L_string_hash_node *L_get_string_hash(int i)
{
	return string_hash[i];
}
