
add_library(Qt5::SystecCanBusPlugin MODULE IMPORTED)

_populate_SerialBus_plugin_properties(SystecCanBusPlugin RELEASE "canbus/libqtsysteccanbus.a")

list(APPEND Qt5SerialBus_PLUGINS Qt5::SystecCanBusPlugin)