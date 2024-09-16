// Copyright (c) 2024 Ethan Sifferman.
// All rights reserved. Distribution Prohibited.

module dinorun import dinorun_pkg::*; (
    input  logic       clk_25_175_i,
    input  logic       rst_ni,

    input  logic       start_i,
    input  logic       up_i,
    input  logic       down_i,

    output logic       digit0_en_o,
    output logic [3:0] digit0_o,
    output logic       digit1_en_o,
    output logic [3:0] digit1_o,
    output logic       digit2_en_o,
    output logic [3:0] digit2_o,
    output logic       digit3_en_o,
    output logic [3:0] digit3_o,

    output logic [3:0] vga_red_o,
    output logic [3:0] vga_green_o,
    output logic [3:0] vga_blue_o,
    output logic       vga_hsync_o,
    output logic       vga_vsync_o
);

// TODO
// VGA Timer instance
logic [9:0] pixel_x;
logic [9:0] pixel_y;
logic next_frame;
logic is_visible;
logic isFrozen;

vga_timer vga_inst (
    .clk_i(clk_25_175_i),
    .rst_ni(rst_ni),
    .position_x_o(pixel_x),
    .position_y_o(pixel_y),
    .hsync_o(vga_hsync_o),
    .vsync_o(vga_vsync_o),
    .visible_o(is_visible)
);

// Edge Detector for next_frame signal
edge_detector edge_det_inst (
    .clk_i(clk_25_175_i),
    .data_i(vga_vsync_o),
    .edge_o(next_frame)
);

// LFSR instance for random number generation
logic [15:0] rand_val;

lfsr16 lfsr_inst (
    .clk_i(clk_25_175_i),
    .rst_ni(rst_ni),
    .next_i(next_frame),
    .rand_o(rand_val)
);

logic dino_pixel;
logic bird_pixel;
logic cactus_pixel;
logic title_pixel;
logic title_screen;
assign title_screen = (state == TITLE);
// Score Counter instance
score_counter score_counter_inst (
    .clk_i(clk_25_175_i),
    .rst_ni(rst_ni),
    .en_i(next_frame && !title_screen && !isFrozen),
    .digit0_o(digit0_o),
    .digit1_o(digit1_o),
    .digit2_o(digit2_o),
    .digit3_o(digit3_o)
);

assign digit0_en_o = 1'b1;
assign digit1_en_o = 1'b1;
assign digit2_en_o = 1'b1;
assign digit3_en_o = 1'b1;

state_t state, next_state;

logic collision;

assign collision = dino_pixel && (bird_pixel || cactus_pixel);

always_ff @(posedge clk_25_175_i) begin
    if (!rst_ni) begin
        state <= TITLE;
    end else begin
        state <= next_state;
    end
end

always_comb begin
    next_state = state;
    unique case (state)
        TITLE: begin
            if (start_i) next_state = PLAYING;
            isFrozen = 0;
        end
        PLAYING: begin
            if (collision) next_state = HIT;
        end
        HIT: begin
            if (start_i) next_state = TITLE;
            isFrozen = 1;
        end
    endcase
end

// Dino instance

dino dino_inst (
    .clk_i(clk_25_175_i),
    .rst_ni(rst_ni),
    .next_frame_i(next_frame),
    .up_i(up_i),
    .down_i(down_i),
    .hit_i(state == HIT),
    .pixel_x_i(pixel_x),
    .pixel_y_i(pixel_y),
    .pixel_o(dino_pixel)
);

// Bird instance

bird bird_inst (
    .clk_i(clk_25_175_i),
    .rst_ni(rst_ni),
    .next_frame_i(next_frame && !isFrozen),
    .spawn_i(rand_val[15:12] == 4'b0000),
    .rand_i(rand_val[1:0]),
    .pixel_x_i(pixel_x),
    .pixel_y_i(pixel_y),
    .pixel_o(bird_pixel)
);

// Cactus instance

cactus cactus_inst (
    .clk_i(clk_25_175_i),
    .rst_ni(rst_ni),
    .next_frame_i(next_frame && !isFrozen),
    .spawn_i(rand_val[15:12] == 4'b0001),
    .rand_i(rand_val[1:0]),
    .pixel_x_i(pixel_x),
    .pixel_y_i(pixel_y),
    .pixel_o(cactus_pixel)
);

title title_inst (
    .pixel_x_i(pixel_x),
    .pixel_y_i(pixel_y),
    .pixel_o(title_pixel)
);

// VGA Output
/* verilator lint_off UNDRIVEN */
logic [3:0] vga_red_oq, vga_red_od, vga_blue_oq, vga_blue_od,
            vga_green_oq, vga_green_od, vga_hsync_od, vga_hsync_oq, vga_vsync_od, vga_vsync_oq;
/* verilator lint_on UNDRIVEN */
always_ff @(posedge clk_25_175_i) begin
    if (!rst_ni) begin
        vga_red_oq <= 4'h0;
        vga_green_oq <= 4'h0;
        vga_blue_oq <= 4'h0;
        vga_hsync_oq <= 0;
        vga_vsync_oq <= 0;
    end else begin
        vga_red_oq <= vga_red_od;
        vga_green_oq <= vga_green_od;
        vga_blue_oq <= vga_blue_od;
        vga_hsync_oq <= vga_hsync_od;
        vga_vsync_oq <= vga_vsync_od;
    end
end

logic floor;

always_comb begin
    vga_red_o = vga_red_oq;
    vga_blue_o = vga_blue_oq;
    vga_green_o = vga_green_oq;
    floor = (pixel_y > 400);
    //vga_hsync_o = vga_hsync_oq;
    //vga_vsync_o = vga_hsync_oq;
end


always_comb begin
    vga_red_od = vga_red_o;
    vga_green_od = vga_green_o;
    vga_blue_od = vga_blue_o;
    if (is_visible) begin
        if (title_screen) begin
            vga_red_od = (title_pixel || dino_pixel) ? 4'hF : 4'h0;
            vga_green_od = (title_pixel || floor) ? 4'hF : 4'h0;
            vga_blue_od = title_pixel ? 4'hF : 4'h0;
        end else if (dino_pixel) begin
            vga_red_od = 4'hF;
            vga_green_od = 4'h0;
            vga_blue_od = 4'h0;
        end else if (bird_pixel) begin
            vga_red_od = 4'h0;
            vga_green_od = 4'h0;
            vga_blue_od = 4'hF;
        end else if (cactus_pixel) begin
            vga_red_od = 4'h0;
            vga_green_od = 4'hF;
            vga_blue_od = 4'h0;
        end else begin
            vga_red_od = 4'h0;
            vga_green_od = floor ? 4'hF : 4'h0;
            vga_blue_od = 4'h0;
        end
    end else begin
        vga_red_od = 4'h0;
        vga_green_od = 4'h0;
        vga_blue_od = 4'h0;
    end
end

endmodule
