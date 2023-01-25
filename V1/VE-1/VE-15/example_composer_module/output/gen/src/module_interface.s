	.public _M70_CC_MALLOCED
	.public _M70_CC_MALLOC_PTR
	.public _M70_CC_MCV
	.public _M70_CC_MIF
	.public _M70_CC_PCT
	.extern _X_P_BY_Simple_delay_ms_PCT
	.extern ___C_STACK_BG
	.extern ___C_STACK_FG
	.extern ___END__pct
	.extern ___START__pct
	.extern ___X_BL_TEMPBUFF
	.extern ___X_BL_TEMPBUFF0
	.extern ___X_BL_TEMPBUFF1
	.extern ___X_BL_TEMPBUFF10
	.extern ___X_BL_TEMPBUFF11
	.extern ___X_BL_TEMPBUFF12
	.extern ___X_BL_TEMPBUFF13
	.extern ___X_BL_TEMPBUFF14
	.extern ___X_BL_TEMPBUFF15
	.extern ___X_BL_TEMPBUFF2
	.extern ___X_BL_TEMPBUFF3
	.extern ___X_BL_TEMPBUFF4
	.extern ___X_BL_TEMPBUFF5
	.extern ___X_BL_TEMPBUFF6
	.extern ___X_BL_TEMPBUFF7
	.extern ___X_BL_TEMPBUFF8
	.extern ___X_BL_TEMPBUFF9
	.extern ___X_BX_TEMPBUFF
	.extern ___X_BX_TEMPBUFF0
	.extern ___X_BX_TEMPBUFF1
	.extern ___X_BX_TEMPBUFF10
	.extern ___X_BX_TEMPBUFF11
	.extern ___X_BX_TEMPBUFF12
	.extern ___X_BX_TEMPBUFF13
	.extern ___X_BX_TEMPBUFF14
	.extern ___X_BX_TEMPBUFF15
	.extern ___X_BX_TEMPBUFF2
	.extern ___X_BX_TEMPBUFF3
	.extern ___X_BX_TEMPBUFF4
	.extern ___X_BX_TEMPBUFF5
	.extern ___X_BX_TEMPBUFF6
	.extern ___X_BX_TEMPBUFF7
	.extern ___X_BX_TEMPBUFF8
	.extern ___X_BX_TEMPBUFF9
	.extern ___X_BY_IOBUFFER_PTRS
	.extern ___X_BY_TEMPBUFF
	.extern ___X_BY_TEMPBUFF0
	.extern ___X_BY_TEMPBUFF1
	.extern ___X_BY_TEMPBUFF10
	.extern ___X_BY_TEMPBUFF11
	.extern ___X_BY_TEMPBUFF12
	.extern ___X_BY_TEMPBUFF13
	.extern ___X_BY_TEMPBUFF14
	.extern ___X_BY_TEMPBUFF15
	.extern ___X_BY_TEMPBUFF2
	.extern ___X_BY_TEMPBUFF3
	.extern ___X_BY_TEMPBUFF4
	.extern ___X_BY_TEMPBUFF5
	.extern ___X_BY_TEMPBUFF6
	.extern ___X_BY_TEMPBUFF7
	.extern ___X_BY_TEMPBUFF8
	.extern ___X_BY_TEMPBUFF9
	.extern ___X_VX_NEXTFRM_REINIT_REQ
	.extern ___X_VX_PPM_INPUTMODE
	.extern ___X_VX_PPM_INPUT_CHANNELS
	.extern ___X_VX_PPM_OUTPUTMODE
	.extern ___X_VX_PPM_OUTPUT_CHANNELS
	.extern ___X_VX_PPM_SAMPLERATE
	.extern ___X_VX_PPM_VALID_CHANNELS
	.extern ___X_VX_REINIT
	.extern ___X_VX_UNDERFLOW
	.extern ___mc_copy__pct
	.extern _cl_buffer_ground_PTR
	.extern _cl_buffer_silence_PTR
	.extern _cl_constant_one_x
	.extern _cl_constant_one_y
	.extern _cl_constant_zero_x
	.extern _cl_constant_zero_y
	.extern _filter_highpass_transposed_direct_II_pct
	.extern _simple_gain_db_pct
	.extern _simple_sum_pct
	.extern _M70_CC_background
	.extern _M70_CC_brick
	.extern _M70_CC_frame
	.extern _M70_CC_postKickstart
	.extern _M70_CC_postMallocInit
	.extern _M70_CC_preMallocInit
	.extern _cl_biquad_prim_register_coeffs
	.extern _cl_prim_gain_down_24db
	.extern _cl_prim_gain_up_24db
X___mc_copy___0_PRIVATE_DATA_1	.xdata_ovly
S___mc_copy___0_PRIVATE_DATA_1_0	.struct
_channel_count	.dw (0x0)
_audio_brick_in	.dw (0x0)
_audio_brick_out	.dw (0x0)
	.endstruct

X_M70_PI_simple_sum_12_PRIVATE_DATA_1	.xdata_ovly
S_M70_PI_simple_sum_12_PRIVATE_DATA_1_0	.struct
_bypass	.dw (0x0)
_audio_brick_out	.dw (0x0)
_audio_brick_in	.dw (0x0)
	.dw (0x0)
	.endstruct

X_M70_PI_simple_gain_db_6_PRIVATE_DATA_1	.xdata_ovly
S_M70_PI_simple_gain_db_6_PRIVATE_DATA_1_0	.struct
_bypass	.dw (0x0)
_input_ptr	.dw (0x0)
_output_ptr	.dw (0x0)
	.endstruct

X_M70_PI_simple_delay_ms_8_PRIVATE_DATA_1	.xdata_ovly
S_M70_PI_simple_delay_ms_8_PRIVATE_DATA_1_0	.struct
_delay_buffer_ptr	.dw (0x0)
_bypass	.dw (0x0)
_audio_brick_in	.dw (0x0)
_audio_brick_out	.dw (0x0)
_sample_resolution	.dw (0x0)
_position_in_buffer	.dw (0x0)
_max_delay_size	.dw (0x0)
_memory_zone	.dw (0x0)
	.endstruct

X_M70_PI_filter_highpass_transposed_direct_II_11_PRIVATE_DATA_1	.xdata_ovly
S_M70_PI_filter_highpass_transposed_direct_II_11_PRIVATE_DATA_1_0	.struct
_pointer	.dw (0x0)
_bypass	.dw (0x0)
_audio_brick_in	.dw (0x0)
_audio_brick_out	.dw (0x0)
_biquad_loop_count	.dw (0x0)
_myccr	.dw (0x0)
_clear_time	.dw (0x0)
_clip_ptr	.dw (0x0)
	.endstruct

X_M70_PI___START___2_PRIVATE_DATA_1	.xdata_ovly
S_M70_PI___START___2_PRIVATE_DATA_1_0	.struct
_headroom_proc	.dw (0x0)
_channel_mask	.dw (0x0)
	.endstruct

X_M70_PI___END___4_PRIVATE_DATA_1	.xdata_ovly
S_M70_PI___END___4_PRIVATE_DATA_1_0	.struct
_headroom_proc	.dw (0x0)
_channel_mask	.dw (0x0)
_mute_disconnected_channels	.dw (0x0)
_input_channel	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.endstruct

Y_M70_CC_MIF	.ydata_ovly
S_M70_CC_MIF_0	.struct
_mcv_ptr	.dw (0x0)
_mct_ptr	.dw (0x0)
_reserved1	.dw (0x0)
_reserved2	.dw (0x0)
_reserved3	.dw (0x0)
_reserved4	.dw (0x0)
_reserved5	.dw (0x0)
_reserved6	.dw (0x0)
	.endstruct

X_M70_PI_simple_sum_12_PUBLIC_DATA	.xdata_ovly
S_M70_PI_simple_sum_12_PUBLIC_DATA_0	.struct
_shift_value	.dw (0x0)
	.endstruct

X_M70_PI_filter_highpass_transposed_direct_II_11_PUBLIC_DATA	.xdata_ovly
S_M70_PI_filter_highpass_transposed_direct_II_11_PUBLIC_DATA_0	.struct
_hold_count	.dw (0x0)
_ramp	.dw (0x0)
_k	.dw (0x0)
_structure_instance	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
_clip	.dw (0x0)
	.endstruct

X_M70_PI_simple_delay_ms_8_PUBLIC_DATA	.xdata_ovly
S_M70_PI_simple_delay_ms_8_PUBLIC_DATA_0	.struct
_enable	.dw (0x0)
_delay_value	.dw (0x0)
	.endstruct

X_M70_PI_simple_gain_db_6_PUBLIC_DATA	.xdata_ovly
S_M70_PI_simple_gain_db_6_PUBLIC_DATA_0	.struct
_Gain	.dw (0x0)
_signal_invert	.dw (0x0)
	.endstruct

X_M70_PI___END___4_PUBLIC_DATA	.xdata_ovly
S_M70_PI___END___4_PUBLIC_DATA_0	.struct
_hold_count	.dw (0x0)
_clip	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.dw (0x0)
	.endstruct

Y_M70_CC_MCV	.ydata_ovly
S_M70_CC_MCV_0	.struct
_sample_rate	.dw (0x0)
_M70_PI___END___4_PUBLIC_DATA S_M70_PI___END___4_PUBLIC_DATA_0
_M70_PI_simple_gain_db_6_PUBLIC_DATA S_M70_PI_simple_gain_db_6_PUBLIC_DATA_0
_M70_PI_simple_delay_ms_8_PUBLIC_DATA S_M70_PI_simple_delay_ms_8_PUBLIC_DATA_0
_M70_PI_filter_highpass_transposed_direct_II_11_PUBLIC_DATA S_M70_PI_filter_highpass_transposed_direct_II_11_PUBLIC_DATA_0
_M70_PI_simple_sum_12_PUBLIC_DATA S_M70_PI_simple_sum_12_PUBLIC_DATA_0
	.endstruct

Y_M70_CC_MCT	.ydata_ovly
S_M70_CC_MCT_0	.struct
_prekick_func_ptr	.dw (0x0)
_postkick_func_ptr	.dw (0x0)
_timer_func_ptr	.dw (0x0)
_frame_func_ptr	.dw (0x0)
_block_func_ptr	.dw (0x0)
_reserved1	.dw (0x0)
_background_func_ptr	.dw (0x0)
_postmalloc_func_ptr	.dw (0x0)
_premalloc_func_ptr	.dw (0x0)
	.endstruct

Y_M70_CC_BUFFER_0	.ydata_ovly align 16
_M70_CC_BUFFER_0
	.bss (0x10)
Y_M70_CC_BUFFER_0_PTR	.ydata_ovly
_M70_CC_BUFFER_0_PTR
	.dw _M70_CC_BUFFER_0
Y_M70_CC_BUFFER_1	.ydata_ovly align 16
_M70_CC_BUFFER_1
	.bss (0x10)
Y_M70_CC_BUFFER_1_PTR	.ydata_ovly
_M70_CC_BUFFER_1_PTR
	.dw _M70_CC_BUFFER_1
Y_M70_CC_BUFFER_2	.ydata_ovly align 16
_M70_CC_BUFFER_2
	.bss (0x10)
Y_M70_CC_BUFFER_2_PTR	.ydata_ovly
_M70_CC_BUFFER_2_PTR
	.dw _M70_CC_BUFFER_2
Y_M70_CC_BUFFER_3	.ydata_ovly align 16
_M70_CC_BUFFER_3
	.bss (0x10)
Y_M70_CC_BUFFER_3_PTR	.ydata_ovly
_M70_CC_BUFFER_3_PTR
	.dw _M70_CC_BUFFER_3
Y_M70_CC_MALLOCED	.ydata_ovly
_M70_CC_MALLOCED
	.dw  (0x1)
Y_M70_CC_MALLOC_PTR	.ydata_ovly
_M70_CC_MALLOC_PTR
	.dw _M70_PI_simple_delay_ms_8_PRIVATE_DATA_1
	.dw  (0x1)
	.dw  (0x1f0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
Y_M70_CC_MCT	.ydata_ovly
_M70_CC_MCT S_M70_CC_MCT_0 (
 0x0,
 _M70_CC_postKickstart,
 0x0,
 _M70_CC_frame,
 _M70_CC_brick,
 0x0,
 _M70_CC_background,
 _M70_CC_postMallocInit,
 _M70_CC_preMallocInit
)

Y_M70_CC_MCV	.ydata_ovly
_M70_CC_MCV S_M70_CC_MCV_0 (
 0x0,
 0x0,
 0x0,
 0x0,
 0x0,
 0x0,
 0x0,
 0x0,
 0x0,
 0x0,
 0x0,
 0x0,
 0x0,
 0x0,
 0xfe800000,
 0x0,
 0x1,
 0xa0000,
 0xbb8,
 0x147ae14,
 0x0,
 0x7fffffff,
 0x7fffffff,
 0x1,
 0x7fffffff,
 0x7fffffff,
 0x1,
 0x0,
 0x0
)

Y_M70_CC_MIF	.ydata_ovly
_M70_CC_MIF S_M70_CC_MIF_0 (
 _M70_CC_MCV,
 _M70_CC_MCT,
 0x0,
 0x0,
 0x0,
 0x0,
 0x0,
 0x0
)

Y_M70_CC_PCT	.ydata_ovly
_M70_CC_PCT
	.dw ___START__pct
	.dw  (0x0)
	.dw _M70_PI___START___2_PRIVATE_DATA_1
	.dw  (0x0)
	.dw _filter_highpass_transposed_direct_II_pct
	.dw _M70_CC_MCV+18
	.dw _M70_PI_filter_highpass_transposed_direct_II_11_PRIVATE_DATA_1
	.dw _M70_PI_filter_highpass_transposed_direct_II_11_PRIVATE_DATA_2
	.dw _simple_gain_db_pct
	.dw _M70_CC_MCV+14
	.dw _M70_PI_simple_gain_db_6_PRIVATE_DATA_1
	.dw  (0x0)
	.dw ___mc_copy__pct
	.dw  (0x0)
	.dw ___mc_copy___0_PRIVATE_DATA_1
	.dw  (0x0)
	.dw _X_P_BY_Simple_delay_ms_PCT
	.dw _M70_CC_MCV+16
	.dw _M70_PI_simple_delay_ms_8_PRIVATE_DATA_1
	.dw  (0x0)
	.dw _simple_sum_pct
	.dw _M70_CC_MCV+28
	.dw _M70_PI_simple_sum_12_PRIVATE_DATA_1
	.dw  (0x0)
	.dw ___END__pct
	.dw _M70_CC_MCV+1
	.dw _M70_PI___END___4_PRIVATE_DATA_1
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
	.dw  (0x0)
X_M70_PI___END___4_PRIVATE_DATA_1	.xdata_ovly
_M70_PI___END___4_PRIVATE_DATA_1 S_M70_PI___END___4_PRIVATE_DATA_1_0 (
 _cl_prim_gain_up_24db,
 0xf,
 0x1,
 _M70_CC_BUFFER_0_PTR,
 _M70_CC_BUFFER_3_PTR,
 _M70_CC_BUFFER_1_PTR,
 _M70_CC_BUFFER_2_PTR,
 _cl_buffer_silence_PTR,
 _cl_buffer_silence_PTR,
 _cl_buffer_silence_PTR,
 _cl_buffer_silence_PTR,
 _cl_buffer_silence_PTR,
 _cl_buffer_silence_PTR,
 _cl_buffer_silence_PTR,
 _cl_buffer_silence_PTR
)

X_M70_PI___START___2_PRIVATE_DATA_1	.xdata_ovly
_M70_PI___START___2_PRIVATE_DATA_1 S_M70_PI___START___2_PRIVATE_DATA_1_0 (
 _cl_prim_gain_down_24db,
 0x3
)

X_M70_PI_filter_highpass_transposed_direct_II_11_PRIVATE_DATA_1	.xdata_ovly
_M70_PI_filter_highpass_transposed_direct_II_11_PRIVATE_DATA_1 S_M70_PI_filter_highpass_transposed_direct_II_11_PRIVATE_DATA_1_0 (
 _cl_biquad_prim_register_coeffs,
 _cl_constant_zero_x,
 ___X_BY_IOBUFFER_PTRS,
 _M70_CC_BUFFER_0_PTR,
 0x2,
 0x0,
 0x0,
 _M70_CC_MCV+27
)

L_M70_PI_filter_highpass_transposed_direct_II_11_PRIVATE_DATA_2	.data_ovly
_M70_PI_filter_highpass_transposed_direct_II_11_PRIVATE_DATA_2
	.dw  (0x0), (0x0)
	.dw  (0x1), (0x1)
	.dw  (0x1), (0x1)
	.dw  (0x0), (0x0)
	.dw  (0x0), (0x0)
	.dw  (0x1), (0x1)
	.dw  (0x1), (0x1)
	.dw  (0x0), (0x0)
	.dw  (0x0), (0x0)
X_M70_PI_simple_delay_ms_8_PRIVATE_DATA_1	.xdata_ovly
_M70_PI_simple_delay_ms_8_PRIVATE_DATA_1 S_M70_PI_simple_delay_ms_8_PRIVATE_DATA_1_0 (
 0x0,
 _cl_constant_zero_x,
 _M70_CC_BUFFER_1_PTR,
 _M70_CC_BUFFER_3_PTR,
 0x10,
 0x0,
 0x1f0,
 0x1
)

X_M70_PI_simple_gain_db_6_PRIVATE_DATA_1	.xdata_ovly
_M70_PI_simple_gain_db_6_PRIVATE_DATA_1 S_M70_PI_simple_gain_db_6_PRIVATE_DATA_1_0 (
 _cl_constant_zero_x,
 ___X_BY_IOBUFFER_PTRS+1,
 _M70_CC_BUFFER_1_PTR
)

X_M70_PI_simple_sum_12_PRIVATE_DATA_1	.xdata_ovly
_M70_PI_simple_sum_12_PRIVATE_DATA_1 S_M70_PI_simple_sum_12_PRIVATE_DATA_1_0 (
 _cl_constant_zero_x,
 _M70_CC_BUFFER_1_PTR,
 ___X_BY_IOBUFFER_PTRS+1,
 _M70_CC_BUFFER_3_PTR
)

X___mc_copy___0_PRIVATE_DATA_1	.xdata_ovly
___mc_copy___0_PRIVATE_DATA_1 S___mc_copy___0_PRIVATE_DATA_1_0 (
 0x1,
 ___X_BY_IOBUFFER_PTRS+1,
 _M70_CC_BUFFER_2_PTR
)

	.code_ovly
