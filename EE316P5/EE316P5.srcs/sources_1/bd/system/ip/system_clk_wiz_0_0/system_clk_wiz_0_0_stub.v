// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.1.3 (win64) Build 2644227 Wed Sep  4 09:45:24 MDT 2019
// Date        : Tue Apr  5 18:09:58 2022
// Host        : DESKTOP-0NNGBEI running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               c:/Users/sixpe/Documents/college/EE316P5/EE316P5/EE316P5.srcs/sources_1/bd/system/ip/system_clk_wiz_0_0/system_clk_wiz_0_0_stub.v
// Design      : system_clk_wiz_0_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z010clg400-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
module system_clk_wiz_0_0(clk_125Mhz, reset, locked, clk_in1)
/* synthesis syn_black_box black_box_pad_pin="clk_125Mhz,reset,locked,clk_in1" */;
  output clk_125Mhz;
  input reset;
  output locked;
  input clk_in1;
endmodule
