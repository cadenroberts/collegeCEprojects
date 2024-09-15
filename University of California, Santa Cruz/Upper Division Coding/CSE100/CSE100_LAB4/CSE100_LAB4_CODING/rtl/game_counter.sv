// Copyright (c) 2024 Ethan Sifferman.
// All rights reserved. Distribution Prohibited.

module game_counter (
    input  logic       clk_4_i,
    input  logic       rst_ni,
    input  logic       en_i,
    output logic [4:0] count_o
);

// TODO
always_ff @(posedge clk_4_i) begin
    if (!rst_ni)
        count_o <= 5'h1F;
    else if (en_i) begin
        if (count_o == 0) begin
            count_o <= 5'h1F;
        end else begin
            count_o <= count_o - 1;
        end
    end
end
//
endmodule
