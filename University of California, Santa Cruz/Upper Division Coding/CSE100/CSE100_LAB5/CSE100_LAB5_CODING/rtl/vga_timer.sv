// Copyright (c) 2024 Ethan Sifferman.



// All rights reserved. Distribution Prohibited.

// https://vesa.org/vesa-standards/



// http://tinyvga.com/vga-timing

// code - 6744



module vga_timer (
    input  logic       clk_i,
    input  logic       rst_ni,
    output logic       hsync_o,
    output logic       vsync_o,
    output logic       visible_o,
    output logic [9:0] position_x_o,
    output logic [9:0] position_y_o

);

    logic [9:0] hCounter;
    localparam int visibleH = 640;
    localparam int hFrontPorch = 16;
    localparam int hSyncPulse = 96;
    localparam int hBackPorch = 48;
    localparam int row = visibleH + hFrontPorch +
                         hSyncPulse + hBackPorch;
    always_ff@(posedge clk_i) begin
        if(!rst_ni) begin
            hCounter <= 10'd0;
        end
        else if(hCounter == row-1) begin
            hCounter <= 10'd0;
        end
        else begin
            hCounter <= hCounter + 10'd1;
        end
    end
    logic [9:0] vCounter;
    localparam int visibleV = 480;
    localparam int vFrontPorch = 10;
    localparam int vSyncPulse = 2;
    localparam int vBackPorch = 33;
    localparam int frame = visibleV + vFrontPorch +
                           vSyncPulse + vBackPorch;
    always_ff@(posedge clk_i) begin
        if(!rst_ni) begin
            vCounter <= 10'd0;
        end
        else if(hCounter == row-1)begin
            if(vCounter == frame-1) begin
                vCounter <= 10'd0;
            end
            else begin
                vCounter <= vCounter + 10'd1;
            end
        end
    end
    always_ff@(posedge clk_i) begin
        if(!rst_ni) begin
            hsync_o <= 1'b1;
        end
        else if(hCounter >= ((visibleH + hFrontPorch)) &&
        hCounter < ((visibleH + hFrontPorch +
        hSyncPulse))) begin
            hsync_o <= 1'b0;
        end
        else begin
            hsync_o <= 1'b1;
        end
    end
    always_ff@(posedge clk_i) begin
        if(!rst_ni) begin
            vsync_o <= 1'b1;
        end
        else if(vCounter >= ((visibleV + vFrontPorch)) &&
        vCounter < (visibleV + vFrontPorch + vSyncPulse)) begin
             vsync_o <= 1'b0;
        end
        else begin
            vsync_o <= 1'b1;
        end
    end
    always_comb begin
        if(!rst_ni) begin
            visible_o = 1'b0;
            position_x_o = 10'd0;
            position_y_o = 10'd0;
        end
        else begin
            visible_o = (hCounter <= visibleH &&
            vCounter < visibleV);
            position_x_o = hCounter;
            position_y_o = vCounter;
        end
    end
endmodule
