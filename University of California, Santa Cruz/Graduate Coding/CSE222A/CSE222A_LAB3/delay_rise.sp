.lib '~/.volare/sky130A/libs.tech/ngspice/sky130.lib.spice' tt
.include '~/.volare/sky130A/libs.ref/sky130_fd_sc_hd/spice/sky130_fd_sc_hd.spice'

.param supply_voltage=1.8V

.option temp=27

Xgate A1 A2 B1 gnd gnd vdd vdd X sky130_fd_sc_hd__a21oi_2

Xinv1 X gnd gnd vdd vdd Z1 sky130_fd_sc_hd__inv_2
Xinv2 X gnd gnd vdd vdd Z2 sky130_fd_sc_hd__inv_2
Xinv3 X gnd gnd vdd vdd Z3 sky130_fd_sc_hd__inv_2
Xinv4 X gnd gnd vdd vdd Z4 sky130_fd_sc_hd__inv_2

VDD vdd 0 supply_voltage

VSW A1 0 PULSE (0V supply_voltage 500ps 5ps 5ps 1000p 2000ps) DC 0V
VA A2 0 1.8V
VB B1 0 0V

.tran 1ps 3ns

.param half_supply = '0.5*supply_voltage'
.param slew_low = '0.1*supply_voltage'
.param slew_high = '0.9*supply_voltage'


.meas tran rise_delay trig v(A1) val=0.9 fall=1 targ v(X) val=0.9 rise=1

.control
run
quit
.endc

.END
