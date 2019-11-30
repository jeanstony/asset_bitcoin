
add_library(Qt5::QXInputGamepadBackendPlugin MODULE IMPORTED)

_populate_Gamepad_plugin_properties(QXInputGamepadBackendPlugin RELEASE "gamepads/libxinputgamepad.a")

list(APPEND Qt5Gamepad_PLUGINS Qt5::QXInputGamepadBackendPlugin)
