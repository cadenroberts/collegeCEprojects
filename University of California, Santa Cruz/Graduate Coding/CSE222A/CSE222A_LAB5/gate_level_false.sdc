current_design top

set A_pin [lindex [get_pins {A_DFF/Q}] 0]
set B_pin [lindex [get_pins {B_DFF/Q}] 0]
set C_pin [lindex [get_pins {C_DFF/Q}] 0]
set D_pin [lindex [get_pins {D_DFF/Q}] 0]
set E_pin [lindex [get_pins {E_DFF/Q}] 0]

set Y_pins [get_pins -of_object [get_nets {Y_o}]]

set N1_pin [lindex [get_pins {GL/A_a_B/X}] 0]
set N2_pin [lindex [get_pins {GL/A_x_N1/X}] 0]
set N3_pin [lindex [get_pins {GL/N1_o_C/X}] 0]
set N4_pin [lindex [get_pins {GL/D_n/Y}] 0]
set N5_pin [lindex [get_pins {GL/D_a_E/X}] 0]
set N6_pin [lindex [get_pins {GL/N4_a_N5/X}] 0]

# These are auxiliary ports. You can ignore them.
set load_ports [list [get_ports {Y1}]\
                     [get_ports {Y2}]\
                     [get_ports {Y3}]\
                     [get_ports {Y4}]\
                     [get_ports {Y5}]\
                     [get_ports {Y6}]\
                     [get_ports {Y7}]\
                     [get_ports {Y8}]\
                     [get_ports {Y9}]]

set inputs [list [get_ports {A}]\
                 [get_ports {B}]\
                 [get_ports {C}]\
                 [get_ports {D}]\
                 [get_ports {E}]\
                 [get_ports {rst}]]

# Define the clock
create_clock -name clk -period 1.5 [get_ports clk]

# Set clock transition to 0.15ns
set_clock_transition 0.15 [get_clocks clk]

# Set clock uncertainty to 0.25ns
set_clock_uncertainty 0.25 [get_clocks clk]

# Set input delays relative to the clock edge
set_input_delay 0.5 -clock [get_clocks clk] [get_ports {A B C D E rst}]

# Set output delay based on required arrival time (RAT)
set_output_delay 0.75 -clock [get_clocks clk] -add_delay $load_ports

# Set the output load to 0.15
set_load -pin_load 0.15 $load_ports

# Define the input drive strength using the sky130_fd_sc_hd__inv_2 cell
set_driving_cell -lib_cell sky130_fd_sc_hd__inv_2 -pin Y [get_ports {A B C D E clk}]

# Define false paths
set_false_path -through $N3_pin
