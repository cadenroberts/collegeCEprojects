module sqrt (
    input [31:0] x_i,
    output reg [7:0] y_o
);
    reg [7:0] sqrt_lut [256];
    initial begin
        $readmemh("sqrt.memh", sqrt_lut);
    end
    always_comb begin
        y_o = sqrt_lut[x_i[7:0]];
    end
endmodule
