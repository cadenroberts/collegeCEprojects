# Read the standard cell library and technology files
read_lib $env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/lib/sky130_fd_sc_hd__tt_025C_1v80.lib

read_lef $env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/techlef/sky130_fd_sc_hd__nom.tlef
read_lef $env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/lef/sky130_fd_sc_hd.lef
read_lef $env(PDK_ROOT)/sky130A/libs.ref/sky130_fd_sc_hd/lef/sky130_ef_sc_hd.lef

# Read the gate-level netlist
read_verilog gate_level.v

# Define the design
current_design top

# Link the design
link_design top

# Read the constraints file
read_sdc gate_level.sdc
