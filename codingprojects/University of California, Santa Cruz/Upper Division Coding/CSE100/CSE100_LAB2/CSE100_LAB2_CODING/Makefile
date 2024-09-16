# Copyright (c) 2024 Ethan Sifferman.
# All rights reserved. Distribution Prohibited.

RTL := $(shell python3 misc/convert_filelist.py rtl/rtl.f)
YOSYS_DATDIR := $(shell yosys-config --datdir)

TOP := tb
PART := xc7a35tcpg236-1

sim: sqrt.memh
	verilator --Mdir $@_dir -f rtl/rtl.f -f dv/dv.f --binary --top ${TOP}
	./$@_dir/V${TOP} +verilator+rand+reset+0
	./$@_dir/V${TOP} +verilator+rand+reset+1
	./$@_dir/V${TOP} +verilator+rand+reset+2

lint:
	verilator --lint-only -f rtl/rtl.f synth/basys3/basys3.sv --top basys3
	verilator --lint-only -f rtl/rtl.f
	verible-verilog-lint \
	 --waiver_files dv/verible.waiver \
	 --rules_config dv/verible.lint \
	 --autofix=inplace-interactive \
	 ${RTL} synth/basys3/basys3.sv

synth/yosys_common/build/rtl.sv: ${RTL}
	mkdir -p $(dir $@)
	cat $^ > $@

synth/yosys_common/build/synth.v: synth/yosys_common/build/rtl.sv sqrt.memh synth/yosys_common/yosys.tcl
	rm -rf slpp_all
	yosys -p 'tcl synth/yosys_common/yosys.tcl' -l synth/yosys_common/build/yosys.log

sqrt.memh: rtl/generate_sqrt_lut.py
	python3 $< $@

gls: synth/yosys_common/build/synth.v
	verilator -I${YOSYS_DATDIR} --Mdir $@_dir -f synth/yosys_common/gls.f -f dv/dv.f --binary --top ${TOP}
	./$@_dir/V${TOP} +verilator+rand+reset+0
	./$@_dir/V${TOP} +verilator+rand+reset+1
	./$@_dir/V${TOP} +verilator+rand+reset+2

synth/basys3/build/basys3/basys3.runs/impl_1/basys3.bit: ${RTL} synth/basys3/basys3.sv sqrt.memh synth/basys3/Basys3_Master.xdc synth/basys3/vivado.tcl
	rm -rf synth/basys3/build/basys3
	mkdir -p synth/basys3/build
	cd synth/basys3/build && \
	 vivado -quiet -nolog -nojournal -notrace -mode tcl \
	  -source ../vivado.tcl

vivado-program: synth/basys3/build/basys3/basys3.runs/impl_1/basys3.bit
	cd synth/basys3/build && \
	 vivado -quiet -nolog -nojournal -notrace -mode tcl \
	  -source ../vivado-program.tcl -tclargs ${PART} ../../../$<

clean:
	rm -rf \
	 sqrt.memh \
	 dump.fst \
	 sim_dir gls_dir \
	 synth/basys3/build \
	 synth/yosys_common/build
	$(MAKE) -C autograder clean
	$(MAKE) -C docs clean
	$(MAKE) -C starter clean
