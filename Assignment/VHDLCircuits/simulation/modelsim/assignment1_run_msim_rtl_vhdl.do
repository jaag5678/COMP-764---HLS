transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vcom -93 -work work {C:/Users/jaag5/OneDrive - McGill University/Study/Courses-Attended/COMP764-HighLevelSynthesis/VHDLCircuits/int32dualportRAM.vhd}

vcom -93 -work work {C:/Users/jaag5/OneDrive - McGill University/Study/Courses-Attended/COMP764-HighLevelSynthesis/VHDLCircuits/testbench_int32dualportRAM.vhd}

vsim -t 1ps -L altera -L lpm -L sgate -L altera_mf -L altera_lnsim -L cyclonev -L rtl_work -L work -voptargs="+acc"  testbench_int32dualportRAM

add wave *
view structure
view signals
run -all