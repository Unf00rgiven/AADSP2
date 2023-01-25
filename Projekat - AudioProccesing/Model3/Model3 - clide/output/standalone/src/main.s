	.public _LPFenable
	.public _Mode
	.public _filterCoeff
	.public _gained
	.public _historyC
	.public _historyL
	.public _historyLS
	.public _historyR
	.public _historyRS
	.public _out
	.public _pStateC
	.public _pStateL
	.public _pStateLS
	.public _pStateR
	.public _pStateRS
	.public _sampleBuffer
	.public _variablesGain
	.public _variablesGainC
	.public _variablesGainL
	.public _variablesGainLS
	.public _variablesGainR
	.public _variablesGainRS
	.public _FilterInit
	.extern _Processing
	.extern _cl_wavread_bits_per_sample
	.extern _cl_wavread_close
	.extern _cl_wavread_frame_rate
	.extern _cl_wavread_getnchannels
	.extern _cl_wavread_number_of_frames
	.extern _cl_wavread_open
	.extern _cl_wavread_recvsample
	.extern _cl_wavwrite_close
	.extern _cl_wavwrite_open
	.extern _cl_wavwrite_sendsample
	.public _main
	.extern _printf
	.public _saturation
	.extern _strcpy
	.extern __div
	.xdata_ovly
_LPFenable
	.bss (0x1)
	.xdata_ovly
_Mode
	.bss (0x1)
	.xdata_ovly
__extractedConst_0_1
	.dw  (0x7fdf3b64)
	.xdata_ovly
__extractedConst_1_1
	.dw  (0x8020c49c)
	.xdata_ovly
_filterCoeff
	.dw  (0xfff56661)
	.dw  (0x141de0)
	.dw  (0xffdfc457)
	.dw  (0x2c8ff9)
	.dw  (0xffcc8508)
	.dw  (0x2a6c3a)
	.dw  (0xffffe9bb)
	.dw  (0xff9b152f)
	.dw  (0x1260112)
	.dw  (0xfd9654d8)
	.dw  (0x453b401)
	.dw  (0xf909acd0)
	.dw  (0xa3a3076)
	.dw  (0xf243872f)
	.dw  (0x10a4f166)
	.dw  (0x6e37aef9)
	.dw  (0x10a4f166)
	.dw  (0xf243872f)
	.dw  (0xa3a3076)
	.dw  (0xf909acd0)
	.dw  (0x453b401)
	.dw  (0xfd9654d8)
	.dw  (0x1260112)
	.dw  (0xff9b152f)
	.dw  (0xffffe9bb)
	.dw  (0x2a6c3a)
	.dw  (0xffcc8508)
	.dw  (0x2c8ff9)
	.dw  (0xffdfc457)
	.dw  (0x141de0)
	.dw  (0xfff56661)
	.xdata_ovly
_gained
	.bss (0x6)
	.xdata_ovly
_historyC
	.bss (0x1f)
	.xdata_ovly
_historyL
	.bss (0x1f)
	.xdata_ovly
_historyLS
	.bss (0x1f)
	.xdata_ovly
_historyR
	.bss (0x1f)
	.xdata_ovly
_historyRS
	.bss (0x1f)
	.xdata_ovly
_out
	.bss (0x5)
	.xdata_ovly
_pStateC
	.dw _historyC
	.xdata_ovly
_pStateL
	.dw _historyL
	.xdata_ovly
_pStateLS
	.dw _historyLS
	.xdata_ovly
_pStateR
	.dw _historyR
	.xdata_ovly
_pStateRS
	.dw _historyRS
	.ydata_ovly
_sampleBuffer
	.bss (0x80)
	.xdata_ovly
_string_const_0
	.dw  (0x45)
	.dw  (0x72)
	.dw  (0x72)
	.dw  (0x6f)
	.dw  (0x72)
	.dw  (0x3a)
	.dw  (0x20)
	.dw  (0x43)
	.dw  (0x6f)
	.dw  (0x75)
	.dw  (0x6c)
	.dw  (0x64)
	.dw  (0x20)
	.dw  (0x6e)
	.dw  (0x6f)
	.dw  (0x74)
	.dw  (0x20)
	.dw  (0x6f)
	.dw  (0x70)
	.dw  (0x65)
	.dw  (0x6e)
	.dw  (0x20)
	.dw  (0x77)
	.dw  (0x61)
	.dw  (0x76)
	.dw  (0x65)
	.dw  (0x66)
	.dw  (0x69)
	.dw  (0x6c)
	.dw  (0x65)
	.dw  (0x2e)
	.dw  (0xa)
	.dw  (0x0)
	.xdata_ovly
_string_const_1
	.dw  (0x45)
	.dw  (0x72)
	.dw  (0x72)
	.dw  (0x6f)
	.dw  (0x72)
	.dw  (0x3a)
	.dw  (0x20)
	.dw  (0x43)
	.dw  (0x6f)
	.dw  (0x75)
	.dw  (0x6c)
	.dw  (0x64)
	.dw  (0x20)
	.dw  (0x6e)
	.dw  (0x6f)
	.dw  (0x74)
	.dw  (0x20)
	.dw  (0x6f)
	.dw  (0x70)
	.dw  (0x65)
	.dw  (0x6e)
	.dw  (0x20)
	.dw  (0x77)
	.dw  (0x61)
	.dw  (0x76)
	.dw  (0x65)
	.dw  (0x66)
	.dw  (0x69)
	.dw  (0x6c)
	.dw  (0x65)
	.dw  (0x2e)
	.dw  (0xa)
	.dw  (0x0)
	.xdata_ovly
_string_const_2
	.dw  (0x45)
	.dw  (0x72)
	.dw  (0x72)
	.dw  (0x6f)
	.dw  (0x72)
	.dw  (0x3a)
	.dw  (0x20)
	.dw  (0x43)
	.dw  (0x6f)
	.dw  (0x75)
	.dw  (0x6c)
	.dw  (0x64)
	.dw  (0x20)
	.dw  (0x6e)
	.dw  (0x6f)
	.dw  (0x74)
	.dw  (0x20)
	.dw  (0x6f)
	.dw  (0x70)
	.dw  (0x65)
	.dw  (0x6e)
	.dw  (0x20)
	.dw  (0x77)
	.dw  (0x61)
	.dw  (0x76)
	.dw  (0x65)
	.dw  (0x66)
	.dw  (0x69)
	.dw  (0x6c)
	.dw  (0x65)
	.dw  (0x2e)
	.dw  (0xa)
	.dw  (0x0)
	.xdata_ovly
_variablesGain
	.dw  (0x50c335d4)
	.dw  (0xcaddc7b)
	.dw  (0x50c335d4)
	.dw  (0xcaddc7b)
	.dw  (0x50c335d4)
	.dw  (0xcaddc7b)
	.dw  (0x50c335d4)
	.dw  (0xcaddc7b)
	.dw  (0x50c335d4)
	.dw  (0xcaddc7b)
	.xdata_ovly
_variablesGainC
	.dw  (0x50c335d4)
	.dw  (0xcaddc7b)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.xdata_ovly
_variablesGainL
	.dw  (0x50c335d4)
	.dw  (0xcaddc7b)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.xdata_ovly
_variablesGainLS
	.dw  (0x50c335d4)
	.dw  (0xcaddc7b)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.xdata_ovly
_variablesGainR
	.dw  (0x50c335d4)
	.dw  (0xcaddc7b)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.xdata_ovly
_variablesGainRS
	.dw  (0x50c335d4)
	.dw  (0xcaddc7b)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.code_ovly



_FilterInit:			/* LN: 75 | CYCLE: 0 | RULES: () */ 
	xmem[i7] = i7			# LN: 75 | 
	i7 += 1			# LN: 75 | 
	i7 = i7 + (0x6)			# LN: 75 | 
cline_75_0:			/* LN: 78 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x1)			# LN: 78 | 
	uhalfword(a0) = (_historyL + 0)			# LN: 78 | 
	xmem[i0] = a0			# LN: 78 | 
cline_78_0:			/* LN: 79 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x2)			# LN: 79 | 
	uhalfword(a0) = (_historyR + 0)			# LN: 79 | 
	xmem[i0] = a0			# LN: 79 | 
cline_79_0:			/* LN: 80 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x3)			# LN: 80 | 
	uhalfword(a0) = (_historyLS + 0)			# LN: 80 | 
	xmem[i0] = a0			# LN: 80 | 
cline_80_0:			/* LN: 81 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x4)			# LN: 81 | 
	uhalfword(a0) = (_historyRS + 0)			# LN: 81 | 
	xmem[i0] = a0			# LN: 81 | 
cline_81_0:			/* LN: 82 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x5)			# LN: 82 | 
	uhalfword(a0) = (_historyC + 0)			# LN: 82 | 
	xmem[i0] = a0			# LN: 82 | 
cline_82_0:			/* LN: 84 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x6)			# LN: 84 | 
	a0 = 0			# LN: 84 | 
	xmem[i0] = a0h			# LN: 84 | 
	do (0x1f), label_end_92			# LN: 84 | 
cline_84_0:			/* LN: 86 | CYCLE: 0 | RULES: () */ 
label_begin_92:			/* LN: 84 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x1)			# LN: 86 | 
	i0 = xmem[i0]			# LN: 86 | 
	a0 = 0			# LN: 86 | 
	xmem[i0] = a0h			# LN: 86 | 
cline_86_0:			/* LN: 87 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x2)			# LN: 87 | 
	i0 = xmem[i0]			# LN: 87 | 
	a0 = 0			# LN: 87 | 
	xmem[i0] = a0h			# LN: 87 | 
cline_87_0:			/* LN: 88 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x3)			# LN: 88 | 
	i0 = xmem[i0]			# LN: 88 | 
	a0 = 0			# LN: 88 | 
	xmem[i0] = a0h			# LN: 88 | 
cline_88_0:			/* LN: 89 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x4)			# LN: 89 | 
	i0 = xmem[i0]			# LN: 89 | 
	a0 = 0			# LN: 89 | 
	xmem[i0] = a0h			# LN: 89 | 
cline_89_0:			/* LN: 90 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x5)			# LN: 90 | 
	i0 = xmem[i0]			# LN: 90 | 
	a0 = 0			# LN: 90 | 
	xmem[i0] = a0h			# LN: 90 | 
cline_90_0:			/* LN: 92 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x1)			# LN: 92 | 
	i0 = xmem[i0]			# LN: 92 | 
	i1 = i7 - (0x1)			# LN: 92 | 
	i0 += 1			# LN: 92 | 
	xmem[i1] = i0			# LN: 92 | 
cline_92_0:			/* LN: 93 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x2)			# LN: 93 | 
	i0 = xmem[i0]			# LN: 93 | 
	i1 = i7 - (0x2)			# LN: 93 | 
	i0 += 1			# LN: 93 | 
	xmem[i1] = i0			# LN: 93 | 
cline_93_0:			/* LN: 94 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x3)			# LN: 94 | 
	i0 = xmem[i0]			# LN: 94 | 
	i1 = i7 - (0x3)			# LN: 94 | 
	i0 += 1			# LN: 94 | 
	xmem[i1] = i0			# LN: 94 | 
cline_94_0:			/* LN: 95 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x4)			# LN: 95 | 
	i0 = xmem[i0]			# LN: 95 | 
	i1 = i7 - (0x4)			# LN: 95 | 
	i0 += 1			# LN: 95 | 
	xmem[i1] = i0			# LN: 95 | 
cline_95_0:			/* LN: 96 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x5)			# LN: 96 | 
	i0 = xmem[i0]			# LN: 96 | 
	i1 = i7 - (0x5)			# LN: 96 | 
	i0 += 1			# LN: 96 | 
	xmem[i1] = i0			# LN: 96 | 
cline_96_0:			/* LN: 84 | CYCLE: 0 | RULES: () */ 
init_latch_label_0:			/* LN: 97 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x6)			# LN: 84 | 
	a0 = xmem[i0]			# LN: 84 | 
	uhalfword(a1) = (0x1)			# LN: 84 | 
	a0 = a0 + a1			# LN: 84 | 
	i0 = i7 - (0x6)			# LN: 84 | 
label_end_92:			# LN: 84 | CYCLE: 5 | RULES: ()
	xmem[i0] = a0h			# LN: 84 | 
cline_84_1:			/* LN: 98 | CYCLE: 0 | RULES: () */ 
for_end_0:			/* LN: 84 | CYCLE: 0 | RULES: () */ 
	i0 = (0) + (_pStateL)			# LN: 98 | 
	i0 = xmem[i0]			# LN: 98 | 
	a0 = 0			# LN: 98 | 
	xmem[i0] = a0h			# LN: 98 | 
cline_98_0:			/* LN: 99 | CYCLE: 0 | RULES: () */ 
	i0 = (0) + (_pStateR)			# LN: 99 | 
	i0 = xmem[i0]			# LN: 99 | 
	a0 = 0			# LN: 99 | 
	xmem[i0] = a0h			# LN: 99 | 
cline_99_0:			/* LN: 100 | CYCLE: 0 | RULES: () */ 
	i0 = (0) + (_pStateLS)			# LN: 100 | 
	i0 = xmem[i0]			# LN: 100 | 
	a0 = 0			# LN: 100 | 
	xmem[i0] = a0h			# LN: 100 | 
cline_100_0:			/* LN: 101 | CYCLE: 0 | RULES: () */ 
	i0 = (0) + (_pStateRS)			# LN: 101 | 
	i0 = xmem[i0]			# LN: 101 | 
	a0 = 0			# LN: 101 | 
	xmem[i0] = a0h			# LN: 101 | 
cline_101_0:			/* LN: 102 | CYCLE: 0 | RULES: () */ 
	i0 = (0) + (_pStateC)			# LN: 102 | 
	i0 = xmem[i0]			# LN: 102 | 
	a0 = 0			# LN: 102 | 
	xmem[i0] = a0h			# LN: 102 | 
cline_102_0:			/* LN: 103 | CYCLE: 0 | RULES: () */ 
	jmp (__epilogue_236)			# LN: 103 | 
__epilogue_236:			/* LN: 103 | CYCLE: 0 | RULES: () */ 
	i7 = i7 - (0x6)			# LN: 103 | 
	i7 -= 1			# LN: 103 | 
	ret			# LN: 103 | 



	# This construction should ensure linking of crt0 in case when target is a standalone program without the OS
	.if defined(_OVLY_)
		.if .strcmp('standalone',_OVLY_)=0
		.if .strcmp('crystal32',_TARGET_FAMILY_)=0
			.extern __start         # dummy use of __start to force linkage of crt0
dummy		.equ(__start)
		.else
			.extern __intvec         # dummy use of __intvec to force linkage of intvec
dummy		.equ(__intvec)
		.endif
		.endif
	.endif

_main:			/* LN: 269 | CYCLE: 0 | RULES: () */ 
	xmem[i7] = i7			# LN: 269 | 
	i7 += 1			# LN: 269 | 
	i7 = i7 + (0x20f)			# LN: 269 | 
	i1 = i7 - (0x1)			# LN: 269 | 
	xmem[i1] = a0h			# LN: 269 | 
	i1 = i7 - (0x2)			# LN: 269 | 
	xmem[i1] = i0			# LN: 269 | 
cline_269_0:			/* LN: 287 | CYCLE: 0 | RULES: () */ 
	uhalfword(a0) = (0x1)			# LN: 287 | 
	xmem[_LPFenable + 0] = a0h			# LN: 287 | 
cline_287_0:			/* LN: 289 | CYCLE: 0 | RULES: () */ 
	uhalfword(a0) = (0x1)			# LN: 289 | 
	xmem[_Mode + 0] = a0h			# LN: 289 | 
cline_289_0:			/* LN: 292 | CYCLE: 0 | RULES: () */ 
	call (_FilterInit)			# LN: 292 | 
cline_292_0:			/* LN: 295 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x3)			# LN: 295 | 
	a0 = 0			# LN: 295 | 
	xmem[i0] = a0h			# LN: 295 | 
	do (0x8), label_end_94			# LN: 295 | 
cline_295_0:			/* LN: 296 | CYCLE: 0 | RULES: () */ 
label_begin_94:			/* LN: 295 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x4)			# LN: 296 | 
	a0 = 0			# LN: 296 | 
	xmem[i0] = a0h			# LN: 296 | 
	do (0x10), label_end_93			# LN: 296 | 
cline_296_0:			/* LN: 297 | CYCLE: 0 | RULES: () */ 
label_begin_93:			/* LN: 296 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x3)			# LN: 297 | 
	a0 = xmem[i0]; a1 = 0			# LN: 297, 297 | 
	a0 = a0 << 4			# LN: 297 | 
	i0 = a0			# LN: 297 | 
	i1 = i7 - (0x4)			# LN: 297 | 
	i0 = i0 + (_sampleBuffer + 0)			# LN: 297 | 
	a0 = xmem[i1]			# LN: 297 | 
	b0 = i0			# LN: 297 | 
	a0 = a0 + b0			# LN: 297 | 
	AnyReg(i0, a0h)			# LN: 297 | 
	nop #empty cycle
	ymem[i0] = a1h			# LN: 297 | 
cline_297_0:			/* LN: 296 | CYCLE: 0 | RULES: () */ 
init_latch_label_1:			/* LN: 297 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x4)			# LN: 296 | 
	a0 = xmem[i0]			# LN: 296 | 
	uhalfword(a1) = (0x1)			# LN: 296 | 
	a0 = a0 + a1			# LN: 296 | 
	i0 = i7 - (0x4)			# LN: 296 | 
label_end_93:			# LN: 296 | CYCLE: 5 | RULES: ()
	xmem[i0] = a0h			# LN: 296 | 
cline_296_1:			/* LN: 295 | CYCLE: 0 | RULES: () */ 
init_latch_label_2:			/* LN: 297 | CYCLE: 0 | RULES: () */ 
for_end_2:			/* LN: 296 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x3)			# LN: 295 | 
	a0 = xmem[i0]			# LN: 295 | 
	uhalfword(a1) = (0x1)			# LN: 295 | 
	a0 = a0 + a1			# LN: 295 | 
	i0 = i7 - (0x3)			# LN: 295 | 
label_end_94:			# LN: 295 | CYCLE: 5 | RULES: ()
	xmem[i0] = a0h			# LN: 295 | 
cline_295_1:			/* LN: 301 | CYCLE: 0 | RULES: () */ 
for_end_1:			/* LN: 295 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x2)			# LN: 301 | 
	i0 = xmem[i0]			# LN: 301 | 
	i1 = i7 - (260 - 0)			# LN: 301 | 
	i4 = xmem[i0]			# LN: 301 | 
	i0 = i1			# LN: 301 | 
	i1 = i4			# LN: 301 | 
	call (_strcpy)			# LN: 301 | 
cline_301_0:			/* LN: 302 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (260 - 0)			# LN: 302 | 
	call (_cl_wavread_open)			# LN: 302 | 
	AnyReg(i0, a0h)			# LN: 302 | 
	i1 = i7 - (0x105)			# LN: 302 | 
	xmem[i1] = i0			# LN: 302 | 
cline_302_0:			/* LN: 303 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x105)			# LN: 303 | 
	a0 = xmem[i0]			# LN: 303 | 
	a0 & a0			# LN: 303 | 
	if (a != 0) jmp (else_2)			# LN: 303 | 
cline_303_0:			/* LN: 305 | CYCLE: 0 | RULES: () */ 
	i0 = (0) + (_string_const_0)			# LN: 305 | 
	call (_printf)			# LN: 305 | 
cline_305_0:			/* LN: 306 | CYCLE: 0 | RULES: () */ 
	halfword(a0) = (0xffff)			# LN: 306 | 
	jmp (__epilogue_244)			# LN: 306 | 
cline_306_0:			/* LN: 312 | CYCLE: 0 | RULES: () */ 
endif_2:			/* LN: 303 | CYCLE: 0 | RULES: () */ 
else_2:			/* LN: 303 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x105)			# LN: 312 | 
	i0 = xmem[i0]			# LN: 312 | 
	call (_cl_wavread_getnchannels)			# LN: 312 | 
	i0 = i7 - (0x106)			# LN: 312 | 
	xmem[i0] = a0h			# LN: 312 | 
cline_312_0:			/* LN: 313 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x105)			# LN: 313 | 
	i0 = xmem[i0]			# LN: 313 | 
	call (_cl_wavread_bits_per_sample)			# LN: 313 | 
	i0 = i7 - (0x107)			# LN: 313 | 
	xmem[i0] = a0h			# LN: 313 | 
cline_313_0:			/* LN: 314 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x105)			# LN: 314 | 
	i0 = xmem[i0]			# LN: 314 | 
	call (_cl_wavread_frame_rate)			# LN: 314 | 
	i0 = i7 - (0x108)			# LN: 314 | 
	xmem[i0] = a0h			# LN: 314 | 
cline_314_0:			/* LN: 315 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x105)			# LN: 315 | 
	i0 = xmem[i0]			# LN: 315 | 
	call (_cl_wavread_number_of_frames)			# LN: 315 | 
	i0 = i7 - (0x109)			# LN: 315 | 
	xmem[i0] = a0h			# LN: 315 | 
cline_315_0:			/* LN: 320 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x2)			# LN: 320 | 
	i0 = xmem[i0]			# LN: 320 | 
	i1 = i7 - (521 - 0)			# LN: 320 | 
	i0 += 1			# LN: 320 | 
	i4 = xmem[i0]			# LN: 320 | 
	i0 = i1			# LN: 320 | 
	i1 = i4			# LN: 320 | 
	call (_strcpy)			# LN: 320 | 
cline_320_0:			/* LN: 322 | CYCLE: 0 | RULES: () */ 
	a0 = xmem[_Mode + 0]			# LN: 322 | 
	a0 & a0			# LN: 322 | 
	if (a != 0) jmp (else_3)			# LN: 322 | 
cline_322_0:			/* LN: 324 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20a)			# LN: 324 | 
	uhalfword(a0) = (0x2)			# LN: 324 | 
	xmem[i0] = a0h			# LN: 324 | 
cline_324_0:			/* LN: 325 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (521 - 0)			# LN: 325 | 
	i1 = i7 - (0x107)			# LN: 325 | 
	a0 = xmem[i1]			# LN: 325 | 
	i1 = i7 - (0x20a)			# LN: 325 | 
	a1 = xmem[i1]			# LN: 325 | 
	i1 = i7 - (0x108)			# LN: 325 | 
	b0 = xmem[i1]			# LN: 325 | 
	call (_cl_wavwrite_open)			# LN: 325 | 
	AnyReg(i0, a0h)			# LN: 325 | 
	i1 = i7 - (0x20b)			# LN: 325 | 
	xmem[i1] = i0			# LN: 325 | 
cline_325_0:			/* LN: 326 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20b)			# LN: 326 | 
	a0 = xmem[i0]			# LN: 326 | 
	a0 & a0			# LN: 326 | 
	if (a != 0) jmp (else_4)			# LN: 326 | 
cline_326_0:			/* LN: 328 | CYCLE: 0 | RULES: () */ 
	i0 = (0) + (_string_const_1)			# LN: 328 | 
	call (_printf)			# LN: 328 | 
cline_328_0:			/* LN: 329 | CYCLE: 0 | RULES: () */ 
	halfword(a0) = (0xffff)			# LN: 329 | 
	jmp (__epilogue_244)			# LN: 329 | 
cline_329_0:			/* LN: 326 | CYCLE: 0 | RULES: () */ 
endif_4:			/* LN: 326 | CYCLE: 0 | RULES: () */ 
else_4:			/* LN: 326 | CYCLE: 0 | RULES: () */ 
	jmp (endif_3)			# LN: 326 | 
cline_326_1:			/* LN: 333 | CYCLE: 0 | RULES: () */ 
else_3:			/* LN: 322 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20a)			# LN: 333 | 
	uhalfword(a0) = (0x5)			# LN: 333 | 
	xmem[i0] = a0h			# LN: 333 | 
cline_333_0:			/* LN: 334 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (521 - 0)			# LN: 334 | 
	i1 = i7 - (0x107)			# LN: 334 | 
	a0 = xmem[i1]			# LN: 334 | 
	i1 = i7 - (0x20a)			# LN: 334 | 
	a1 = xmem[i1]			# LN: 334 | 
	i1 = i7 - (0x108)			# LN: 334 | 
	b0 = xmem[i1]			# LN: 334 | 
	call (_cl_wavwrite_open)			# LN: 334 | 
	AnyReg(i0, a0h)			# LN: 334 | 
	i1 = i7 - (0x20b)			# LN: 334 | 
	xmem[i1] = i0			# LN: 334 | 
cline_334_0:			/* LN: 335 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20b)			# LN: 335 | 
	a0 = xmem[i0]			# LN: 335 | 
	a0 & a0			# LN: 335 | 
	if (a != 0) jmp (else_5)			# LN: 335 | 
cline_335_0:			/* LN: 337 | CYCLE: 0 | RULES: () */ 
	i0 = (0) + (_string_const_2)			# LN: 337 | 
	call (_printf)			# LN: 337 | 
cline_337_0:			/* LN: 338 | CYCLE: 0 | RULES: () */ 
	halfword(a0) = (0xffff)			# LN: 338 | 
	jmp (__epilogue_244)			# LN: 338 | 
cline_338_0:			/* LN: 353 | CYCLE: 0 | RULES: () */ 
endif_3:			/* LN: 322 | CYCLE: 0 | RULES: () */ 
endif_5:			/* LN: 335 | CYCLE: 0 | RULES: () */ 
else_5:			/* LN: 335 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20c)			# LN: 353 | 
	a0 = 0			# LN: 353 | 
	xmem[i0] = a0h			# LN: 353 | 
for_3:			/* LN: 353 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x109)			# LN: 353 | 
	a0 = xmem[i0]			# LN: 353 | 
	uhalfword(a1) = (0x10)			# LN: 353 | 
	call (__div)			# LN: 353 | 
	i0 = i7 - (0x20c)			# LN: 353 | 
	a1 = xmem[i0]			# LN: 353 | 
	a1 - a0			# LN: 353 | 
	if (a >= 0) jmp (for_end_3)			# LN: 353 | 
cline_353_0:			/* LN: 355 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20d)			# LN: 355 | 
	a0 = 0			# LN: 355 | 
	xmem[i0] = a0h			# LN: 355 | 
	do (0x10), label_end_95			# LN: 355 | 
cline_355_0:			/* LN: 357 | CYCLE: 0 | RULES: () */ 
label_begin_95:			/* LN: 355 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20e)			# LN: 357 | 
	a0 = 0			# LN: 357 | 
	xmem[i0] = a0h			# LN: 357 | 
for_5:			/* LN: 357 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20e)			# LN: 357 | 
	a0 = xmem[i0]			# LN: 357 | 
	i0 = i7 - (0x106)			# LN: 357 | 
	a1 = xmem[i0]			# LN: 357 | 
	a0 - a1			# LN: 357 | 
	if (a >= 0) jmp (for_end_5)			# LN: 357 | 
cline_357_0:			/* LN: 359 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x105)			# LN: 359 | 
	i0 = xmem[i0]			# LN: 359 | 
	call (_cl_wavread_recvsample)			# LN: 359 | 
	i0 = i7 - (0x20f)			# LN: 359 | 
	xmem[i0] = a0h			# LN: 359 | 
cline_359_0:			/* LN: 360 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20e)			# LN: 360 | 
	a0 = xmem[i0]			# LN: 360 | 
	a0 = a0 << 4			# LN: 360 | 
	i0 = a0			# LN: 360 | 
	i1 = i7 - (0x20d)			# LN: 360 | 
	i0 = i0 + (_sampleBuffer + 0)			# LN: 360 | 
	a0 = xmem[i1]			# LN: 360 | 
	a1 = i0			# LN: 360 | 
	a0 = a1 + a0			# LN: 360 | 
	AnyReg(i0, a0h)			# LN: 360 | 
	i1 = i7 - (0x20f)			# LN: 360 | 
	a0 = xmem[i1]			# LN: 360 | 
	ymem[i0] = a0h			# LN: 360 | 
cline_360_0:			/* LN: 357 | CYCLE: 0 | RULES: () */ 
init_latch_label_3:			/* LN: 361 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20e)			# LN: 357 | 
	a0 = xmem[i0]			# LN: 357 | 
	uhalfword(a1) = (0x1)			# LN: 357 | 
	a0 = a0 + a1			# LN: 357 | 
	i0 = i7 - (0x20e)			# LN: 357 | 
	xmem[i0] = a0h			# LN: 357 | 
	jmp (for_5)			# LN: 357 | 
cline_357_1:			/* LN: 355 | CYCLE: 0 | RULES: () */ 
init_latch_label_4:			/* LN: 362 | CYCLE: 0 | RULES: () */ 
for_end_5:			/* LN: 357 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20d)			# LN: 355 | 
	a0 = xmem[i0]			# LN: 355 | 
	uhalfword(a1) = (0x1)			# LN: 355 | 
	a0 = a0 + a1			# LN: 355 | 
	i0 = i7 - (0x20d)			# LN: 355 | 
label_end_95:			# LN: 355 | CYCLE: 5 | RULES: ()
	xmem[i0] = a0h			# LN: 355 | 
cline_355_1:			/* LN: 366 | CYCLE: 0 | RULES: () */ 
for_end_4:			/* LN: 355 | CYCLE: 0 | RULES: () */ 
	i0 = (0) + (_sampleBuffer)			# LN: 366 | 
	i1 = (0) + (_sampleBuffer)			# LN: 366 | 
	call (_Processing)			# LN: 366 | 
cline_366_0:			/* LN: 368 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20d)			# LN: 368 | 
	a0 = 0			# LN: 368 | 
	xmem[i0] = a0h			# LN: 368 | 
	do (0x10), label_end_96			# LN: 368 | 
cline_368_0:			/* LN: 370 | CYCLE: 0 | RULES: () */ 
label_begin_96:			/* LN: 368 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20e)			# LN: 370 | 
	a0 = 0			# LN: 370 | 
	xmem[i0] = a0h			# LN: 370 | 
for_7:			/* LN: 370 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20e)			# LN: 370 | 
	a0 = xmem[i0]			# LN: 370 | 
	i0 = i7 - (0x20a)			# LN: 370 | 
	a1 = xmem[i0]			# LN: 370 | 
	a0 - a1			# LN: 370 | 
	if (a >= 0) jmp (for_end_7)			# LN: 370 | 
cline_370_0:			/* LN: 372 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20e)			# LN: 372 | 
	a0 = xmem[i0]			# LN: 372 | 
	a0 = a0 << 4			# LN: 372 | 
	i0 = a0			# LN: 372 | 
	i1 = i7 - (0x20d)			# LN: 372 | 
	i0 = i0 + (_sampleBuffer + 0)			# LN: 372 | 
	a0 = xmem[i1]			# LN: 372 | 
	a1 = i0			# LN: 372 | 
	a0 = a1 + a0			# LN: 372 | 
	AnyReg(i0, a0h)			# LN: 372 | 
	i1 = i7 - (0x20f)			# LN: 372 | 
	a0 = ymem[i0]			# LN: 372 | 
	xmem[i1] = a0h			# LN: 372 | 
cline_372_0:			/* LN: 373 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20b)			# LN: 373 | 
	i1 = i7 - (0x20f)			# LN: 373 | 
	a0 = xmem[i1]			# LN: 373 | 
	i0 = xmem[i0]			# LN: 373 | 
	call (_cl_wavwrite_sendsample)			# LN: 373 | 
cline_373_0:			/* LN: 370 | CYCLE: 0 | RULES: () */ 
init_latch_label_5:			/* LN: 374 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20e)			# LN: 370 | 
	a0 = xmem[i0]			# LN: 370 | 
	uhalfword(a1) = (0x1)			# LN: 370 | 
	a0 = a0 + a1			# LN: 370 | 
	i0 = i7 - (0x20e)			# LN: 370 | 
	xmem[i0] = a0h			# LN: 370 | 
	jmp (for_7)			# LN: 370 | 
cline_370_1:			/* LN: 368 | CYCLE: 0 | RULES: () */ 
init_latch_label_6:			/* LN: 375 | CYCLE: 0 | RULES: () */ 
for_end_7:			/* LN: 370 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20d)			# LN: 368 | 
	a0 = xmem[i0]			# LN: 368 | 
	uhalfword(a1) = (0x1)			# LN: 368 | 
	a0 = a0 + a1			# LN: 368 | 
	i0 = i7 - (0x20d)			# LN: 368 | 
label_end_96:			# LN: 368 | CYCLE: 5 | RULES: ()
	xmem[i0] = a0h			# LN: 368 | 
cline_368_1:			/* LN: 353 | CYCLE: 0 | RULES: () */ 
init_latch_label_7:			/* LN: 376 | CYCLE: 0 | RULES: () */ 
for_end_6:			/* LN: 368 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20c)			# LN: 353 | 
	a0 = xmem[i0]			# LN: 353 | 
	uhalfword(a1) = (0x1)			# LN: 353 | 
	a0 = a0 + a1			# LN: 353 | 
	i0 = i7 - (0x20c)			# LN: 353 | 
	xmem[i0] = a0h			# LN: 353 | 
	jmp (for_3)			# LN: 353 | 
cline_353_1:			/* LN: 381 | CYCLE: 0 | RULES: () */ 
for_end_3:			/* LN: 353 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x105)			# LN: 381 | 
	i0 = xmem[i0]			# LN: 381 | 
	call (_cl_wavread_close)			# LN: 381 | 
cline_381_0:			/* LN: 382 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x20b)			# LN: 382 | 
	i0 = xmem[i0]			# LN: 382 | 
	call (_cl_wavwrite_close)			# LN: 382 | 
cline_382_0:			/* LN: 385 | CYCLE: 0 | RULES: () */ 
	a0 = 0			# LN: 385 | 
	jmp (__epilogue_244)			# LN: 385 | 
cline_385_0:			/* LN: 386 | CYCLE: 0 | RULES: () */ 
__epilogue_244:			/* LN: 386 | CYCLE: 0 | RULES: () */ 
	i7 = i7 - (0x20f)			# LN: 386 | 
	i7 -= 1			# LN: 386 | 
	ret			# LN: 386 | 



_saturation:			/* LN: 105 | CYCLE: 0 | RULES: () */ 
	xmem[i7] = i7			# LN: 105 | 
	i7 += 1			# LN: 105 | 
	i7 = i7 + (0x4)			# LN: 105 | 
	i0 = i7 - (0x1)			# LN: 105 | 
	xmem[i0] = a0h			# LN: 105 | 
cline_105_0:			/* LN: 107 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x1)			# LN: 107 | 
	a0 = xmem[i0]			# LN: 107 | 
	i0 = i7 - (0x4)			# LN: 107 | 
	xmem[i0] = a0g; i0 += 1			# LN: 107, 107 | 
	xmem[i0] = a0h; i0 += 1			# LN: 107, 107 | 
	xmem[i0] = a0l			# LN: 107 | 
cline_107_0:			/* LN: 109 | CYCLE: 0 | RULES: () */ 
	ufixed16(b0) = (0x7fdf)			# LN: 109 | 
	lo16(b0) = (0x3b64)			# LN: 109 | 
	b0 = b0 >> 1			# LN: 109 | 
	i0 = i7 - (0x1)			# LN: 109 | 
	a0 = xmem[i0]			# LN: 109 | 
	a0 = a0 >> 1			# LN: 109 | 
	a0 - b0			# LN: 109 | 
	if (a <= 0) jmp (else_0)			# LN: 109 | 
cline_109_0:			/* LN: 111 | CYCLE: 0 | RULES: () */ 
	a0 = xmem[__extractedConst_0_1 + 0]			# LN: 111 | 
	jmp (__epilogue_238)			# LN: 111 | 
cline_111_0:			/* LN: 113 | CYCLE: 0 | RULES: () */ 
endif_0:			/* LN: 109 | CYCLE: 0 | RULES: () */ 
else_0:			/* LN: 109 | CYCLE: 0 | RULES: () */ 
	b0 = (0x8020)			# LN: 113 | 
	lo16(b0) = (0xc49c)			# LN: 113 | 
	b0 = b0 >> 1			# LN: 113 | 
	i0 = i7 - (0x1)			# LN: 113 | 
	a0 = xmem[i0]			# LN: 113 | 
	a0 = a0 >> 1			# LN: 113 | 
	a0 - b0			# LN: 113 | 
	if (a >= 0) jmp (else_1)			# LN: 113 | 
cline_113_0:			/* LN: 115 | CYCLE: 0 | RULES: () */ 
	a0 = xmem[__extractedConst_1_1 + 0]			# LN: 115 | 
	jmp (__epilogue_238)			# LN: 115 | 
cline_115_0:			/* LN: 118 | CYCLE: 0 | RULES: () */ 
endif_1:			/* LN: 113 | CYCLE: 0 | RULES: () */ 
else_1:			/* LN: 113 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x1)			# LN: 118 | 
	a0 = xmem[i0]			# LN: 118 | 
	jmp (__epilogue_238)			# LN: 118 | 
cline_118_0:			/* LN: 119 | CYCLE: 0 | RULES: () */ 
__epilogue_238:			/* LN: 119 | CYCLE: 0 | RULES: () */ 
	i7 = i7 - (0x4)			# LN: 119 | 
	i7 -= 1			# LN: 119 | 
	ret			# LN: 119 | 
