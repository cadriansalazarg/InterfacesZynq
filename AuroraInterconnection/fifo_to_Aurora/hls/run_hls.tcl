# Create a Vivado HLS project
open_project -reset hls_fifo_to_aurora_hw_prj

set_top fifo_to_Aurora_IP
add_files fifo_to_Aurora_IP.cpp -cflags "-std=c++11 -Wno-unknown-pragmas"
add_files -tb fifo_to_Aurora_IP_tb.cpp -cflags "-std=c++11 -Wno-unknown-pragmas" 

open_solution -reset "Optimized"
set_part  {xc7z020clg484-1} 
create_clock -period 2.0 -name default  

config_compile  
config_schedule -effort high  -relax_ii_for_timing=0 
config_bind -effort high
config_sdx -optimization_level s -target none
set_clock_uncertainty 0.125

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
