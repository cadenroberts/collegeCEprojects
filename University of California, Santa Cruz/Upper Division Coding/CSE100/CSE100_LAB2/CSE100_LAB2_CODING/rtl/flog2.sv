module flog2 (
  input wire [7:0] x_i,
  output wire [7:0] y_o
);

  assign y_o = x_i[7] ? 8'b00000111 :
               x_i[6] ? 8'b00000110 :
               x_i[5] ? 8'b00000101 :
               x_i[4] ? 8'b00000100 :
               x_i[3] ? 8'b00000011 :
               x_i[2] ? 8'b00000010 :
               x_i[1] ? 8'b00000001 :
                        8'b00000000;

endmodule
