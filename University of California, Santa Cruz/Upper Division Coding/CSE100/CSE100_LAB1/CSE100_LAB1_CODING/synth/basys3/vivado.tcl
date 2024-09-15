# Copyright (c) 2024 Ethan Sifferman.
# All rights reserved. Distribution Prohibited.

# start_gui

create_project basys3 basys3 -part xc7a35tcpg236-1

add_files -norecurse {
 ../../../rtl/hex7seg.sv
 ../basys3.sv
}
add_files -fileset constrs_1 -norecurse {
 ../Basys3_Master.xdc
}

# Run Synthesis
set_property STEPS.SYNTH_DESIGN.ARGS.FLATTEN_HIERARCHY none [get_runs synth_1]
launch_runs synth_1 -jobs [exec nproc]
wait_on_run synth_1

# Run PNR
launch_runs impl_1
wait_on_run impl_1

# Artix-7
open_run impl_1
set_property CONFIG_VOLTAGE 3.3 [current_design]
set_property CFGBVS VCCO [current_design]

# Create Bitstream
launch_runs impl_1 -to_step write_bitstream
wait_on_run impl_1

exit
