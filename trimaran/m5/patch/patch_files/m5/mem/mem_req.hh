/*
 * Copyright (c) 2002, 2003, 2004, 2005
 * The Regents of The University of Michigan
 * All Rights Reserved
 *
 * This code is part of the M5 simulator, developed by Nathan Binkert,
 * Erik Hallnor, Steve Raasch, and Steve Reinhardt, with contributions
 * from Ron Dreslinski, Dave Greene, Lisa Hsu, Kevin Lim, Ali Saidi,
 * and Andrew Schultz.
 *
 * Permission is granted to use, copy, create derivative works and
 * redistribute this software and such derivative works for any
 * purpose, so long as the copyright notice above, this grant of
 * permission, and the disclaimer below appear in all copies made; and
 * so long as the name of The University of Michigan is not used in
 * any advertising or publicity pertaining to the use or distribution
 * of this software without specific, written prior authorization.
 *
 * THIS SOFTWARE IS PROVIDED AS IS, WITHOUT REPRESENTATION FROM THE
 * UNIVERSITY OF MICHIGAN AS TO ITS FITNESS FOR ANY PURPOSE, AND
 * WITHOUT WARRANTY BY THE UNIVERSITY OF MICHIGAN OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE. THE REGENTS OF THE UNIVERSITY OF MICHIGAN SHALL NOT BE
 * LIABLE FOR ANY DAMAGES, INCLUDING DIRECT, SPECIAL, INDIRECT,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, WITH RESPECT TO ANY CLAIM
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OF THE SOFTWARE, EVEN
 * IF IT HAS BEEN OR IS HEREAFTER ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGES.
 */

/**
 * @file
 * Declarte the memory request object and some builder functions.
 */

#ifndef __MEM_REQ_HH__
#define __MEM_REQ_HH__

#include <list>

#include "mem/mem_cmd.hh"

#include "base/fast_alloc.hh"
#include "base/refcnt.hh"

#include "sim/root.hh"
#include "targetarch/isa_traits.hh"

class ExecContext;
class Event;

/** The request is a Load locked/store conditional. */
const unsigned LOCKED		= 0x001;
/** The virtual address is also the physical address. */
const unsigned PHYSICAL		= 0x002;
/** The request is an ALPHA VPTE pal access (hw_ld). */
const unsigned VPTE		= 0x004;
/** Use the alternate mode bits in ALPHA. */
const unsigned ALTMODE		= 0x008;
/** The request is to an uncacheable address. */
const unsigned UNCACHEABLE	= 0x010;
/** The request should not cause a page fault. */
const unsigned NO_FAULT         = 0x020;
/** The request was satisfied */
const unsigned SATISFIED	= 0x040;
/** The request contains compressed data. */
const unsigned COMPRESSED       = 0x080;
/** The request should be prefetched into the exclusive state. */
const unsigned PF_EXCLUSIVE	= 0x100;
/** The request should be marked as LRU. */
const unsigned EVICT_NEXT	= 0x200;
/** The request will result in a cache line fill. */
const unsigned CACHE_LINE_FILL  = 0x400;
/** The request is from the instruction cache. */
const unsigned INST_READ        = 0x800;
/** The request is a copy pending on the first source block. */
const unsigned COPY_SOURCE1     = 0x1000;
/** The request is a copy pending on the second source block. */
const unsigned COPY_SOURCE2     = 0x2000;
/** The request is a copy pending on the first destination block. */
const unsigned COPY_DEST1       = 0x4000;
/** The request is a copy pending on the first destination block. */
const unsigned COPY_DEST2       = 0x8000;
/** The request saw another cache that had the block in shared state. */
const uint32_t SHARED_LINE      = 0x10000; 
/** The request is being NACKed. */
const uint32_t NACKED_LINE      = 0x20000;
/** The request should not allocate a line. */
const uint32_t NO_ALLOCATE      = 0x40000;

/** Mask to quickly check copy pending bits. */
const unsigned COPY_PENDING_MASK = 0xf000;


// Forward declaration for pointer
class MSHR;

/**
 * A basic memory request object that contains the fields necessary to complete * a memory access.
 * @todo Compress the data in this structure.
 * @todo Make these fast alloced again.
 */
class MemReq : public FastAlloc, public RefCounted
{
  public:
    /** Define the invalid address. */
    static const Addr inval_addr = ULL(0xffffffffffffffff);

    /** The virtual address of the request. */
    Addr vaddr;
    /** The physical address of the request. */
    Addr paddr;

    /** Destination address for copies. */
    Addr dest;

    /** The command to perform. */
    MemCmd cmd;
    /** Pointer to the associated MSHR. */
    MSHR *mshr;

    /** whether this req came from the CPU or not */
    bool nic_req;

    /**
     * Bus ID of the requesting interface. Used to quickly forward the reponse
     * across the bus.
     */
    int busId;

    /** The address space ID. */
    int asid;
    /** The related execution context. */
    ExecContext *xc;
    /** The size of the request. */
    int size;
    /** The actual size of the data in a compressed request. */
    int actualSize;
    /** The return value of store conditional. */
    uint64_t result;
    /** Storage for the various bit flags. */
    uint32_t flags;

    /** The event to schedule upon completion of this request. */
    Event *completionEvent;

    /** The cpu number for statistics. */
    int cpu_num;
    /** The requesting  thread id. */
    int  thread_num;
    /** The time this request was started. Used to calculate latencies. */
    Tick time;

    /** program counter of initiating access; for tracing/debugging */
    Addr pc;

    /*  M5elements                                             */
    /** register number of initiating access; for SIMU tracing */
    int register_id;                 // <lieberm 05/19/2005>

    /** The block offset of this request. */
    int offset;
    /** Storage for any data transfered. */
    uint8_t *data;
	  
    /**
     * Contruct and initialize a memory request.
     * @param va The virtual address.
     * @param ec The execution context.
     * @param _size The size of the request.
     * @param _flags The initial flags of the request.
     */
    MemReq(Addr va = inval_addr,
	   ExecContext *ec = NULL,
	   int _size = 0,
	   unsigned _flags = 0)
	: vaddr(va),
	  paddr(inval_addr),
	  dest(inval_addr),
	  mshr(0),
	  nic_req(false),
	  busId(0),
	  asid(0),
	  xc(ec),
	  size(_size), flags(_flags),
	  completionEvent(NULL),
	  //cpu_num(0), 
	  thread_num(0),
	  time(0),
	  pc(0),
	  offset(0),
	  data(NULL)
    {
    }

    MemReq(const MemReq &r)
    {
        vaddr = r.vaddr;
        paddr = r.paddr;
        dest = r.dest;
        cmd = r.cmd;
        mshr = r.mshr;
        nic_req = r.nic_req;
        busId = r.busId;
        asid = r.asid;
        xc = r.xc;
        size = r.size;
        flags = r.flags;
        completionEvent = r.completionEvent;
        thread_num = r.thread_num;
        time = r.time;
        pc = r.pc;
        offset = r.offset;
        data = r.data;
    }

    /**
     * Destructor.
     */
    ~MemReq()
    {
	mshr = NULL;
	xc = NULL;
	if (data)
	    delete [] data;
    }

    /**
     * Reset the memory request to the new paramters.
     * @param _va The virtual address, defaults to invalid address.
     * @param _size The request size, defaults to 0.
     * @param _flags The request flags, defaults to 0.
     */
    void reset(Addr _va = inval_addr, int _size = 0, unsigned _flags = 0)
    {
	vaddr = _va;
	size = _size;
	flags = _flags;
	paddr = inval_addr;
    }

    /**
     * Returns true if this request is satisfied.
     * @return true if satisfied.
     */
    const bool isSatisfied() const
    {
	return flags & SATISFIED;
    }

    /**
     * Returns true if this request is NACKed.
     * @return true if NACKed.
     */
    const bool isNacked() const
    {
	return flags & NACKED_LINE;
    }

    /**
     * Returns true if this request is uncacheable.
     * @return true if this request is uncacheable.
     */
    const bool isUncacheable() const
    {
	return flags & UNCACHEABLE;
    }

    /**
     * Returns true if this request is uncacheable.
     * @return true if this request is uncacheable.
     */
    const bool isNoAllocate() const
    {
	return flags & NO_ALLOCATE;
    }

    /**
     * Returns true if this request is a cache line fill.
     * @return true if this request is a cache line fill.
     */
    const bool isCacheFill() const
    {
	return flags & CACHE_LINE_FILL;
    }

    /**
     * Returns true if this is an instruction read request.
     * @return true if this is an instruction read request.
     */
    const bool isInstRead() const
    {
	return flags & INST_READ;
    }

    /**
     * Returns true if this request has compressed data.
     * @return true if this request has compressed data.
     */
    const bool isCompressed() const
    {
	return flags & COMPRESSED;
    }

    /**
     * Checks to see if the given request overlaps this one.
     * @param req Ther request to check.
     * @return True if the requests overlap.
     */
    const bool overlaps(RefCountingPtr<MemReq> & req) const
    {
	if (paddr < req->paddr) {
	    return paddr + size > req->paddr;
	}
	return req->paddr + req->size > paddr;
    }		

    /**
     * Returns true if this request is a pending copy.
     */
    const bool pendingCopy() const
    {
	assert(cmd == Copy);
	return flags & COPY_PENDING_MASK;
    }

    const bool sharedAsserted() const
    {
	return flags & SHARED_LINE;
    }
    
};

/** A ref counted pointer to a MemReq. */
typedef RefCountingPtr<MemReq> MemReqPtr;
/** Typedef for a standard list of MemReqPtr. */
typedef std::list<MemReqPtr> MemReqList;

/** 
 * Return a Writeback request for the given parameters.
 * @param addr The address being written.
 * @param asid The address space ID.
 * @param xc The execution context to write to.
 * @param size The number of bytes being written.
 * @param data A pointer to the data being written.
 * @param compressed_size Number of bytes to write if we are doing compression.
 * @return A reference counted pointer to the generated MemReq.
 */
MemReqPtr buildWritebackReq(Addr addr, int asid, ExecContext *xc, 
			    int size, uint8_t *data,
			    int compressed_size);
#endif //__MEM_REQ_HH__
