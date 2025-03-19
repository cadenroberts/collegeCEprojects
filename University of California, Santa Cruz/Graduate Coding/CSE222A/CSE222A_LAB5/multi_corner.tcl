# Fast Corner (FF, -40C, 1.95V)
puts "Fast:"
read_lib "$env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/lib/sky130_fd_sc_hd__ff_n40C_1v95.lib"
read_lef "$env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/techlef/sky130_fd_sc_hd__nom.tlef"
read_lef "$env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/lef/sky130_fd_sc_hd.lef"
read_lef "$env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/lef/sky130_ef_sc_hd.lef"
read_verilog gate_level.v
current_design top
link_design top
read_sdc gate_level_false.sdc
report_checks -through Y_DFF/D -path_delay min_max
puts "Typical:"
# Typical Corner (TT, 25C, 1.80V)
read_lib "$env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/lib/sky130_fd_sc_hd__tt_025C_1v80.lib"
read_lef "$env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/techlef/sky130_fd_sc_hd__nom.tlef"
read_lef "$env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/lef/sky130_fd_sc_hd.lef"
read_lef "$env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/lef/sky130_ef_sc_hd.lef"
read_verilog gate_level.v
current_design top
link_design top
read_sdc gate_level_false.sdc
report_checks -through Y_DFF/D -path_delay min_max
# Slow Corner (SS, 100C, 1.60V)
puts "Slow:"
read_lib "$env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/lib/sky130_fd_sc_hd__ss_100C_1v60.lib"
read_lef "$env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/techlef/sky130_fd_sc_hd__nom.tlef"
read_lef "$env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/lef/sky130_fd_sc_hd.lef"
read_lef "$env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/lef/sky130_ef_sc_hd.lef"
read_verilog gate_level.v
current_design top
link_design top
read_sdc gate_level_false.sdc
report_checks -through Y_DFF/D -path_delay min_max

