// Copyright (c) 2024 Ethan Sifferman.
// All rights reserved. Distribution Prohibited.

module led_shifter (
    input  logic        clk_i,
    input  logic        rst_ni,

    input  logic        shift_i,

    input  logic [15:0] switches_i,
    input  logic        load_i,

    input  logic        off_i,
    output logic [15:0] leds_o
);

// TODO
logic [15:0] led_reg;

always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
        led_reg <= 16'h0000;
    end else if (load_i) begin
        led_reg <= switches_i;
    end else if (shift_i) begin
        led_reg <= {led_reg[14:0], 1'b1};
    end
end

assign leds_o = off_i ? 16'h0000 : led_reg;

//
endmodule
