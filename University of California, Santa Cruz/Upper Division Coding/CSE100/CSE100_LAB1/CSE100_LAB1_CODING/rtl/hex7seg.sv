module hex7seg(
    input  logic d3, d2, d1, d0,
    output logic A, B, C, D, E, F, G
);

assign A = ~((~d3&~d2&~d1&d0)|(~d3&d2&~d1&~d0)|(d3&~d2&d1&d0)|(d3&d2&~d1&d0));

assign B = ~((~d3&d2&~d1&d0)|(~d3&d2&d1&~d0)|(d3&~d2&d1&d0)|(d3&d2&~d1&~d0)|(d3&d2&d1));

assign C = ~((~d3&~d2&d1&~d0)|(d3&d2&~d1&~d0)|(d3&d2&d1));

assign D = ~((~d3&~d2&~d1&d0)|(~d3&d2&~d1&~d0)|(~d3&d2&d1&d0)|(d3&~d2&d1&~d0)|(d3&d2&d1&d0));

assign E = ~((~d3&~d2&~d1&d0)|(~d3&~d2&d1&d0)|(~d3&d2&~d1)|(~d3&d2&d1&d0)|(d3&~d2&~d1&d0));

assign F = ~((~d3&~d2&d0)|(~d3&~d2&d1&~d0)|(~d3&d2&d1&d0)|(d3&d2&~d1&d0));

assign G = ~((~d3&~d2&~d1)|(~d3&d2&d1&d0)|(d3&d2&~d1&~d0));

endmodule

// CODE 6967
// A is off for 1, 4, B, D
// B is off for 5, 6, B, C, E, F
// C is off for 2, C, E, F
// D is off for 1, 4, 7, A, F
// E is off for 1, 3, 4, 5, 7, 9
// F is off for 1, 2, 3, 7, D
// G is off for 0, 1, 7, C
