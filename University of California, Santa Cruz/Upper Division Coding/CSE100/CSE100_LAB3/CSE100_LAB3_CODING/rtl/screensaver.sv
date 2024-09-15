// Copyright (c) 2024 Ethan Sifferman.
// All rights reserved. Distribution Prohibited.
// code - 6744
module screensaver (
    input  logic       clk_i,
    input  logic       rst_ni,
    input  logic [3:0] select_image_i,
    output logic [3:0] vga_red_o,
    output logic [3:0] vga_blue_o,
    output logic [3:0] vga_green_o,
    output logic       vga_hsync_o,
    output logic       vga_vsync_o
);
localparam int IMAGE_WIDTH = 160;
localparam int IMAGE_HEIGHT = 120;
localparam int IMAGE_ROM_SIZE = (IMAGE_WIDTH * IMAGE_HEIGHT);
logic [$clog2(IMAGE_ROM_SIZE)-1:0] rom_addr;
logic [11:0] image0_rdata;
logic [11:0] image1_rdata;
logic [11:0] image2_rdata;
logic [11:0] image3_rdata;
logic [9:0] x_o;
logic [9:0] y_o;
logic visible;
vga_timer timer(
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .hsync_o(vga_hsync_o),
    .vsync_o(vga_vsync_o),
    .visible_o(visible),
    .position_x_o(x_o),
    .position_y_o(y_o)
);
assign rom_addr = (y_o[9:2] * IMAGE_WIDTH) + x_o[9:2];
images #(
    .IMAGE_ROM_SIZE(IMAGE_ROM_SIZE)
) images (
    .clk_i(clk_i),
    .rom_addr_i(rom_addr),
    .image0_rdata_o(image0_rdata),
    .image1_rdata_o(image1_rdata),
    .image2_rdata_o(image2_rdata),
    .image3_rdata_o(image3_rdata)
);
logic [1:0] current_image;
logic [11:0] currentImageData;
always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
        current_image <= 2'b00;
    end else begin
        case (select_image_i)
            4'b0001: current_image <= 2'b00;
            4'b0010: current_image <= 2'b01;
            4'b0100: current_image <= 2'b10;
            4'b1000: current_image <= 2'b11;
            default: current_image <= current_image; // Keep using the same image
        endcase
    end
end
always_comb begin
    case (current_image)
        2'b00: currentImageData = image0_rdata;
        2'b01: currentImageData = image1_rdata;
        2'b10: currentImageData = image2_rdata;
        2'b11: currentImageData = image3_rdata;
        default: currentImageData = 12'h000;
    endcase
end
always_comb begin
    if (!rst_ni) begin
        vga_red_o = 4'b0000;
        vga_blue_o = 4'b0000;
        vga_green_o = 4'b0000;
    end
    if(!visible) begin
        vga_red_o = 4'b0000;
        vga_blue_o = 4'b0000;
        vga_green_o = 4'b0000;
    end
    else  begin
        vga_red_o = currentImageData[11:8];
        vga_green_o = currentImageData[7:4];
        vga_blue_o = currentImageData[3:0];
    end
end


endmodule
