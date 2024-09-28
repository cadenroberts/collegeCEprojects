module hex7seg(
    input  logic d3, d2, d1, d0,
    output logic A, B, C, D, E, F, G
);

    always_comb begin
        case ({d3, d2, d1, d0})
            4'h0: {A, B, C, D, E, F, G} = 7'b1000000; // 0
            4'h1: {A, B, C, D, E, F, G} = 7'b1111001; // 1
            4'h2: {A, B, C, D, E, F, G} = 7'b0100100; // 2
            4'h3: {A, B, C, D, E, F, G} = 7'b0110000; // 3
            4'h4: {A, B, C, D, E, F, G} = 7'b0011001; // 4
            4'h5: {A, B, C, D, E, F, G} = 7'b0010010; // 5
            4'h6: {A, B, C, D, E, F, G} = 7'b0000010; // 6
            4'h7: {A, B, C, D, E, F, G} = 7'b1111000; // 7
            4'h8: {A, B, C, D, E, F, G} = 7'b0000000; // 8
            4'h9: {A, B, C, D, E, F, G} = 7'b0010000; // 9
            4'hA: {A, B, C, D, E, F, G} = 7'b0001000; // A
            4'hB: {A, B, C, D, E, F, G} = 7'b0000011; // b
            4'hC: {A, B, C, D, E, F, G} = 7'b1000110; // C
            4'hD: {A, B, C, D, E, F, G} = 7'b0100001; // d
            4'hE: {A, B, C, D, E, F, G} = 7'b0000110; // E
            4'hF: {A, B, C, D, E, F, G} = 7'b0001110; // F
            default: {A, B, C, D, E, F, G} = 7'b1111111; // Default: all segments off
        endcase
    end

endmodule
