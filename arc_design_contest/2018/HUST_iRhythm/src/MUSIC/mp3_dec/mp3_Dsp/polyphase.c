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
 * polyphase.c - final stage of subband transform (polyphase synthesis filter)
 *
 * This is the C reference version using __int64
 * Look in the appropriate subdirectories for optimized asm implementations
 *   (e.g. arm/asmpoly.s)
 **************************************************************************************/
#include "embARC.h"
#include "embARC_debug.h"

#include "coder.h"
#include "Dsp_assembly.h"

/* input to Polyphase = Q(DQ_FRACBITS_OUT-2), gain 2 bits in convolution
 *  we also have the implicit bias of 2^15 to add back, so net fraction bits =
 *    DQ_FRACBITS_OUT - 2 - 2 - 15
 *  (see comment on Dequantize() for more info)
 */
#define DEF_NFRACBITS	(DQ_FRACBITS_OUT - 2 - 2 - 15)
#define CSHIFT	12	/* coefficients have 12 leading sign bits for early-terminating mulitplies */

#define CHECK_BIT 2

void PolyphaseMono(char *pcm, int *vbuf, const int *coefBase)
{
	int i;
	const int *coef;
	int *vb1;
	int sum1L, sum2L;

	/* special case, output sample 0 */
	coef = coefBase;
	vb1 = vbuf;

	MC0M(&sum1L, coef, vb1);

	*(pcm    ) = (char)(SAR32(sum1L, CHECK_BIT));
	*(pcm + 1) = (char)(SAR32(sum1L, CHECK_BIT));

	/* special case, output sample 16 */
	coef = coefBase + 256;
	vb1 = vbuf + 1024;


	MC1M(&sum1L, coef, vb1);

	*( pcm + 32 ) = (char)(SAR32(sum1L, CHECK_BIT));
	*( pcm + 33 ) = (char)(SAR32(sum1L, CHECK_BIT));

	/* main convolution loop: sum1L = samples 1, 2, 3, ... 15   sum2L = samples 31, 30, ... 17 */
	coef = coefBase + 16;
	vb1 = vbuf + 64;
	pcm += 2;

	/* right now, the compiler creates bad asm from this... */
	for (i = 15; i > 0; i--) {

		MC2M(&sum1L, &sum2L, coef, vb1);
		coef += 16;
		vb1 += 64;

		*( pcm ) =  (char)(SAR32(sum1L, CHECK_BIT));
		*( pcm + 1 ) = (char)(SAR32(sum1L, CHECK_BIT));
		*( pcm + 4 * i) = (char)(SAR32(sum2L, CHECK_BIT));
		*( pcm + 4 * i + 1 ) = (char)(SAR32(sum2L, CHECK_BIT));
		pcm += 2;
	}

}



/**************************************************************************************
 * Function:    PolyphaseStereo
 *
 * Description: filter one subband and produce 32 output PCM samples for each channel
 *
 * Inputs:      pointer to PCM output buffer
 *              number of "extra shifts" (vbuf format = Q(DQ_FRACBITS_OUT-2))
 *              pointer to start of vbuf (preserved from last call)
 *              start of filter coefficient table (in proper, shuffled order)
 *              no minimum number of guard bits is required for input vbuf
 *                (see additional scaling comments below)
 *
 * Outputs:     32 samples of two channels of decoded PCM data, (i.e. Q16.0)
 *
 * Return:      none
 *
 * Notes:       interleaves PCM samples LRLRLR...
 *
 * TODO:        add 32-bit version for platforms where 64-bit mul-acc is not supported
 **************************************************************************************/
void PolyphaseStereo(char *pcm, int *vbuf, const int *coefBase)
{
	int i;
	const int *coef;
	int *vb1;
	int sum1L, sum2L, sum1R, sum2R;

	/*                   Part 1                    */
	/* special case, output sample 0 */
	coef = coefBase;
	vb1 = vbuf;
	MC0S(&sum1L, &sum1R, coef, vb1);

	*(pcm ) = (char)(SAR32(sum1L, CHECK_BIT));
	*(pcm + 1) = (char)(SAR32(sum1R, CHECK_BIT));

	/*                   Part 2                    */
	/* special case, output sample 16 */
	coef = coefBase + 256;
	vb1 = vbuf + 1024;

	MC1S(&sum1L, &sum1R, coef, vb1);

	*(pcm + 32) = (char)(SAR32(sum1L, CHECK_BIT));
	*(pcm + 33) = (char)(SAR32(sum1R, CHECK_BIT));

	/*                   Part 3                     */

	/* main convolution loop: sum1L = samples 1, 2, 3, ... 15   sum2L = samples 31, 30, ... 17 */
	coef = coefBase + 16;
	vb1 = vbuf + 64;
	pcm += 2;

	/* right now, the compiler creates bad asm from this... */
	for (i = 15; i > 0; i--) {
		MC2S(&sum1L, &sum1R, &sum2L, &sum2R, coef, vb1);
		coef += 16;
		vb1 += 64;
		*(pcm )				=		(char)(SAR32(sum1L, CHECK_BIT));
		*(pcm + 1)			=		(char)(SAR32(sum1R, CHECK_BIT));
		*(pcm + 4 * i)		=		(char)(SAR32(sum2L, CHECK_BIT));
		*(pcm + 4 * i + 1)	=		(char)(SAR32(sum2R, CHECK_BIT));
		pcm += 2;
	}
}
