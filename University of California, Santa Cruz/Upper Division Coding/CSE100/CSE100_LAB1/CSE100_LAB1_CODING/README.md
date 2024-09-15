
# hex7seg

```bash
# open in VS Code
code .

# lint with Verilator and Verible
make lint
# simulate with Verilator
make sim
gtkwave dump.fst
# synthesize with Yosys, then simulate with Verilator
make gls
gtkwave dump.fst
# synthesize, place-and-route, and program with Vivado
make vivado-program
```
