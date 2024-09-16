module alu (
    input  logic [7:0] a_i,
    input  logic [7:0] b_i,
    input  logic [3:0] operation_i,
    output logic [7:0] y_o
);
    logic [7:0] flog2_result;
    logic [7:0] sqrt_result;
    flog2 flog2_inst (
        .x_i(a_i),
        .y_o(flog2_result)
    );
    sqrt sqrt_inst (
        .x_i({24'b0, a_i}),
        .y_o(sqrt_result)
    );
    always_comb begin
        case (operation_i)
            4'b0001: y_o = a_i + b_i;              // ADD
            4'b0010: y_o = a_i - b_i;              // SUB
            4'b0100: y_o = flog2_result;    // FLOG2
            4'b1000: y_o = sqrt_result;             // SQRT
            default: y_o = 8'b0;             // Default
        endcase
    end
endmodule

//2894
