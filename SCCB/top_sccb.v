`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/06/04 12:56:38
// Design Name: 
// Module Name: top_sccb
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module top_sccb(
    output  wire    scl,
    inout   wire    sda,
    inout   wire   [53:0]  FIX_IO_mio,
    inout   wire    FIXED_IO_0_ps_clk,
    inout   wire    FIXED_IO_0_ps_porb,
    inout   wire    FIXED_IO_0_ps_srstb
    
    );
wire    FCLK_CLK0_0;
wire    FCLK_RESET0_N_0;
wire [31:0] WR_DATA_0;
wire [31:0] WR_END_0;
wire [31:0] WR_FLAG_0;
design_1_wrapper design_1_wrapper
       (.FCLK_CLK0_0                 (FCLK_CLK0_0),
        .FCLK_RESET0_N_0             (FCLK_RESET0_N_0),
        .FIXED_IO_0_mio              (FIX_IO_mio),
        .FIXED_IO_0_ps_clk           (FIXED_IO_0_ps_clk),
        .FIXED_IO_0_ps_porb          (FIXED_IO_0_ps_porb),
        .FIXED_IO_0_ps_srstb         (FIXED_IO_0_ps_srstb),
        .WR_DATA_0(WR_DATA_0),
        .WR_END_0(WR_END_0),
        .WR_FLAG_0(WR_FLAG_0)
        ) ;    
 sccb_ctrl_01 sccb_ctrl_01(
        .axi_clk        (FCLK_CLK0_0),
        .axi_rst_n      (FCLK_RESET0_N_0),
        .scl            (scl),//sccbport
        .sda            (sda),
        .WR_DATA_0(WR_DATA_0),
        .WR_END_0(WR_END_0),
        .WR_FLAG_0(WR_FLAG_0)
                );   
endmodule
