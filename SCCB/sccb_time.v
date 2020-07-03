module sccb_ctrl_01(
		input	wire		axi_clk,
		input 	wire		axi_rst_n,
		inout	wire    [31:0]  WR_DATA_0,
		inout	wire    [31:0]  WR_END_0,
		inout	wire    [31:0]  WR_FLAG_0,
		output	reg			scl,//sccbport
		inout	wire		sda
		);
		
parameter DIV_MAX=500-1;//假设AXI—�?�CLK�?100M I2C时钟，等�?200k时的分频系数
parameter SCL_P=((DIV_MAX+1)/2)-1;
parameter SCL_N=0;
parameter SDA_P=((DIV_MAX+1)/4)-1;
parameter SDA_N=((DIV_MAX+1)/4)*3-1;
	
reg	[31:0]	write_flag_r;//axi总线写入寄存器�?�，表示有数据需要写入I2C
reg	[31:0]	write_end_r;//axi总线读取的寄存器值，表示本次I2C写入完毕 1是end
reg	[31:0]	write_data_r=32'h0065_4321;//高八位是保留位，其中�?24位为{ID ADDR，REG_ADDR,REG_DATA}

reg			w_start_flag;
reg			w_end_flag;
reg			w_3phase_flag;//三相位写{ID ADDR，REG_ADDR,REG_DATA}
reg	[8:0]	div_cnt;
reg			scl_pose_flag,scl_nege_flag;
reg			sda_pose_flag,sda_nege_flag;
reg			sda_o;
wire 	[7:0]	phase_1,phase_2,phase_3;
reg		[7:0]	phase_1_r,phase_2_r,phase_3_r;//移位用，SDA上升沿赋值，下降沿移�?
reg				phase_1_flag;
reg		[3:0]	phase_1_cnt;
reg				phase_2_flag;
reg		[3:0]	phase_2_cnt;
reg				phase_3_flag;
reg		[3:0]	phase_3_cnt;
reg             en;//��en=1�����Z ;

assign sda=sda_o;
//assign sda=(en==1'b0)?sda_o:1'b0;
//分频
always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n ==1'b0)
		begin
			div_cnt<='d0;
		end
		else if (div_cnt==DIV_MAX&&w_3phase_flag==1'b1)
		begin 
			div_cnt<='d0;
		end
		else if(w_3phase_flag==1'b1)
		begin 
			div_cnt<=div_cnt+1'b1;
		end
		else
		begin
			div_cnt='d0;
		end
//四个标志
always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
		begin
			scl_pose_flag<=1'b0;
		end
		else if(w_3phase_flag==1'b1&&div_cnt==SCL_P)
		begin
			scl_pose_flag<=1'b1;
		end
		else
		begin
			scl_pose_flag<=1'b0;
		end
		
always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
		begin
			scl_nege_flag<=1'b0;
		end
		else if(w_3phase_flag==1'b1&&div_cnt==SCL_N)
		begin
			scl_nege_flag<=1'b1;
		end
		else
		begin
			scl_nege_flag<=1'b0;
		end

always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
		begin
			sda_nege_flag<=1'b0;
		end
		else if(w_3phase_flag==1'b1&&div_cnt==SDA_N)
		begin
			sda_nege_flag<=1'b1;
		end
		else
		begin
			sda_nege_flag<=1'b0;
		end

always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
		begin
			sda_pose_flag<=1'b0;
		end
		else if(w_3phase_flag==1'b1&&div_cnt==SDA_P)
		begin
			sda_pose_flag<=1'b1;
		end
		else
		begin
			sda_pose_flag<=1'b0;
		end
		
always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
			w_3phase_flag<=1'b0;
		else if(write_flag_r[0]==1'b1&&write_end_r[0]==1'b0)
			w_3phase_flag<=1'b1;
		else if(w_end_flag==1'b1&&scl_nege_flag==1'b1)//当stop条件完成时，w_3phase_flag<=1'b0
			w_3phase_flag<=1'b0;
			
always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
		begin
			w_start_flag<=1'b0;
		end
		else if(w_3phase_flag==1'b1&&scl_nege_flag==1'b1&&w_start_flag==1'b0&&{phase_1_flag,phase_2_flag,phase_3_flag,w_end_flag}==4'b0000)//缺少其他标志无效的条�?
		begin
			w_start_flag<=1'b1;
		end
		else if(w_start_flag==1'b1&&scl_nege_flag==1'b1)
		begin 
			w_start_flag<=1'b0;
		end

always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
			sda_o<=1'b0;
		else if(w_3phase_flag==1'b1&&scl_nege_flag==1'b1&&w_start_flag==1'b0&&{phase_1_flag,phase_2_flag,phase_3_flag,w_end_flag}==4'b0000)
			sda_o<=1'b1;
		else if(w_start_flag==1'b1&&scl_pose_flag==1'b1)
			sda_o<=1'b0;
		else if(phase_1_flag==1'b1&&sda_pose_flag==1'b1)
			sda_o<=phase_1_r[7];//移位寄存器与sda_o相关�?
		else if(phase_2_flag==1'b1&&sda_pose_flag==1'b1)
			sda_o<=phase_2_r[7];
		else if(phase_3_flag==1'b1&&sda_pose_flag==1'b1)
            sda_o<=phase_3_r[7];	
		else if(w_end_flag==1'b1&&sda_pose_flag==1'b1)
			sda_o<=1'b0;
		else if(w_end_flag==1'b1&&sda_nege_flag==1'b1)
			sda_o<=1'b1;
		
always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
			scl<=1'b1;
		else if(w_start_flag==1'b1&&scl_nege_flag==1'b1)
			scl<=1'b0;
		else if(w_start_flag==1'b1)
			scl<=1'b1;
		else if((phase_1_flag==1'b1||phase_2_flag==1'b1||phase_3_flag==1'b1)&&scl_nege_flag==1'b1)//在w_end_flag时不让scl变低以保持为�?
			scl<=1'b0;
		else if((phase_1_flag==1'b1||phase_2_flag==1'b1||phase_3_flag==1'b1||w_end_flag==1'b1)&&scl_pose_flag==1'b1)
			scl<=1'b1;
			
assign	phase_1=write_data_r[23:16];
assign	phase_2=write_data_r[15:8];
assign	phase_3=write_data_r[7:0];
//相位1
always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
			phase_1_r<='d0;
		else if(w_start_flag==1'b1&&scl_nege_flag==1'b1)
			phase_1_r<=phase_1;
		else if(phase_1_flag==1'b1&&sda_nege_flag==1'b1)
			phase_1_r<={phase_1_r[6:0],1'b1};//移位操作

always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
			phase_1_flag<=1'b0;
		else if(w_start_flag==1'b1&&scl_nege_flag==1'b1)
			phase_1_flag<=1'b1;
		else if(phase_1_flag==1'b1&&phase_1_cnt=='d9&&scl_nege_flag==1'b1)
			phase_1_flag<=1'b0;
		
always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
			phase_1_cnt<='d0;
		else if(phase_1_flag==1'b1&&scl_pose_flag==1'b1)
			phase_1_cnt<=phase_1_cnt+1'b1;
		else if(phase_1_flag==1'b0)
			phase_1_cnt<='d0;
//相位2	
always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
			phase_2_r<='d0;
		else if(phase_1_flag==1'b1&&phase_1_cnt=='d9&&scl_nege_flag==1'b1)
			phase_2_r<=phase_2;
		else if(phase_2_flag==1'b1&&sda_nege_flag==1'b1)
			phase_2_r<={phase_2_r[6:0],1'b1};//移位操作
		
always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
			phase_2_flag<=1'b0;
		else if(phase_1_flag==1'b1&&phase_1_cnt=='d9&&scl_nege_flag==1'b1)	
			phase_2_flag<=1'b1;
		else if(phase_2_flag==1'b1&&phase_2_cnt=='d9&&scl_nege_flag==1'b1)	
			phase_2_flag<=1'b0;

always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
			phase_2_cnt<='d0;
		else if(phase_2_flag==1'b1&&scl_pose_flag==1'b1)
			phase_2_cnt<=phase_2_cnt+1'b1;
		else if(phase_2_flag==1'b0)
			phase_2_cnt<='d0;
//相位3			
always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
			phase_3_r<='d0;
		else if(phase_2_flag==1'b1&&phase_2_cnt=='d9&&scl_nege_flag==1'b1)
			phase_3_r<=phase_3;
		else if(phase_3_flag==1'b1&&sda_nege_flag==1'b1)
			phase_3_r<={phase_3_r[6:0],1'b1};//移位操作

always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
			phase_3_flag<=1'b0;
		else if(phase_2_flag==1'b1&&phase_2_cnt=='d9&&scl_nege_flag==1'b1)	
			phase_3_flag<=1'b1;
		else if(phase_3_flag==1'b1&&phase_3_cnt=='d9&&scl_nege_flag==1'b1)	
			phase_3_flag<=1'b0;

always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
			phase_3_cnt<='d0;
		else if(phase_3_flag==1'b1&&scl_pose_flag==1'b1)
			phase_3_cnt<=phase_3_cnt+1'b1;
		else if(phase_3_flag==1'b0)
			phase_3_cnt<='d0;

//stop state
always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
			w_end_flag<=1'b0;
		else if(phase_3_flag==1'b1&&phase_3_cnt=='d9&&scl_nege_flag==1'b1)	
			w_end_flag<=1'b1;
		else if(w_end_flag==1'b1&&scl_nege_flag==1'b1)
			w_end_flag<=1'b0;			

always @(posedge axi_clk or negedge axi_rst_n)
		if(axi_rst_n==1'b0)
			write_end_r[0]<=1'b0;
		else if(write_flag_r[0]==1'b1)
		    write_end_r[0]<=1'b0;
		else if(w_end_flag==1'b1&&scl_nege_flag==1'b1)
			write_end_r[0]<=1'b1;		
		
			
always @(posedge axi_clk or negedge axi_rst_n)//ʹ��д3flag����ʵ��ѭ��������
		if(axi_rst_n==1'b0)
		    write_flag_r[0]<=1'b0;
		else if(w_3phase_flag==1'b0)
		    write_flag_r[0]<=1'b1;
		else write_flag_r[0]<=1'b0;
		
always @(posedge axi_clk or negedge axi_rst_n)
        if(axi_rst_n==1'b0)		
            en<=1'b0;
        else if(phase_1_flag==1'b1&&phase_1_cnt=='d8&&sda_pose_flag==1'b1)
            en<=1'b1;
        else if(phase_2_flag==1'b1&&phase_2_cnt=='d0&&sda_pose_flag==1'b1)
            en<=1'b0;
        else if(phase_2_flag==1'b1&&phase_2_cnt=='d8&&sda_pose_flag==1'b1)
            en<=1'b1;
        else if(phase_3_flag==1'b1&&phase_3_cnt=='d0&&sda_pose_flag==1'b1)
            en<=1'b0;    
        else if(phase_3_flag==1'b1&&phase_3_cnt=='d8&&sda_pose_flag==1'b1)
            en<=1'b1;
		else if(w_end_flag==1'b1&&sda_pose_flag==1'b1)
		    en<=1'b0;

always @(posedge axi_clk or negedge axi_rst_n)
        if(axi_rst_n==1'b0)	
            write_data_r<='d0;
        else
            write_data_r<=WR_DATA_0;
        
        
        
        
        
endmodule