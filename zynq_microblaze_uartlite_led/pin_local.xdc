create_clock -period 20.000 -name sys_clk [get_ports sys_clk]
set_property -dict {PACKAGE_PIN N18 IOSTANDARD LVCMOS15} [get_ports sys_clk]
set_property -dict {PACKAGE_PIN P14 IOSTANDARD LVCMOS15} [get_ports sys_rst_n]


# GPIO2
set_property -dict {PACKAGE_PIN H15 IOSTANDARD LVCMOS33} [get_ports UART_rxd]
set_property -dict {PACKAGE_PIN G15 IOSTANDARD LVCMOS33} [get_ports UART_txd]

set_property -dict {PACKAGE_PIN K16 IOSTANDARD LVCMOS33} [get_ports {LED_tri_io[0]}]
set_property -dict {PACKAGE_PIN K19 IOSTANDARD LVCMOS33} [get_ports {LED_tri_io[1]}]
set_property -dict {PACKAGE_PIN L16 IOSTANDARD LVCMOS33} [get_ports {LED_tri_io[2]}]
set_property -dict {PACKAGE_PIN M19 IOSTANDARD LVCMOS33} [get_ports {LED_tri_io[3]}]

set_property -dict {PACKAGE_PIN L19 IOSTANDARD LVCMOS33} [get_ports {KEY_tri_i[0]}]
set_property -dict {PACKAGE_PIN L20 IOSTANDARD LVCMOS33} [get_ports {KEY_tri_i[1]}]

set_property PULLUP true [get_ports {KEY_tri_i[0]}]
set_property PULLUP true [get_ports {KEY_tri_i[1]}]