############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
open_project sat_ctrl
set_top sat_ctrl_accel
add_files xf_sat_ctrl_accel.cpp -cflags "-D__SDSVHLS__ -I../../L1/include/. --std=c++0x"
add_files xf_sat_ctrl_config.h -cflags "-D__SDSVHLS__ -I../../L1/include/. --std=c++0x"
add_files -tb xf_sat_ctrl_accel_tb.cpp -cflags "-I../../L1/include -D__SDSVHLS__ --std=c++0x -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution1"
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 10 -name default
#source "./sat_ctrl/solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -format ip_catalog
