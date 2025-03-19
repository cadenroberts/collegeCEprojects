.lib '~/.volare/sky130A/libs.tech/ngspice/sky130.lib.spice' tt
.include '~/.volare/sky130A/libs.ref/sky130_fd_sc_hd/spice/sky130_fd_sc_hd.spice'

.param supply_voltage=1.8V

.option temp=27

Xinv A gnd gnd vdd vdd Z sky130_fd_sc_hd__inv_2

Xinv1 Z gnd gnd vdd vdd Z1 sky130_fd_sc_hd__inv_2
Xinv2 Z gnd gnd vdd vdd Z2 sky130_fd_sc_hd__inv_2
Xinv3 Z gnd gnd vdd vdd Z3 sky130_fd_sc_hd__inv_2
Xinv4 Z gnd gnd vdd vdd Z4 sky130_fd_sc_hd__inv_2

VDD vdd 0 supply_voltage

VSW A 0 PULSE (0V supply_voltage 500ps 5ps 5ps 1000p 2000ps) DC 0V

.tran 1ps 3ns

.param half_supply = '0.5*supply_voltage'
.param slew_low = '0.1*supply_voltage'
.param slew_high = '0.9*supply_voltage'

.meas tran rise_delay trig v(A) val=half_supply fall=1 targ v(Z) val=half_supply rise=1

.meas tran rise_time trig v(Z) val=slew_low rise=1 targ v(Z) val=slew_high rise=1

.control
run
quit
.endc

.END
