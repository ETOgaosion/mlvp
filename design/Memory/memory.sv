/* from https://github.com/chenyangbing/UVM-example/blob/master/memory.sv */

/*
              -----------------
              |               |
    addr ---->|               |
              |               |------> rdata
              | Memory Model  |
   wdata ---->|               |
              |               | 
              -----------------
                   ^     ^
                   |     |
                wr_en  rd_en

-------------------------------------------------------------------------- */

module memory
  #( parameter ADDR_WIDTH = 2,
     parameter DATA_WIDTH = 8 ) (
    input clk,
    input reset,
    
    //control signals
    input [ADDR_WIDTH-1:0]  addr,
    input                   wr_en,
    input                   rd_en,
    
    //data signals
    input  [DATA_WIDTH-1:0] wdata,
    output reg [DATA_WIDTH-1:0] rdata
  ); 
  
  //Memory
  reg [DATA_WIDTH-1:0] mem [2**ADDR_WIDTH];

  //Reset 
  always @(posedge reset) 
    for(int i=0;i<2**ADDR_WIDTH;i++) mem[i]=8'h00;
   
  //!< Write data to Memory
  always @(posedge clk) 
    if (reset)
      for(int i=0;i<2**ADDR_WIDTH;i++) mem[i]=8'h00;
    else
    begin
      if (wr_en)    mem[addr] <= wdata;
      if (rd_en)    rdata <= mem[addr];
    end

endmodule