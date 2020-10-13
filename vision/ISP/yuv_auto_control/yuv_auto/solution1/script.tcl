############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
open_project yuv_auto
set_top yuv_auto_control
add_files yuv_auto_control.cpp
add_files -tb yuv_auto_control.cpp
open_solution "solution1"
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 10 -name default
#source "./yuv_auto/solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -format ip_catalog
