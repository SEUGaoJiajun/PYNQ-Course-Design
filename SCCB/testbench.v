`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/06/01 13:11:30
// Design Name: 
// Module Name: testbench
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


module testbench(

    );
    reg     sclk,rst_n;
    
    
    initial begin
            rst_n=1'b0;
            sclk=1'b0;
            #100 rst_n=1'b1;
    end
    always #5 sclk=~sclk;
    sccb_ctrl_01    sccb_ctrl_01(
            .axi_clk            (sclk),
            .axi_rst_n          (rst_n),
            .scl                (scl),//,//sccbport
            .sda                (sda)
            );
endmodule
