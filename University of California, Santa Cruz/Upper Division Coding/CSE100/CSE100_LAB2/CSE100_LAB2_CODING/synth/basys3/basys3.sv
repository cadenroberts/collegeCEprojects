module basys3 (
    input  wire [15:0] sw,      // 16 switches
    input  wire [3:0] btn,      // 4 directional buttons
    output wire [7:0] led       // 8 LEDs
);
    wire [7:0] a_i = sw[7:0];    // Bottom 8 switches for alu.a_i
    wire [7:0] b_i = sw[15:8];   // Top 8 switches for alu.b_i
    wire [3:0] operation_i = btn; // 4 buttons for alu.operation_i
    wire [7:0] y_o;
    alu my_alu (
        .a_i(a_i),
        .b_i(b_i),
        .operation_i(operation_i),
        .y_o(y_o)
    );
    assign led = y_o;
endmodule

