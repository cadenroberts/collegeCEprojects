// Copyright (c) 2024 Ethan Sifferman.
// All rights reserved. Distribution Prohibited.

module tb;

logic d3,d2,d1,d0;
logic A,B,C,D,E,F,G;

hex7seg hex7seg(
    .d3,
    .d2,
    .d1,
    .d0,

    .A,
    .B,
    .C,
    .D,
    .E,
    .F,
    .G
);

initial begin
    $dumpfile( "dump.fst" );
    $dumpvars;
    $display( "Begin simulation." );

    for (int i = 0; i < 16; i++) begin
        {d3,d2,d1,d0} = 4'(i);
        #1;
    end

    $display( "End simulation." );
    $finish;
end

always @* begin
    unique case ({d3,d2,d1,d0})
    4'h0: assert ({A,B,C,D,E,F,G} == 7'b1111110) else $error("Assertion failed: Wrong result for 0");
    4'h1: assert ({A,B,C,D,E,F,G} == 7'b0110000) else $error("Assertion failed: Wrong result for 1");
    4'h2: assert ({A,B,C,D,E,F,G} == 7'b1101101) else $error("Assertion failed: Wrong result for 2");
    4'h3: assert ({A,B,C,D,E,F,G} == 7'b1111001) else $error("Assertion failed: Wrong result for 3");
    4'h4: assert ({A,B,C,D,E,F,G} == 7'b0110011) else $error("Assertion failed: Wrong result for 4");
    4'h5: assert ({A,B,C,D,E,F,G} == 7'b1011011) else $error("Assertion failed: Wrong result for 5");
    4'h6: assert ({A,B,C,D,E,F,G} == 7'b1011111) else $error("Assertion failed: Wrong result for 6");
    4'h7: assert ({A,B,C,D,E,F,G} == 7'b1110000) else $error("Assertion failed: Wrong result for 7");
    4'h8: assert ({A,B,C,D,E,F,G} == 7'b1111111) else $error("Assertion failed: Wrong result for 8");
    4'h9: assert ({A,B,C,D,E,F,G} == 7'b1111011) else $error("Assertion failed: Wrong result for 9");
    4'ha: assert ({A,B,C,D,E,F,G} == 7'b1110111) else $error("Assertion failed: Wrong result for a");
    4'hb: assert ({A,B,C,D,E,F,G} == 7'b0011111) else $error("Assertion failed: Wrong result for b");
    4'hc: assert ({A,B,C,D,E,F,G} == 7'b1001110) else $error("Assertion failed: Wrong result for c");
    4'hd: assert ({A,B,C,D,E,F,G} == 7'b0111101) else $error("Assertion failed: Wrong result for d");
    4'he: assert ({A,B,C,D,E,F,G} == 7'b1001111) else $error("Assertion failed: Wrong result for e");
    4'hf: assert ({A,B,C,D,E,F,G} == 7'b1000111) else $error("Assertion failed: Wrong result for f");
    endcase
end

endmodule
