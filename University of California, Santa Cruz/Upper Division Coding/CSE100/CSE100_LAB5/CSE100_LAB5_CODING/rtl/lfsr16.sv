// Copyright (c) 2024 Ethan Sifferman.
// All rights reserved. Distribution Prohibited.

module lfsr16 (
    input  logic       clk_i,
    input  logic       rst_ni,

    input  logic       next_i,
    output logic [15:0] rand_o
);

// TODO
logic [15:0] lfsr_q, lfsr_d;

// Feedback taps
logic feedback;

always_ff @(posedge clk_i) begin
    if (!rst_ni)
        lfsr_q <= 16'hFFFF; // Initialize to non-zero value
    else if (next_i)
        lfsr_q <= lfsr_d;
end

// LFSR implementation with taps at positions 8, 7, 6, and 1
assign feedback = lfsr_q[15] ^ lfsr_q[14] ^ lfsr_q[12] ^ lfsr_q[3];

always_comb begin
    lfsr_d = {lfsr_q[14:0], feedback};
end

// Output the 5 least significant bits
assign rand_o = lfsr_q;

endmodule
