# An highlighted block
set_param messaging.defaultLimit 50000

proc ReadReg { Addr } {
# puts "Start reading operation"
set address [format "0x%08X" $Addr]
set len 1
create_hw_axi_txn read_txn -force [get_hw_axis hw_axi_1] -type read \
-address $address -len $len
run_hw_axi [get_hw_axi_txns read_txn]
set read_proc [list [report_hw_axi_txn [get_hw_axi_txns read_txn]]];
set read_status_value [lindex $read_proc 0 1]
delete_hw_axi_txn [get_hw_axi_txns read_txn]
puts $read_status_value;
return $read_status_value;
}

proc WriteReg { Addr data} {
set len 1
set address [format "0x%08X" $Addr]
set data [format "0x%08X" $data]
create_hw_axi_txn -force write_txn [get_hw_axis hw_axi_1] -type write \
-address $address -len $len -data $data
run_hw_axi [get_hw_axi_txns write_txn]
delete_hw_axi_txn [get_hw_axi_txns write_txn]
}

proc WriteReg_N {Addr data length} {
    for {set i 0} {$i < $length} {incr i} {
        set new_address [expr $Addr + 4*$i]
        set new_data [expr $data + $i]
        WriteReg $new_address $new_data
    }
}

proc ReadReg_N {Addr length} {
    for {set i 0} {$i < $length} {incr i} {
        set new_address [expr $Addr + 4*$i]
        ReadReg $new_address
    }
}

# 定义 mrd 函数，根据参数数量选择调用 ReadReg 或 ReadReg_N
proc mrd {args} {
    set num_args [llength $args]

    if {$num_args == 1} {
        ReadReg [lindex $args 0]
    } elseif {$num_args == 2} {
        ReadReg_N [lindex $args 0] [lindex $args 1]
    } else {
        puts "Invalid number of arguments for mrd"
    }
}

# 定义 mwr 函数，根据参数数量选择调用 WriteReg 或 WriteReg_N
proc mwr {args} {
    set num_args [llength $args]

    if {$num_args == 2} {
        WriteReg [lindex $args 0] [lindex $args 1]
    } elseif {$num_args == 3} {
        WriteReg_N [lindex $args 0] [lindex $args 1] [lindex $args 2]
    } else {
        puts "Invalid number of arguments for mwr"
    }
}


# Example usage:
#WriteReg 0xC0000000 0x12345678
#ReadReg 0xC0000000
#WriteReg_N 0xC0000000 1 10
#ReadReg_N 0xC0000000 10


#WriteReg_N 0xC0000000 0x87654321 8
#ReadReg_N 0xC0000000 8
#WriteReg 0x44A00018 0xC0000000
#WriteReg 0x44A00020 0xC0001000
#WriteReg 0x44A00028 32
#ReadReg_N 0xC0001000  8

mwr 0xC0000000 0x87654321 16
#mrd 0xC0000000 16
mwr 0x41E00000 0x00010003
mwr 0x41E00018 0xC0000000
mwr 0x41E00048 0xC0001000
mwr 0x41E00028 64

mwr 0x41E00030 0x00010003
#mrd 0x41E00000 30
mwr 0x41E00058 64
mrd 0xC0001000 16

mwr 0x41E00030 0x00010003
#mrd 0x41E00000 30
mwr 0x41E00058 0
mwr 0x41E00058 64
mrd 0xC0001000 16

