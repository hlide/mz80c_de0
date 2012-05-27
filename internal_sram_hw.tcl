# TCL File Generated by Component Editor 11.0sp1
# Wed May 02 13:08:51 GMT+09:00 2012
# DO NOT MODIFY


# +-----------------------------------
# | 
# | internal_sram "internal_sram" v1.0
# | null 2012.05.02.13:08:49
# | 
# | 
# +-----------------------------------

# +-----------------------------------
# | request TCL package from ACDS 11.0
# | 
package require -exact sopc 11.0
# | 
# +-----------------------------------

# +-----------------------------------
# | module internal_sram
# | 
set_module_property NAME internal_sram
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property DISPLAY_NAME internal_sram
set_module_property TOP_LEVEL_HDL_MODULE ""
set_module_property INSTANTIATE_IN_SYSTEM_MODULE false
set_module_property EDITABLE true
set_module_property ANALYZE_HDL TRUE
set_module_property STATIC_TOP_LEVEL_MODULE_NAME ""
set_module_property FIX_110_VIP_PATH false
# | 
# +-----------------------------------

# +-----------------------------------
# | files
# | 
# | 
# +-----------------------------------

# +-----------------------------------
# | parameters
# | 
# | 
# +-----------------------------------

# +-----------------------------------
# | display items
# | 
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point avalon_int_sram_slave
# | 
add_interface avalon_int_sram_slave avalon end
set_interface_property avalon_int_sram_slave addressAlignment DYNAMIC
set_interface_property avalon_int_sram_slave addressUnits WORDS
set_interface_property avalon_int_sram_slave associatedClock clock_sink
set_interface_property avalon_int_sram_slave associatedReset clock_sink_reset
set_interface_property avalon_int_sram_slave burstOnBurstBoundariesOnly false
set_interface_property avalon_int_sram_slave explicitAddressSpan 0
set_interface_property avalon_int_sram_slave holdTime 0
set_interface_property avalon_int_sram_slave isMemoryDevice true
set_interface_property avalon_int_sram_slave isNonVolatileStorage false
set_interface_property avalon_int_sram_slave linewrapBursts false
set_interface_property avalon_int_sram_slave maximumPendingReadTransactions 0
set_interface_property avalon_int_sram_slave printableDevice false
set_interface_property avalon_int_sram_slave readLatency 0
set_interface_property avalon_int_sram_slave readWaitTime 1
set_interface_property avalon_int_sram_slave setupTime 0
set_interface_property avalon_int_sram_slave timingUnits Cycles
set_interface_property avalon_int_sram_slave writeWaitTime 0

set_interface_property avalon_int_sram_slave ENABLED true

add_interface_port avalon_int_sram_slave ADDR address Input 19
add_interface_port avalon_int_sram_slave DATA_O readdata Output 16
add_interface_port avalon_int_sram_slave DATA_I writedata Input 16
add_interface_port avalon_int_sram_slave DEN byteenable_n Input 2
add_interface_port avalon_int_sram_slave CS chipselect_n Input 1
add_interface_port avalon_int_sram_slave WE write_n Input 1
add_interface_port avalon_int_sram_slave WREQ waitrequest_n Output 1
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point clock_sink
# | 
add_interface clock_sink clock end
set_interface_property clock_sink clockRate 0

set_interface_property clock_sink ENABLED true

add_interface_port clock_sink CLK clk Input 1
# | 
# +-----------------------------------
