transcript on
if {[file exists gate_work]} {
	vdel -lib gate_work -all
}
vlib gate_work
vmap work gate_work

vcom -93 -work work {assignment1.vho}

vcom -93 -work work {C:/Users/jaag5/OneDrive - McGill University/Study/Courses-Attended/COMP764-HighLevelSynthesis/COMP-764---HLS/Assignment/VHDLCircuits/testbench_eqSolver.vhd}

vsim -t 1ps +transport_int_delays +transport_path_delays -sdftyp /NA=assignment1_vhd.sdo -L altera -L altera_lnsim -L cyclonev -L gate_work -L work -voptargs="+acc"  testbench_eqSolver

add wave *
view structure
view signals
run -all
