// Copyright (c) 2024 Ethan Sifferman.
// All rights reserved. Distribution Prohibited.

module tb;

logic [7:0] a_i;
logic [7:0] b_i;
logic [3:0] operation_i;

localparam logic [3:0] ADD   = 4'b0001;
localparam logic [3:0] SUB   = 4'b0010;
localparam logic [3:0] FLOG2 = 4'b0100;
localparam logic [3:0] SQRT  = 4'b1000;

alu alu(
    .a_i,
    .b_i,
    .operation_i
);

initial begin : sim
    $dumpfile( "dump.fst" );
    $dumpvars;
    $display( "Begin simulation." );

    operation_i = ADD;
    for (int i = 0; i < 64; i++) begin
        a_i = 8'($random);
        b_i = 8'($random);
        #1ns;
    end
    operation_i = SUB;
    for (int i = 0; i < 64; i++) begin
        a_i = 8'($random);
        b_i = 8'($random);
        #1ns;
    end
    operation_i = FLOG2;
    for (int i = 0; i < 64; i++) begin
        a_i = 8'($random);
        b_i = 'x;
        #1ns;
    end
    operation_i = SQRT;
    for (int i = 0; i < 64; i++) begin
        a_i = 8'($random);
        b_i = 'x;
        #1ns;
    end

    $display( "End simulation." );
    $finish;
end

function automatic int flog2_model(int in);
    real log2 = $ln($itor(a_i))/$ln(2);
    return $rtoi($floor(log2));
endfunction

function automatic int sqrt_model(int in);
    real sqrt = $sqrt($itor(in));
    return int'(sqrt);
endfunction

always @* begin : assertions
    unique case (alu.operation_i)
    ADD:   assert (       a_i + b_i == alu.y_o) else $error("Wrong result for %0d + %0d", a_i, b_i);
    SUB:   assert (       a_i - b_i == alu.y_o) else $error("Wrong result for %0d - %0d", a_i, b_i);
    FLOG2: assert (flog2_model(a_i) == alu.y_o) else $error("Wrong result for flog2(%0d)", a_i);
    SQRT:  assert ( sqrt_model(a_i) == alu.y_o) else $error("Wrong result for sqrt(%0d)", a_i);
    default: ;
    endcase
end

endmodule
