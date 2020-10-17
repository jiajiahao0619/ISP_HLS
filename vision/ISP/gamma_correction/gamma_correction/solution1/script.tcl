############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
open_project gamma_correction
set_top gammacorrection_accel
add_files xf_gammacorrection_accel.cpp -cflags "-D__SDSVHLS__ -I../../L1/include/ --std=c++0x"
add_files -tb data/128x128.png
add_files -tb data/ref_Y.png
add_files -tb xf_gammacorrection_config.h -cflags "-D__SDSVHLS__ -I../../L1/include --std=c++0x"
add_files -tb xf_gammacorrection_tb.cpp -cflags "-D__SDSVHLS__ -I../../L1/include --std=c++0x"
open_solution "solution1"
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 10 -name default
#source "./gamma_correction/solution1/directives.tcl"
csim_design -argv {ref_Y.png}
csynth_design
cosim_design
export_design -format ip_catalog
