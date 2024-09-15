// Copyright (c) 2024 Ethan Sifferman.
// All rights reserved. Distribution Prohibited.
module stop_it import stop_it_pkg::*; (
    input  logic        rst_ni,
    input  logic        clk_4_i,
    input  logic        go_i,
    input  logic        stop_i,
    input  logic        load_i,
    input  logic [15:0] switches_i,
    output logic [15:0] leds_o,
    output logic        digit0_en_o,
    output logic [3:0]  digit0_o,
    output logic        digit1_en_o,
    output logic [3:0]  digit1_o,
    output logic        digit2_en_o,
    output logic [3:0]  digit2_o,
    output logic        digit3_en_o,
    output logic [3:0]  digit3_o
);
logic [4:0] targetNumber;
logic shiftEnable;
lfsr lfsr(
    .clk_i(clk_4_i),
    .rst_ni(rst_ni),
    .next_i(shiftEnable),
    .rand_o(targetNumber)
);
logic [4:0] gameCount;
//active low game reset
logic gameReset_ni;
logic gameEnable;
game_counter game_counter(
    .clk_4_i(clk_4_i),
    .rst_ni(!gameReset_ni),
    .en_i(gameEnable),
    .count_o(gameCount)
);
logic [4:0] timeCount;
//active low time reset
logic timeReset_ni;
logic timeEnable;
time_counter time_counter(
    .clk_4_i(clk_4_i),
    .rst_ni(!timeReset_ni),
    .en_i(timeEnable),
    .count_o(timeCount)
);
logic [15:0] leds;
led_shifter led_shifter(
    .clk_i(clk_4_i),
    .rst_ni(rst_ni),
    .shift_i(getsPoint),
    .switches_i(switches_i),
    .load_i(load_i),
    //turns off every odd timeCount value
    .off_i(blink && timeCount[0]),
    .leds_o(leds)
);
state_t state_d, state_q;
always_ff @(posedge clk_4_i) begin
    if (!rst_ni) begin
        state_q <= WAITING_TO_START;
    end else begin
        state_q <= state_d;
    end
end
assign leds_o = leds;
/*verilator lint_off UNOPTFLAT*/
logic getsPoint;
/*verilator lint_off UNOPTFLAT*/
logic blink;
always_comb begin
    state_d = state_q;
    timeEnable = 1'b0;
    gameEnable = 1'b0;
    timeReset_ni = 1'b0;
    gameReset_ni = 1'b0;
    getsPoint = 1'b0;
    shiftEnable = 1'b1;
    unique case (state_q)
        WAITING_TO_START: begin
            timeReset_ni = 1'b1;
            timeEnable = 1'b0;
            gameReset_ni = 1'b1;
            if(go_i) begin
                timeReset_ni = 1'b0;
                timeEnable = 1'b1;
                //shiftEnable = 1'b1;
                state_d = STARTING;
            end
            else begin
                state_d = WAITING_TO_START;
            end
        end
        STARTING: begin
            //no reset
            timeReset_ni = 1'b0;
            //shiftEnable = 1'b0;
            shiftEnable = 1'b0;
            timeEnable = 1;
            //if paused for 2 seconds
            if(timeCount == 8) begin
                gameReset_ni = 1'b0;
                timeReset_ni = 1'b1;
                state_d = DECREMENTING;
            end
            else begin
                state_d = STARTING;
            end
        end
        DECREMENTING: begin
            gameEnable = 1'b1;
            shiftEnable = 1'b0;
            if(stop_i) begin
                if(targetNumber == gameCount) begin
                    timeEnable = 1'b1;
                    timeReset_ni = 1'b0;
                    gameEnable = 1'b0;
                    gameReset_ni = 1'b0;
                    getsPoint = 1'b0;
                    state_d = CORRECT;
                end
                else begin
                    timeEnable = 1'b1;
                    timeReset_ni = 1'b0;
                    gameEnable = 1'b0;
                    gameReset_ni = 1'b0;
                    state_d = WRONG;
                end
            end
            else begin
                gameEnable = 1'b1;
                state_d = DECREMENTING;
            end
        end
        WRONG: begin
            timeEnable = 1'b1;
            shiftEnable = 1'b0;
            //no reset
            timeReset_ni = 1'b0;
            //if 4 seconds pass
            if(timeCount == 16) begin
                timeEnable = 1'b0;
                timeReset_ni = 1'b1;
                gameEnable = 1'b0;
                gameReset_ni = 1'b0;
                state_d = WAITING_TO_START;
            end
            else begin
                state_d = WRONG;
            end
        end
        CORRECT: begin
            timeEnable = 1'b1;
            shiftEnable = 1'b0;
            timeReset_ni = 1'b0;
            getsPoint = 1'b0;
            //if 4 seconds passed
            if(timeCount == 16) begin
                if(leds == 16'b1111111111111111) begin
                    state_d = WON;
                end
                else begin
                    timeEnable = 1'b0;
                    getsPoint = 1'b1;
                    timeReset_ni = 1'b1;
                    gameEnable = 1'b0;
                    gameReset_ni = 1'b0;
                    state_d = WAITING_TO_START;
                end
            end
            else begin
                state_d = CORRECT;
            end
        end
        WON: begin
            timeEnable = 1'b1;
            gameEnable = 1'b0;
            shiftEnable = 1'b0;
            timeReset_ni = 1'b0;
            gameReset_ni = 1'b0;
            state_d = WON;
        end
        default: begin
            state_d = WAITING_TO_START;
        end
    endcase
end
logic d0, d1, d2, d3;
always_comb begin
    if(state_q == WON) begin
        blink = 1'b1;
    end
    else begin
        blink = 1'b0;
    end
    digit0_o = gameCount[3:0];
    digit1_o = {3'b000, gameCount[4]};
    digit2_o = targetNumber[3:0];
    digit3_o = {3'b000, targetNumber[4]};
    //d0 d1 enables
    d0 = ((state_q == WAITING_TO_START) |
    (state_q == DECREMENTING) |
    (state_q == STARTING) |
    ((state_q == CORRECT) && timeCount[0]) |
    ((state_q == WRONG) && timeCount[0]) |
    (state_q == WON));
    d1 = ((state_q == WAITING_TO_START) |
    (state_q == DECREMENTING) |
    (state_q == STARTING) |
    ((state_q == CORRECT) && timeCount[0]) |
    ((state_q == WRONG) && timeCount[0]) |
    (state_q == WON));
    //d2 and d3 enables
    d2 = ((state_q == STARTING) |
    (state_q == DECREMENTING) |
    ((state_q == CORRECT) && timeCount[0]) |
    ((state_q == WRONG) && !timeCount[0]) |
    (state_q == WON));
    d3 = ((state_q == STARTING) |
    (state_q == DECREMENTING) |
    ((state_q == CORRECT) && timeCount[0]) |
    ((state_q == WRONG) && !timeCount[0]) |
    (state_q == WON));
    digit0_en_o = d0;
    digit1_en_o = d1;
    digit2_en_o = d2;
    digit3_en_o = d3;
end
endmodule

