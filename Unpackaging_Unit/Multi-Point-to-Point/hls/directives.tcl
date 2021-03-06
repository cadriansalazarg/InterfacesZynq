############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
set_directive_interface -mode ap_fifo "unpackaging_IP_block" in_fifo
set_directive_interface -mode axis -register_mode off "unpackaging_IP_block" output
set_directive_interface -mode ap_none "unpackaging_IP_block" current_node
set_directive_interface -mode ap_ctrl_chain -register "unpackaging_IP_block"
set_directive_unroll "unpackaging_IP_block/Offset_Producer"
set_directive_pipeline "memcopy/LoopPayload"
set_directive_pipeline "unpackaging_IP_block/Loop_Consumer"
set_directive_array_partition -type complete -dim 1 "unpackaging_IP_block" offset_nodes
set_directive_resource -core RAM_2P_BRAM "memcopy" Message

# Uncomment to use this optimization, change factor to be a submultiple of ((PACKAGE_SIZE_BYTES-8)/8)
#set_directive_array_partition -type block -factor 3 -dim 1 "unpackaging_IP_block" packet.MESSAGE