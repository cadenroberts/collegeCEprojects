// Copyright (c) 2024 Ethan Sifferman.
// All rights reserved. Distribution Prohibited.

module lfsr (
    input  logic       clk_i,
    input  logic       rst_ni,

    input  logic       next_i,
    output logic [4:0] rand_o
);

// TODO
logic [7:0] lfsr_q, lfsr_d;

// Feedback taps
logic feedback;

always_ff @(posedge clk_i) begin
    if (!rst_ni)
        lfsr_q <= 8'hFF; // Initialize to non-zero value
    else if (next_i)
        lfsr_q <= lfsr_d;
end

// LFSR implementation with taps at positions 8, 7, 6, and 1
assign feedback = lfsr_q[7] ^ lfsr_q[6] ^ lfsr_q[5] ^ lfsr_q[0];

always_comb begin
    lfsr_d = {lfsr_q[6:0], feedback};
end

// Output the 5 least significant bits
assign rand_o = lfsr_q[4:0];
//

endmodule
