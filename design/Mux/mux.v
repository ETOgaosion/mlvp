/* from https://github.com/sugureshkumar/2-1-Mux-Verification-using-UVM/blob/master/mux.v */

module mux(out,sel,a,b);
    input a,b;
    input sel;
    output reg out;

    always@(a or b or sel) begin
        if(sel == 1'b1) begin
            out = a;
        end
        else
        begin
            out = b;
        end
    end
endmodule