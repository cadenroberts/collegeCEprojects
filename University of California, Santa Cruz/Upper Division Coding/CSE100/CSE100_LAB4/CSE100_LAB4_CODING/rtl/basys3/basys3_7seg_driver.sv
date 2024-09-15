// Copyright (c) 2024 Ethan Sifferman.
// All rights reserved. Distribution Prohibited.

module basys3_7seg_driver (
    input              clk_1k_i,
    input              rst_ni,

    input  logic       digit0_en_i,
    input  logic [3:0] digit0_i,
    input  logic       digit1_en_i,
    input  logic [3:0] digit1_i,
    input  logic       digit2_en_i,
    input  logic [3:0] digit2_i,
    input  logic       digit3_en_i,
    input  logic [3:0] digit3_i,

    output logic [3:0] anode_o,
    output logic [6:0] segments_o
);

// TODO
// Segment decoder instance
logic [1:0] sel;
logic a0, b0, c0, d0, e0, f0, g0;
logic a1, b1, c1, d1, e1, f1, g1;
logic a2, b2, c2, d2, e2, f2, g2;
logic a3, b3, c3, d3, e3, f3, g3;

// Instantiate segment decoders
hex7seg h7seg0 (.d3(digit0_i[3]), .d2(digit0_i[2]), .d1(digit0_i[1]), .d0(digit0_i[0]),
.A(a0), .B(b0), .C(c0), .D(d0), .E(e0), .F(f0), .G(g0));
hex7seg h7seg1 (.d3(digit1_i[3]), .d2(digit1_i[2]), .d1(digit1_i[1]), .d0(digit1_i[0]),
.A(a1), .B(b1), .C(c1), .D(d1), .E(e1), .F(f1), .G(g1));
hex7seg h7seg2 (.d3(digit2_i[3]), .d2(digit2_i[2]), .d1(digit2_i[1]), .d0(digit2_i[0]),
.A(a2), .B(b2), .C(c2), .D(d2), .E(e2), .F(f2), .G(g2));
hex7seg h7seg3 (.d3(digit3_i[3]), .d2(digit3_i[2]), .d1(digit3_i[1]), .d0(digit3_i[0]),
.A(a3), .B(b3), .C(c3), .D(d3), .E(e3), .F(f3), .G(g3));

always_ff @(posedge clk_1k_i) begin
    if (!rst_ni)
        sel <= 2'b00;
    else
        sel <= sel + 1;
end

always_comb begin
    case (sel)
        2'b00: begin
            anode_o = 4'b1110; // Select the first digit
            if (digit0_en_i) begin
                segments_o = {a0, b0, c0, d0, e0, f0, g0};
            end
            segments_o = {a0, b0, c0, d0, e0, f0, g0};
        end
        2'b01: begin
            anode_o = 4'b1101; // Select the second digit
            if (digit1_en_i) begin
                segments_o = {a1, b1, c1, d1, e1, f1, g1};
            end
            segments_o = {a1, b1, c1, d1, e1, f1, g1};
        end
        2'b10: begin
            anode_o = 4'b1011; // Select the third digit
            if (digit2_en_i) begin
                segments_o = {a2, b2, c2, d2, e2, f2, g2};
            end
            segments_o = {a2, b2, c2, d2, e2, f2, g2};
        end
        2'b11: begin
            anode_o = 4'b0111; // Select the fourth digit
            if (digit3_en_i) begin
                segments_o = {a3, b3, c3, d3, e3, f3, g3};
            end
            segments_o = {a3, b3, c3, d3, e3, f3, g3};
        end
        default: begin
            anode_o = 4'b1111; // All digits are off
            segments_o = 7'b1111111; // All segments are off
        end
    endcase
end
//
endmodule

