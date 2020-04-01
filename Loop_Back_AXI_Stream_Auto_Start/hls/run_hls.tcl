############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2013 Xilinx Inc. All rights reserved.
############################################################

# Create a Vivado HLS project
open_project -reset hls_loop_back_axi_stream_prj
set_top loopback
add_files loopback.cpp -cflags "-std=c++11 -Wno-unknown-pragmas" 
add_files -tb loopback_tb.cpp -cflags "-std=c++11 -Wno-unknown-pragmas" 

# Solution1 *************************
open_solution -reset "solution1"
set_part  {xc7z020clg484-1} 
create_clock -period 10 -name default  
source "directives.tcl"

# Run C simulation
csim_design
# Run Synthesis
csynth_design
# Run RTL verification
cosim_design
# Create the IP package
export_design

exit



