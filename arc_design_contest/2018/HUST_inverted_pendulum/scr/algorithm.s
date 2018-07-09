	.file "algorithm.s"

/* entry for lqr algorithm */
	.text
	.global kalmanPredict, kalmanCorrect, accl2AngleIndex, lqrCalc, pidTick
	.align 4

kalmanPredict:
	mpy 	r1,r1,0x42		;B*u
	ld_s	r3,[r0,0x14]	;xpos
	ld_s	r2,[r0,0x18]	;Ppos
	ld		r4,[r0]			;Q
	add_s	r2,r2,r4		;Q+Ppos
	st		r2,[r0,0x10]	;Ppri = Q+Ppos
	asrsr	r1,r1,0x0F 		;RND32((B*u)>>15)
	add_s	r1,r3,r1		;xpos + RND32((B*u)>>15)
	j_s.d	[blink]
	st		r1,[r0,0xc]     ;xpri = xpos + RND32((B*u)>>15)
	nop_s	

kalmanCorrect:
	ld		r5,[r0,16]		;Ppri
	ld		r4,[r0,4]		;R
	ld		r6,[r0,12]		;xpri
	asl		r2,r5,0x0F  	;Ppri << 15
	add 	r4,r4,r5		;Ppri + R
	asrsr 	r3,r4,0x1 		;(Ppri + R) / 2
	add_s 	r2,r2,r3 		;(Ppri << 15) + (Ppri + R) / 2
	div		r4,r2,r4		;((Ppri << 15) + (Ppri + R) / 2) / (Ppri + R)
	rsub	r3,r4,0x1000 	;I - k
	subs	r1,r1,r6		;z - xpri
	mpy		r3,r3,r5		;(I - k) * Ppri
	st		r4,[r0,8]		;k = ((Ppri << 15) + (Ppri + R) / 2) / (Ppri + R)
	mpy 	r1,r1,r4 		;(z - xpri) * k
 	asrsr	r3,r3,0x0F 		;RND32((I - k) * Ppri>>15)
	st		r3,[r0,0x18] 	;Ppos = RND32((I - k) * Ppri>>15)
	asrsr	r1,r1,0x0F 		;RND32((z - xpri) * k>>15)
	add 	r1,r1,r6 		;xpri + RND32((z - xpri) * k>>15)
	st		r1,[r0,0x14] 	;xpos = xpri + RND32((z - xpri) * k>>15)
	j_s.d	[blink]
	mov_s	r0,r1
	nop_s	

accl2AngleIndex:
	ldh.x 	r6,[r1]			;AcclX
	ldh.x	r7,[r2] 		;AcclZ
	ldh 	r4,[r0,0] 		;imuVals.acclX
	ldh 	r5,[r0,4]		;imuVals.acclZ
	ldh 	r0,[r3] 		;AcclLPFParam
	rsub 	r8,r0,128		;128 - AcclLPFParam
	mpyw 	r6,r8,r6		;AcclX * (128 - AcclLPFParam)
	mpyw 	r4,r0,r4		;imuVals.acclX * AcclLPFParam
	sub 	r6,r6,r4 		;AcclX * (128 - AcclLPFParam) - imuVals.acclX * AcclLPFParam
	asrsr 	r6,r6,7			;RND32(AcclX * (128 - AcclLPFParam) - imuVals.acclX * AcclLPFParam) >> 7
	sth		r6,[r1]			;AcclX = RND32(AcclX * (128 - AcclLPFParam) - imuVals.acclX * AcclLPFParam) >> 7;
	mpyw 	r7,r8,r7 		;AcclZ * (128 - AcclLPFParam)
	mpyw 	r5,r0,r5		;imuVals.acclZ * AcclLPFParam
	sub 	r7,r7,r5 		;AcclZ * (128 - AcclLPFParam) - imuVals.acclZ * AcclLPFParam
	asrsr	r7,r7,7 		;RND32(AcclZ * (128 - AcclLPFParam) - imuVals.acclZ * AcclLPFParam) >> 7
	sth		r7,[r2]			;AcclZ = RND32(AcclZ * (128 - AcclLPFParam) - imuVals.acclZ * AcclLPFParam) >> 7;
	asls	r6,r6,7 		;AcclX * 128
	div		r0,r6,r7 		;AcclX * 128 / AcclZ
	adds 	r0,r0,192	 	;index = AcclX * 128 / AcclZ + 192
	asls 	r0,r0,7			;index << 7 (0~255 -> 0~32768)
	abssh 	r0,r0			;SAT16(abs(index)), keep abs(index)<255 

	j_s.d	[blink]
	asrsr 	r0,r0,7
	nop_s

lqrCalc:
	ld 		r5,[r0]
	ld		r7,[r0,4]
	ld		r6,[r0,8]
	ld_s	r0,[r0,0xC]
	mpy 	r1,r5,r1
	mpy		r2,r7,r2
	mpy 	r3,r6,r3
	add_s	r1,r1,r2
	mpy		r0,r0,r4
	add_s	r1,r1,r3
	j_s.d	[blink]
	add_s	r0,r0,r1
	nop_s

pidTick:
	ld_s 	r3,[r0,0]		;p
	ld 		r4,[r0,4]		;i
	ld 		r5,[r0,8]		;d
	ld 		r6,[r0,0x0c]	;n
	ld 		r7,[r0,0x10]	;accI
	ld 		r8,[r0,0x14]	;accD
	mpy_s 	r3,r3,r1		;p*diff
	asrsr 	r3,r3,8 		;pout=p*diff>>8
	mpy 	r5,r5,r1		;d*diff
	asrsr 	r5,r5,8 		;d*diff>>8
	sub 	r5,r5,r8 		;d*diff>>8 - accD
	mpy 	r5,r5,r6		;(d*diff>>8 - accD)*n
	asrsr 	r5,r5,8 		;dout=(d*diff>>8 - accD)*n >> 8
	mpymu	r2,r5,0x10624dd3
	asr 	r5,r5,0x1f
	and		r6,r5,0x10624dd3
	sub 	r2,r2,r6
	asr_s	r2,r2,0x5
	sub 	r6,r2,r6
	# div		r6,r5,500 		;dout*Ts
	add 	r8,r6,r8		;accD+=dout*Ts
	mpy 	r4,r4,r1		;i*diff
	asls 	r8,r8,10		;SAT32(accD<<10), keep abs(accD)<65536
	mpymu	r2,r4,0x10624dd3
	asrsr 	r8,r8,10		;accD >> 10
	asr 	r4,r4,0x1f
	and		r1,r4,0x10624dd3
	sub 	r2,r2,r1
	asr_s	r2,r2,0x5
	sub 	r4,r2,r4
	# div 	r4,r4,500		;i*diff*Ts 
	st 		r8,[r0,0x14]
	asrsr 	r4,r4,8 		;i*diff*Ts>>8
	add 	r7,r7,r4 		;accI+=i*diff*Ts>>8
	add 	r3,r3,r8		;pout+=acclD
	asls 	r7,r7,10		;SAT32(accI<<10), keep abs(accI)<65536 
	asrsr 	r7,r7,10		;accI >> 10
	st 		r7,[r0,0x10]
	add 	r3,r3,r7		;pout+=accI
	asls 	r0,r3,10		;SAT32(pout<<10), keep abs(pout)<65536
	j_s.d	[blink]
	asrsr 	r0,r0,15		;pout >> 15
	nop_s


	
