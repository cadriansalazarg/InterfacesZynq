# Create a Vivado HLS project
open_project -reset hls_unpackaging_block_hw_prj
set_top unpackaging_IP_block
add_files unpackaging_IP.cpp -cflags "-std=c++11 -Wno-unknown-pragmas"
add_files -tb unpackaging_IP_tb.cpp -cflags "-std=c++11 -Wno-unknown-pragmas" 

open_solution -reset "solution1"
set_part  {xc7z020clg484-1} 
create_clock -period 3.2 -name default  

config_compile  
config_schedule -effort high  -relax_ii_for_timing=0 
config_bind -effort high
config_sdx -optimization_level s -target none
set_clock_uncertainty 0.5

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
