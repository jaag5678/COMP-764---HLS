transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vcom -93 -work work {C:/Users/jaag5/OneDrive - McGill University/Study/Courses-Attended/COMP764-HighLevelSynthesis/COMP-764---HLS/Assignment/VHDLCircuits/int32add.vhd}
vcom -93 -work work {C:/Users/jaag5/OneDrive - McGill University/Study/Courses-Attended/COMP764-HighLevelSynthesis/COMP-764---HLS/Assignment/VHDLCircuits/int32mult.vhd}
vcom -93 -work work {C:/Users/jaag5/OneDrive - McGill University/Study/Courses-Attended/COMP764-HighLevelSynthesis/COMP-764---HLS/Assignment/VHDLCircuits/int32dualportRAM.vhd}
vcom -93 -work work {C:/Users/jaag5/OneDrive - McGill University/Study/Courses-Attended/COMP764-HighLevelSynthesis/COMP-764---HLS/Assignment/VHDLCircuits/ex4.vhd}

vcom -93 -work work {C:/Users/jaag5/OneDrive - McGill University/Study/Courses-Attended/COMP764-HighLevelSynthesis/COMP-764---HLS/Assignment/VHDLCircuits/test_ex4.vhd}

vsim -t 1ps -L altera -L lpm -L sgate -L altera_mf -L altera_lnsim -L cyclonev -L rtl_work -L work -voptargs="+acc"  test_ex4

add wave *
view structure
view signals
run -all
