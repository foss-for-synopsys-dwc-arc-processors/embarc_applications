/* ***** BEGIN LICENSE BLOCK *****
 * Version: RCSL 1.0/RPSL 1.0
 *
 * Portions Copyright (c) 1995-2002 RealNetworks, Inc. All Rights Reserved.
 *
 * The contents of this file, and the files included with this file, are
 * subject to the current version of the RealNetworks Public Source License
 * Version 1.0 (the "RPSL") available at
 * http://www.helixcommunity.org/content/rpsl unless you have licensed
 * the file under the RealNetworks Community Source License Version 1.0
 * (the "RCSL") available at http://www.helixcommunity.org/content/rcsl,
 * in which case the RCSL will apply. You may also obtain the license terms
 * directly from RealNetworks.  You may not use this file except in
 * compliance with the RPSL or, if you have a valid RCSL with RealNetworks
 * applicable to this file, the RCSL.  Please see the applicable RPSL or
 * RCSL for the rights, obligations and limitations governing use of the
 * contents of the file.
 *
 * This file is part of the Helix DNA Technology. RealNetworks is the
 * developer of the Original Code and owns the copyrights in the portions
 * it created.
 *
 * This file, and the files included with this file, is distributed and made
 * available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 *
 * Technology Compatibility Kit Test Suite(s) Location:
 *    http://www.helixcommunity.org/content/tck
 *
 * Contributor(s):
 *
 * ***** END LICENSE BLOCK ***** */

/**************************************************************************************
 * Fixed-point MP3 decoder
 * Jon Recker (jrecker@real.com), Ken Cooke (kenc@real.com)
 * June 2003
 *
 */

#ifndef _ASSEMBLY_H
#define _ASSEMBLY_H
#include "embARC.h"
#include "embARC_debug.h"

#include "embARC_toolchain.h"

#define ACC0_LO 0x580
#define ACC0_HI 0X582


typedef long long Word64;


static __inline Word64 MADD64(Word64 sum, int x, int y)
{
	return (Word64)x * y + sum;
}

static __inline Word64 SHL64(Word64 x, int n)
{
	return (x << n);
}

static __inline Word64 SAR64(Word64 x, int n)
{
	return (x >> n);
}

static __inline int SAR32(int x, int n)
{
	register int y;

	Asm("ASRS %0, %1, %2" : "=r"(y): "r"(x), "r"(n));

	return y;

}

static __inline int SAL32(int x, int n)
{
	register int y;

	Asm("ASLS %0, %1, %2" : "=r"(y): "r"(x), "r"(n));

	return y;

}

static __inline int MULSHIFT32(int x, int y)
{

	register int z;

	Asm("MPYM %0, %1, %2" : "=r"(z): "r"(x), "r"(y));

	return z;
}

static __inline int FASTABS(int x)
{
	register int y;
	Asm("ABSS %0, %1" : "=r"(x):  "r"(y));
	return y;

}

static __inline int CLZ(int x)
{
	int numZeros;

	if (!x) {
		return (sizeof(int) * 8);
	}

	numZeros = 0;

	while (!(x & 0x80000000)) {
		numZeros++;
		x <<= 1;
	}

	return numZeros;
}

static __inline int FASMAX( int x, int y )
{
	register int z;
	Asm("MAX %0, %1, %2" : "=r"(z): "r"(x), "r"(y));
	return z;
}

static __inline int FASMIN( int x, int y )
{
	register int z;
	Asm("MIN %0, %1, %2" : "=r"(z): "r"(x), "r"(y));
	return z;
}

static __inline void MC0M(int *SL1, const int *coef, int *vb1)
{
	Asm(
		"LD %%r0, %0\n\t"
		"LD %%r1, %1\n\t"
		"LD %%r2, %2\n\t"
		"LD %%r3, %3\n\t"
		"LD %%r4, %4\n\t"
		"LD %%r5, %5\n\t"
		"LD %%r6, %6\n\t"
		"LD %%r7, %7\n\t"
		"LD %%r8, %8\n\t"
		"LD %%r9, %9\n\t"
		"LD %%r10, %10\n\t"
		"LD %%r11, %11\n\t"
		"LD %%r12, %12\n\t"
		"LD %%r13, %13\n\t"
		"LD %%r14, %14\n\t"
		"LD %%r15, %15\n\t"

		"NEGS %%r1,  %%r1\n\t"
		"NEGS %%r3,  %%r3\n\t"
		"NEGS %%r5,  %%r5\n\t"
		"NEGS %%r7,  %%r7\n\t"
		"NEGS %%r9,  %%r9\n\t"
		"NEGS %%r11, %%r11\n\t"
		"NEGS %%r13, %%r13\n\t"
		"NEGS %%r15, %%r15"
		:
		:
		"m"(*coef), "m"(*(coef + 1)), "m"(*(coef + 2)),
		"m"(*(coef + 3)), "m"(*(coef + 4)), "m"(*(coef + 5)),
		"m"(*(coef + 6)), "m"(*(coef + 7)), "m"(*(coef + 8)),
		"m"(*(coef + 9)), "m"(*(coef + 10)), "m"(*(coef + 11)),
		"m"(*(coef + 12)), "m"(*(coef + 13)), "m"(*(coef + 14)), "m"(*(coef + 15))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);

	Asm(
		"SR 0, [0x580]\n\t"
		"SR 0, [0x582]"
		:
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);

	Asm(
		"LD %%r17, %0\n\t"
		"LD %%r18, %1\n\t"
		"LD %%r19, %2\n\t"
		"LD %%r20, %3\n\t"
		"LD %%r21, %4\n\t"
		"LD %%r22, %5\n\t"
		"LD %%r23, %6\n\t"
		"LD %%r24, %7\n\t"

		"MAC %%r16, %%r17, %%r0\n\t"
		"MAC %%r16, %%r18, %%r1\n\t"
		"MAC %%r16, %%r19, %%r2\n\t"
		"MAC %%r16, %%r20, %%r3\n\t"
		"MAC %%r16, %%r21, %%r4\n\t"
		"MAC %%r16, %%r22, %%r5\n\t"
		"MAC %%r16, %%r23, %%r6\n\t"
		"MAC %%r16, %%r24, %%r7\n\t"

		"LD %%r17, %8\n\t"
		"LD %%r18, %9\n\t"
		"LD %%r19, %10\n\t"
		"LD %%r20, %11\n\t"
		"LD %%r21, %12\n\t"
		"LD %%r22, %13\n\t"
		"LD %%r23, %14\n\t"
		"LD %%r24, %15\n\t"

		"MAC %%r16, %%r17, %%r8\n\t"
		"MAC %%r16, %%r18, %%r9\n\t"
		"MAC %%r16, %%r19, %%r10\n\t"
		"MAC %%r16, %%r20, %%r11\n\t"
		"MAC %%r16, %%r21, %%r12\n\t"
		"MAC %%r16, %%r22, %%r13\n\t"
		"MAC %%r16, %%r23, %%r14\n\t"
		"MAC %%r16, %%r24, %%r15"
		:
		:
		"m"(*(vb1 + 0)), "m"(*(vb1 + (23))), "m"(*(vb1 + 1)),
		"m"(*(vb1 + (22))), "m"(*(vb1 + 2)), "m"(*(vb1 + (21))),
		"m"(*(vb1 + 3)), "m"(*(vb1 + (20))), "m"(*(vb1 + 4)),
		"m"(*(vb1 + (19))), "m"(*(vb1 + 5)), "m"(*(vb1 + (18))),
		"m"(*(vb1 + 6)), "m"(*(vb1 + (17))), "m"(*(vb1 + 7)), "m"(*(vb1 + (16)))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24"
	);

	Asm (
		"LR %%r18, [0X582]\n\t "
		"ST %%r18, %0"
		:
		"=m" (*SL1)
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24"
	);

}

static __inline void MC1M(int *SL1, const int *coef, int *vb1)
{
	Asm(
		"LD %%r0, %0 \n\t"
		"LD %%r1, %1\n\t"
		"LD %%r2, %2\n\t"
		"LD %%r3, %3\n\t"
		"LD %%r4, %4\n\t"
		"LD %%r5, %5\n\t"
		"LD %%r6, %6\n\t"
		"LD %%r7, %7\n\t"

		"LD %%r8, %8\n\t"
		"LD %%r9, %9\n\t"
		"LD %%r10,%10\n\t"
		"LD %%r11,%11\n\t"
		"LD %%r12,%12\n\t"
		"LD %%r13,%13\n\t"
		"LD %%r14,%14\n\t"
		"LD %%r15,%15"
		:
		:
		"m"(*coef), "m"(*(coef + 1)), "m"(*(coef + 2)),
		"m"(*(coef + 3)), "m"(*(coef + 4)), "m"(*(coef + 5)),
		"m"(*(coef + 6)), "m"(*(coef + 7)), "m"(*(vb1)),
		"m"(*(vb1 + 1)), "m"(*(vb1 + 2)), "m"(*(vb1 + 3)),
		"m"(*(vb1 + 4)), "m"(*(vb1 + 5)), "m"(*(vb1 + 6)), "m"(*(vb1 + 7))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);

	Asm(
		"SR 0, [0x580]\n\t"
		"SR 0, [0x582]"
		:
		:
		: "%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);

	Asm(
		"MAC %%r16, %%r8, 	%%r0\n\t"
		"MAC %%r16, %%r9, 	%%r1\n\t"
		"MAC %%r16, %%r10, 	%%r2\n\t"
		"MAC %%r16, %%r11, 	%%r3\n\t"
		"MAC %%r16, %%r12, 	%%r4\n\t"
		"MAC %%r16, %%r13, 	%%r5\n\t"
		"MAC %%r16, %%r14,	%%r6\n\t"
		"MAC %%r16, %%r15, 	%%r7"
		:
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);

	// *SL1 = _arc_aux_read(ACC0_HI);

	Asm (
		"LR %%r17, [0X582]\n\t "
		"ST %%r17,%0"
		:
		"=m" (*SL1)
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);
}

static __inline void MC2M(int *SL1, int *SL2, const int *coef, int *vb1)
{
	Asm(
		"LD %%r0, %0\n\t"
		"LD %%r1, %1\n\t"
		"LD %%r2, %2\n\t"
		"LD %%r3, %3\n\t"
		"LD %%r4, %4\n\t"
		"LD %%r5, %5\n\t"
		"LD %%r6, %6\n\t"
		"LD %%r7, %7"
		:
		:
		"m"(*coef), "m"(*(coef + 1)), "m"(*(coef + 2)),
		"m"(*(coef + 3)), "m"(*(coef + 4)), "m"(*(coef + 5)),
		"m"(*(coef + 6)), "m"(*(coef + 7))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15"
	);

	Asm(
		"LD %%r8, 	%0\n\t"
		"LD %%r9, 	%1\n\t"
		"LD %%r10, 	%2\n\t"
		"LD %%r11, 	%3\n\t"
		"LD %%r12, 	%4\n\t"
		"LD %%r13, 	%5\n\t"
		"LD %%r14, 	%6\n\t"
		"LD %%r15, 	%7"
		:
		:
		"m"(*(coef + 8)), "m"(*(coef + 9)), "m"(*(coef + 10)),
		"m"(*(coef + 11)), "m"(*(coef + 12)), "m"(*(coef + 13)),
		"m"(*(coef + 14)), "m"(*(coef + 15))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15"
	);

	Asm(
		"LD %%r16, 	%0\n\t"
		"LD %%r17, 	%1\n\t"
		"LD %%r18, 	%2\n\t"
		"LD %%r19, 	%3\n\t"
		"LD %%r20, 	%4\n\t"
		"LD %%r21, 	%5\n\t"
		"LD %%r22, 	%6\n\t"
		"LD %%r23, 	%7"
		:
		:
		"m"(*(vb1)), "m"(*(vb1 + 1)), "m"(*(vb1 + 2)),
		"m"(*(vb1 + 3)), "m"(*(vb1 + 4)), "m"(*(vb1 + 5)),
		"m"(*(vb1 + 6)), "m"(*(vb1 + 7))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24", "%r25"
	);

	Asm(
		"SR 0, [0x580]\n\t"
		"SR 0, [0x582]\n\t"

		"MAC %%r25, %%r16, %%r1\n\t"
		"MAC %%r25, %%r17, %%r3\n\t"
		"MAC %%r25, %%r18, %%r5\n\t"
		"MAC %%r25, %%r19, %%r7\n\t"
		"MAC %%r25, %%r20, %%r9\n\t"
		"MAC %%r25, %%r21, %%r11\n\t"
		"MAC %%r25, %%r22, %%r13\n\t"
		"MAC %%r25, %%r23, %%r15\n\t"

		"LR %%r24, [0x582]"
		:
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24", "%r25"
	);

	Asm(
		"SR 0, [0x580]\n\t"
		"SR 0, [0x582]\n\t"

		"MAC %%r25, %%r16, %%r0\n\t"
		"MAC %%r25, %%r17, %%r2\n\t"
		"MAC %%r25, %%r18, %%r4\n\t"
		"MAC %%r25, %%r19, %%r6\n\t"
		"MAC %%r25, %%r20, %%r8\n\t"
		"MAC %%r25, %%r21, %%r10\n\t"
		"MAC %%r25, %%r22, %%r12\n\t"
		"MAC %%r25, %%r23, %%r14\n\t"

		"NEGS %%r1,  %%r1\n\t"
		"NEGS %%r3,  %%r3\n\t"
		"NEGS %%r5,  %%r5\n\t"
		"NEGS %%r7,  %%r7\n\t"
		"NEGS %%r9,  %%r9\n\t"
		"NEGS %%r11, %%r11\n\t"
		"NEGS %%r13, %%r13\n\t"
		"NEGS %%r15, %%r15"
		:
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24", "%r25"
	);

	Asm(
		"LD %%r16, %1\n\t"
		"LD %%r17, %2\n\t"
		"LD %%r18, %3\n\t"
		"LD %%r19, %4\n\t"
		"LD %%r20, %5\n\t"
		"LD %%r21, %6\n\t"
		"LD %%r22, %7\n\t"
		"LD %%r23, %8\n\t"

		"MAC %%r25, %%r16, %%r1\n\t"
		"MAC %%r25, %%r17, %%r3\n\t"
		"MAC %%r25, %%r18, %%r5\n\t"
		"MAC %%r25, %%r19, %%r7\n\t"
		"MAC %%r25, %%r20, %%r9\n\t"
		"MAC %%r25, %%r21, %%r11\n\t"
		"MAC %%r25, %%r22, %%r13\n\t"
		"MAC %%r25, %%r23, %%r15\n\t"

		"LR %%r25, [0x582]\n\t"
		"ST %%r25, %0"
		:
		"=m"(*SL1)
		:
		"m"(*(vb1 + 23)), "m"(*(vb1 + 22)), "m"(*(vb1 + 21)),
		"m"(*(vb1 + 20)), "m"(*(vb1 + 19)), "m"(*(vb1 + 18)),
		"m"(*(vb1 + 17)), "m"(*(vb1 + 16))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24", "%r25"
	);

	Asm(
		"SR %%r24, 	[0x582]\n\t"
		"SR 0, 		[0x580]\n\t"

		"MAC %%r25, %%r16, %%r0\n\t"
		"MAC %%r25, %%r17, %%r2\n\t"
		"MAC %%r25, %%r18, %%r4\n\t"
		"MAC %%r25, %%r19, %%r6\n\t"
		"MAC %%r25, %%r20, %%r8\n\t"
		"MAC %%r25, %%r21, %%r10\n\t"
		"MAC %%r25, %%r22, %%r12\n\t"
		"MAC %%r25, %%r23, %%r14\n\t"

		"LR %%r25, [0x582]\n\t"
		"ST %%r25, %0"
		:
		"=m"(*SL2)
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24", "%r25"
	);
}


/**
 * \brief       Core Caculate Function of MP3 Decoder,Improve efficiency by
 *              using assembly to use more register to decrease the operation
 *              times with Memory
 *
 * \param       SL1                 Pointer to Output Caculate Result 1
 *
 * \param       SR1                 Pointer to Output Caculate Result 2
 *
 * \param[in]   coef                Pointer of coef Array to Caculate
 *
 * \param[in]   vb1                 Pointer of vb1 Array to Caculate
 *
 */
static __inline void MC0S(int *SL1, int *SR1, const int *coef, int *vb1)
{

	Asm(
		"LD %%r0, %0\n\t"
		"LD %%r1, %1\n\t"
		"LD %%r2, %2\n\t"
		"LD %%r3, %3\n\t"
		"LD %%r4, %4\n\t"
		"LD %%r5, %5\n\t"
		"LD %%r6, %6\n\t"
		"LD %%r7, %7\n\t"
		"LD %%r8, %8\n\t"
		"LD %%r9, %9\n\t"
		"LD %%r10, %10\n\t"
		"LD %%r11, %11\n\t"
		"LD %%r12, %12\n\t"
		"LD %%r13, %13\n\t"
		"LD %%r14, %14\n\t"
		"LD %%r15, %15\n\t"

		"NEGS %%r1,  %%r1\n\t"
		"NEGS %%r3,  %%r3\n\t"
		"NEGS %%r5,  %%r5\n\t"
		"NEGS %%r7,  %%r7\n\t"
		"NEGS %%r9,  %%r9\n\t"
		"NEGS %%r11, %%r11\n\t"
		"NEGS %%r13, %%r13\n\t"
		"NEGS %%r15, %%r15"
		:
		:
		"m"(*coef), "m"(*(coef + 1)), "m"(*(coef + 2)),
		"m"(*(coef + 3)), "m"(*(coef + 4)), "m"(*(coef + 5)),
		"m"(*(coef + 6)), "m"(*(coef + 7)), "m"(*(coef + 8)),
		"m"(*(coef + 9)), "m"(*(coef + 10)), "m"(*(coef + 11)),
		"m"(*(coef + 12)), "m"(*(coef + 13)), "m"(*(coef + 14)), "m"(*(coef + 15))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);

	Asm(
		"SR 0, [0x580]\n\t"
		"SR 0, [0x582]"
		:
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);

	Asm(
		"LD %%r17, %0\n\t"
		"LD %%r18, %1\n\t"
		"LD %%r19, %2\n\t"
		"LD %%r20, %3\n\t"
		"LD %%r21, %4\n\t"
		"LD %%r22, %5\n\t"
		"LD %%r23, %6\n\t"
		"LD %%r24, %7\n\t"

		"MAC %%r16, %%r17, %%r0\n\t"
		"MAC %%r16, %%r18, %%r1\n\t"
		"MAC %%r16, %%r19, %%r2\n\t"
		"MAC %%r16, %%r20, %%r3\n\t"
		"MAC %%r16, %%r21, %%r4\n\t"
		"MAC %%r16, %%r22, %%r5\n\t"
		"MAC %%r16, %%r23, %%r6\n\t"
		"MAC %%r16, %%r24, %%r7\n\t"

		"LD %%r17, %8\n\t"
		"LD %%r18, %9\n\t"
		"LD %%r19, %10\n\t"
		"LD %%r20, %11\n\t"
		"LD %%r21, %12\n\t"
		"LD %%r22, %13\n\t"
		"LD %%r23, %14\n\t"
		"LD %%r24, %15\n\t"

		"MAC %%r16, %%r17, %%r8\n\t"
		"MAC %%r16, %%r18, %%r9\n\t"
		"MAC %%r16, %%r19, %%r10\n\t"
		"MAC %%r16, %%r20, %%r11\n\t"
		"MAC %%r16, %%r21, %%r12\n\t"
		"MAC %%r16, %%r22, %%r13\n\t"
		"MAC %%r16, %%r23, %%r14\n\t"
		"MAC %%r16, %%r24, %%r15"
		:
		:
		"m"(*(vb1 + 0)), "m"(*(vb1 + (23))), "m"(*(vb1 + 1)),
		"m"(*(vb1 + (22))), "m"(*(vb1 + 2)), "m"(*(vb1 + (21))),
		"m"(*(vb1 + 3)), "m"(*(vb1 + (20))), "m"(*(vb1 + 4)),
		"m"(*(vb1 + (19))), "m"(*(vb1 + 5)), "m"(*(vb1 + (18))),
		"m"(*(vb1 + 6)), "m"(*(vb1 + (17))), "m"(*(vb1 + 7)), "m"(*(vb1 + (16)))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24"
	);

	Asm (
		"LR %%r18, [0X582]\n\t "
		"ST %%r18, %0"
		:
		"=m" (*SL1)
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24"
	);

	Asm(
		"SR 0, [0x580]\n\t"
		"SR 0, [0x582]"
		:
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24"
	);

	Asm(
		"LD %%r17, %0\n\t"
		"LD %%r18, %1\n\t"
		"LD %%r19, %2\n\t"
		"LD %%r20, %3\n\t"
		"LD %%r21, %4\n\t"
		"LD %%r22, %5\n\t"
		"LD %%r23, %6\n\t"
		"LD %%r24, %7\n\t"

		"MAC %%r16, %%r17, %%r0\n\t"
		"MAC %%r16, %%r18, %%r1\n\t"
		"MAC %%r16, %%r19, %%r2\n\t"
		"MAC %%r16, %%r20, %%r3\n\t"
		"MAC %%r16, %%r21, %%r4\n\t"
		"MAC %%r16, %%r22, %%r5\n\t"
		"MAC %%r16, %%r23, %%r6\n\t"
		"MAC %%r16, %%r24, %%r7\n\t"

		"LD %%r17, %8\n\t"
		"LD %%r18, %9\n\t"
		"LD %%r19, %10\n\t"
		"LD %%r20, %11\n\t"
		"LD %%r21, %12\n\t"
		"LD %%r22, %13\n\t"
		"LD %%r23, %14\n\t"
		"LD %%r24, %15\n\t"

		"MAC %%r16, %%r17, %%r8\n\t"
		"MAC %%r16, %%r18, %%r9\n\t"
		"MAC %%r16, %%r19, %%r10\n\t"
		"MAC %%r16, %%r20, %%r11\n\t"
		"MAC %%r16, %%r21, %%r12\n\t"
		"MAC %%r16, %%r22, %%r13\n\t"
		"MAC %%r16, %%r23, %%r14\n\t"
		"MAC %%r16, %%r24, %%r15\n\t"
		:
		:
		"m"(*(vb1 + 32 + 0)), "m"(*(vb1 + 32 + (23 - 0))), "m"(*(vb1 + 32 + 1)),
		"m"(*(vb1 + 32 + (23 - 1))), "m"(*(vb1 + 32 + 2)), "m"(*(vb1 + 32 + (23 - 2))),
		"m"(*(vb1 + 32 + 3)), "m"(*(vb1 + 32 + (23 - 3))), "m"(*(vb1 + 32 + 4)),
		"m"(*(vb1 + 32 + (23 - 4))), "m"(*(vb1 + 32 + 5)), "m"(*(vb1 + 32 + (23 - 5))),
		"m"(*(vb1 + 32 + 6)), "m"(*(vb1 + 32 + (23 - 6))), "m"(*(vb1 + 32 + 7)), "m"(*(vb1 + 32 + (23 - 7)))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24"
	);

	Asm (
		"LR %%r18, [0X582]\n\t "
		"ST %%r18, %0"
		:
		"=m" (*SR1)
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);

}

/**
 * \brief       Core Caculate Function of MP3 Decoder,Improve efficiency by
 *              using assembly to use more register to decrease the operation
 *              times with Memory
 *
 * \param       SL1                 Pointer to Output Caculate Result 1
 *
 * \param       SR1                 Pointer to Output Caculate Result 2
 *
 * \param[in]   coef                Pointer of coef Array to Caculate
 *
 * \param[in]   vb1                 Pointer of vb1 Array to Caculate
 *
 */
static __inline void MC1S(int *SL1, int *SR1, const int *coef, int *vb1)
{

	Asm(
		"LD %%r0, %0 \n\t"
		"LD %%r1, %1\n\t"
		"LD %%r2, %2\n\t"
		"LD %%r3, %3\n\t"
		"LD %%r4, %4\n\t"
		"LD %%r5, %5\n\t"
		"LD %%r6, %6\n\t"
		"LD %%r7, %7\n\t"

		"LD %%r8, %8\n\t"
		"LD %%r9, %9\n\t"
		"LD %%r10,%10\n\t"
		"LD %%r11,%11\n\t"
		"LD %%r12,%12\n\t"
		"LD %%r13,%13\n\t"
		"LD %%r14,%14\n\t"
		"LD %%r15,%15"
		:
		:
		"m"(*coef), "m"(*(coef + 1)), "m"(*(coef + 2)),
		"m"(*(coef + 3)), "m"(*(coef + 4)), "m"(*(coef + 5)),
		"m"(*(coef + 6)), "m"(*(coef + 7)), "m"(*(vb1)),
		"m"(*(vb1 + 1)), "m"(*(vb1 + 2)), "m"(*(vb1 + 3)),
		"m"(*(vb1 + 4)), "m"(*(vb1 + 5)), "m"(*(vb1 + 6)), "m"(*(vb1 + 7))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);

	Asm(
		"SR 0, [0x580]\n\t"
		"SR 0, [0x582]"
		:
		:
		: "%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);

	Asm(
		"MAC %%r16, %%r8, 	%%r0\n\t"
		"MAC %%r16, %%r9, 	%%r1\n\t"
		"MAC %%r16, %%r10, 	%%r2\n\t"
		"MAC %%r16, %%r11, 	%%r3\n\t"
		"MAC %%r16, %%r12, 	%%r4\n\t"
		"MAC %%r16, %%r13, 	%%r5\n\t"
		"MAC %%r16, %%r14,	%%r6\n\t"
		"MAC %%r16, %%r15, 	%%r7"
		:
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);

	// *SL1 = _arc_aux_read(ACC0_HI);

	Asm (
		"LR %%r17, [0X582]\n\t "
		"ST %%r17,%0"
		:
		"=m" (*SL1)
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);

	Asm(
		"SR 0, [0x580]\n\t"
		"SR 0, [0x582]"
		:
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);

	Asm(
		"LD %%r8, 	%0\n\t"
		"LD %%r9, 	%1\n\t"
		"LD %%r10, 	%2\n\t"
		"LD %%r11, 	%3\n\t"
		"LD %%r12, 	%4\n\t"
		"LD %%r13, 	%5\n\t"
		"LD %%r14, 	%6\n\t"
		"LD %%r15, 	%7\n\t"

		"MAC %%r16, %%r8, 	%%r0\n\t"
		"MAC %%r16, %%r9, 	%%r1\n\t"
		"MAC %%r16, %%r10, 	%%r2\n\t"
		"MAC %%r16, %%r11, 	%%r3\n\t"
		"MAC %%r16, %%r12, 	%%r4\n\t"
		"MAC %%r16, %%r13, 	%%r5\n\t"
		"MAC %%r16, %%r14, 	%%r6\n\t"
		"MAC %%r16, %%r15, 	%%r7"
		:
		:
		"m"(*(vb1 + 32)), "m"(*(vb1 + 33)), "m"(*(vb1 + 34)),
		"m"(*(vb1 + 35)), "m"(*(vb1 + 36)), "m"(*(vb1 + 37)),
		"m"(*(vb1 + 38)), "m"(*(vb1 + 39))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);

	Asm (
		"LR %%r17, 	[0X582]\n\t "
		"ST %%r17,	%0"
		:
		"=m" (*SR1)
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7", "%r8", "%r9",
		"%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%r16", "%r17", "%r18"
	);
}

/**
 * \brief       Core Caculate Function of MP3 Decoder,Improve efficiency by
 *              using assembly to use more register to decrease the operation
 *              times with Memory
 *
 * \param       SL1                 Pointer to Output Caculate Result 1
 *
 * \param       SR1                 Pointer to Output Caculate Result 2
 *
 * \param       SL2                 Pointer to Output Caculate Result 3
 *
 * \param       SR2                 Pointer to Output Caculate Result 4
 *
 * \param[in]   coef                Pointer of coef Array to Caculate
 *
 * \param[in]   vb1                 Pointer of vb1 Array to Caculate
 *
 */
static __inline void MC2S(int *SL1, int *SR1, int *SL2, int *SR2, const int *coef, int *vb1)
{

	Asm(
		"LD %%r0, %0\n\t"
		"LD %%r1, %1\n\t"
		"LD %%r2, %2\n\t"
		"LD %%r3, %3\n\t"
		"LD %%r4, %4\n\t"
		"LD %%r5, %5\n\t"
		"LD %%r6, %6\n\t"
		"LD %%r7, %7"
		:
		:
		"m"(*coef), "m"(*(coef + 1)), "m"(*(coef + 2)),
		"m"(*(coef + 3)), "m"(*(coef + 4)), "m"(*(coef + 5)),
		"m"(*(coef + 6)), "m"(*(coef + 7))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23"
	);

	Asm(
		"LD %%r8, 	%0\n\t"
		"LD %%r9, 	%1\n\t"
		"LD %%r10, 	%2\n\t"
		"LD %%r11, 	%3\n\t"
		"LD %%r12, 	%4\n\t"
		"LD %%r13, 	%5\n\t"
		"LD %%r14, 	%6\n\t"
		"LD %%r15, 	%7"
		:
		:
		"m"(*(coef + 8)), "m"(*(coef + 9)), "m"(*(coef + 10)),
		"m"(*(coef + 11)), "m"(*(coef + 12)), "m"(*(coef + 13)),
		"m"(*(coef + 14)), "m"(*(coef + 15))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23"
	);

	Asm(
		"LD %%r16, 	%0\n\t"
		"LD %%r17, 	%1\n\t"
		"LD %%r18, 	%2\n\t"
		"LD %%r19, 	%3\n\t"
		"LD %%r20, 	%4\n\t"
		"LD %%r21, 	%5\n\t"
		"LD %%r22, 	%6\n\t"
		"LD %%r23, 	%7"
		:
		:
		"m"(*(vb1)), "m"(*(vb1 + 1)), "m"(*(vb1 + 2)),
		"m"(*(vb1 + 3)), "m"(*(vb1 + 4)), "m"(*(vb1 + 5)),
		"m"(*(vb1 + 6)), "m"(*(vb1 + 7))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24", "%r25"
	);

	Asm(
		"SR 0, [0x580]\n\t"
		"SR 0, [0x582]\n\t"

		"MAC %%r25, %%r16, %%r1\n\t"
		"MAC %%r25, %%r17, %%r3\n\t"
		"MAC %%r25, %%r18, %%r5\n\t"
		"MAC %%r25, %%r19, %%r7\n\t"
		"MAC %%r25, %%r20, %%r9\n\t"
		"MAC %%r25, %%r21, %%r11\n\t"
		"MAC %%r25, %%r22, %%r13\n\t"
		"MAC %%r25, %%r23, %%r15\n\t"

		"LR %%r24, [0x582]"
		:
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24", "%r25"
	);

	Asm(
		"SR 0, [0x580]\n\t"
		"SR 0, [0x582]\n\t"

		"MAC %%r25, %%r16, %%r0\n\t"
		"MAC %%r25, %%r17, %%r2\n\t"
		"MAC %%r25, %%r18, %%r4\n\t"
		"MAC %%r25, %%r19, %%r6\n\t"
		"MAC %%r25, %%r20, %%r8\n\t"
		"MAC %%r25, %%r21, %%r10\n\t"
		"MAC %%r25, %%r22, %%r12\n\t"
		"MAC %%r25, %%r23, %%r14\n\t"

		"NEGS %%r1,  %%r1\n\t"
		"NEGS %%r3,  %%r3\n\t"
		"NEGS %%r5,  %%r5\n\t"
		"NEGS %%r7,  %%r7\n\t"
		"NEGS %%r9,  %%r9\n\t"
		"NEGS %%r11, %%r11\n\t"
		"NEGS %%r13, %%r13\n\t"
		"NEGS %%r15, %%r15"
		:
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24", "%r25"
	);

	Asm(
		"LD %%r16, %1\n\t"
		"LD %%r17, %2\n\t"
		"LD %%r18, %3\n\t"
		"LD %%r19, %4\n\t"
		"LD %%r20, %5\n\t"
		"LD %%r21, %6\n\t"
		"LD %%r22, %7\n\t"
		"LD %%r23, %8\n\t"

		"MAC %%r25, %%r16, %%r1\n\t"
		"MAC %%r25, %%r17, %%r3\n\t"
		"MAC %%r25, %%r18, %%r5\n\t"
		"MAC %%r25, %%r19, %%r7\n\t"
		"MAC %%r25, %%r20, %%r9\n\t"
		"MAC %%r25, %%r21, %%r11\n\t"
		"MAC %%r25, %%r22, %%r13\n\t"
		"MAC %%r25, %%r23, %%r15\n\t"

		"LR %%r25, [0x582]\n\t"
		"ST %%r25, %0"
		:
		"=m"(*SL1)
		:
		"m"(*(vb1 + 23)), "m"(*(vb1 + 22)), "m"(*(vb1 + 21)),
		"m"(*(vb1 + 20)), "m"(*(vb1 + 19)), "m"(*(vb1 + 18)),
		"m"(*(vb1 + 17)), "m"(*(vb1 + 16))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24", "%r25"
	);

	Asm(
		"SR %%r24, 	[0x582]\n\t"
		"SR 0, 		[0x580]\n\t"

		"MAC %%r25, %%r16, %%r0\n\t"
		"MAC %%r25, %%r17, %%r2\n\t"
		"MAC %%r25, %%r18, %%r4\n\t"
		"MAC %%r25, %%r19, %%r6\n\t"
		"MAC %%r25, %%r20, %%r8\n\t"
		"MAC %%r25, %%r21, %%r10\n\t"
		"MAC %%r25, %%r22, %%r12\n\t"
		"MAC %%r25, %%r23, %%r14\n\t"

		"LR %%r25, [0x582]\n\t"
		"ST %%r25, %0"
		:
		"=m"(*SL2)
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24", "%r25"
	);

	Asm(
		"SR 0, [0x582]\n\t"
		"SR 0, [0x580]\n\t"

		"LD %%r16, %0\n\t"
		"LD %%r17, %1\n\t"
		"LD %%r18, %2\n\t"
		"LD %%r19, %3\n\t"
		"LD %%r20, %4\n\t"
		"LD %%r21, %5\n\t"
		"LD %%r22, %6\n\t"
		"LD %%r23, %7\n\t"

		"MAC %%r25, %%r16, %%r1\n\t"
		"MAC %%r25, %%r17, %%r3\n\t"
		"MAC %%r25, %%r18, %%r5\n\t"
		"MAC %%r25, %%r19, %%r7\n\t"
		"MAC %%r25, %%r20, %%r9\n\t"
		"MAC %%r25, %%r21, %%r11\n\t"
		"MAC %%r25, %%r22, %%r13\n\t"
		"MAC %%r25, %%r23, %%r15\n\t"

		"LR %%r24, [0x582]"
		:
		:
		"m"(*(vb1 + 55)), "m"(*(vb1 + 54)), "m"(*(vb1 + 53)),
		"m"(*(vb1 + 52)), "m"(*(vb1 + 51)), "m"(*(vb1 + 50)),
		"m"(*(vb1 + 49)), "m"(*(vb1 + 48))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24", "%r25"
	);

	Asm(
		"SR 0, [0x582]\n\t"
		"SR 0, [0x580]\n\t"

		"MAC %%r25, %%r16, %%r0\n\t"
		"MAC %%r25, %%r17, %%r2\n\t"
		"MAC %%r25, %%r18, %%r4\n\t"
		"MAC %%r25, %%r19, %%r6\n\t"
		"MAC %%r25, %%r20, %%r8\n\t"
		"MAC %%r25, %%r21, %%r10\n\t"
		"MAC %%r25, %%r22, %%r12\n\t"
		"MAC %%r25, %%r23, %%r14\n\t"

		"NEGS %%r1,  %%r1\n\t"
		"NEGS %%r3,  %%r3\n\t"
		"NEGS %%r5,  %%r5\n\t"
		"NEGS %%r7,  %%r7\n\t"
		"NEGS %%r9,  %%r9\n\t"
		"NEGS %%r11, %%r11\n\t"
		"NEGS %%r13, %%r13\n\t"
		"NEGS %%r15, %%r15"
		:
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24", "%r25"
	);

	Asm(
		"LD %%r16, %1\n\t"
		"LD %%r17, %2\n\t"
		"LD %%r18, %3\n\t"
		"LD %%r19, %4\n\t"
		"LD %%r20, %5\n\t"
		"LD %%r21, %6\n\t"
		"LD %%r22, %7\n\t"
		"LD %%r23, %8\n\t"

		"MAC %%r25, %%r16, %%r1\n\t"
		"MAC %%r25, %%r17, %%r3\n\t"
		"MAC %%r25, %%r18, %%r5\n\t"
		"MAC %%r25, %%r19, %%r7\n\t"
		"MAC %%r25, %%r20, %%r9\n\t"
		"MAC %%r25, %%r21, %%r11\n\t"
		"MAC %%r25, %%r22, %%r13\n\t"
		"MAC %%r25, %%r23, %%r15\n\t"

		"LR %%r25,[0x582]\n\t"
		"ST %%r25, %0"
		:
		"=m"(*SR2)
		:
		"m"(*(vb1 + 32)), "m"(*(vb1 + 33)), "m"(*(vb1 + 34)),
		"m"(*(vb1 + 35)), "m"(*(vb1 + 36)), "m"(*(vb1 + 37)),
		"m"(*(vb1 + 38)), "m"(*(vb1 + 39))
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24", "%r25"
	);

	Asm(
		"SR %%r24,	[0x582]\n\t"
		"SR 0,		[0x580]\n\t"

		"MAC %%r25, %%r16, %%r0\n\t"
		"MAC %%r25, %%r17, %%r2\n\t"
		"MAC %%r25, %%r18, %%r4\n\t"
		"MAC %%r25, %%r19, %%r6\n\t"
		"MAC %%r25, %%r20, %%r8\n\t"
		"MAC %%r25, %%r21, %%r10\n\t"
		"MAC %%r25, %%r22, %%r12\n\t"
		"MAC %%r25, %%r23, %%r14\n\t"

		"LR %%r25,[0x582]\n\t"
		"ST %%r25, %0"
		:
		"=m"(*SR1)
		:
		:
		"%r0", "%r1", "%r2", "%r3", "%r4", "%r5", "%r6", "%r7",
		"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%r16", "%r17", "%r18", "%r19", "%r20", "%r21", "%r22", "%r23", "%r24", "%r25"
	);

}




#endif /* _ASSEMBLY_H */
