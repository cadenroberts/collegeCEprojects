current_design top
set ::env(IO_SYNC) 0
set clock_port clk_i
set clock_period 100
create_clock -name $clock_port -period $::env(CLOCK_PERIOD) [get_ports $clock_port]
set clk_input [get_port $clock_port]
set clk_indx [lsearch [all_inputs] $clk_input]
set all_inputs_wo_clk [lreplace [all_inputs] $clk_indx $clk_indx ""]
set_input_delay 2 -clock $clock_port $all_inputs_wo_clk
set_output_delay 2 -clock $clock_port [all_outputs]
set_driving_cell -lib_cell sky130_fd_sc_hd__dfxtp_1 -pin {Q} $all_inputs_wo_clk
set_clock_uncertainty 0.1 [get_clocks $clock_port]
set_input_transition 0.05 $all_inputs_wo_clk
set_load 0.006 [all_outputs]
