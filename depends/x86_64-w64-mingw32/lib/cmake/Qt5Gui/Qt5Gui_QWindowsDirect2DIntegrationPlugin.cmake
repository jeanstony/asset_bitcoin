
add_library(Qt5::QWindowsDirect2DIntegrationPlugin MODULE IMPORTED)

_populate_Gui_plugin_properties(QWindowsDirect2DIntegrationPlugin RELEASE "platforms/libqdirect2d.a")

list(APPEND Qt5Gui_PLUGINS Qt5::QWindowsDirect2DIntegrationPlugin)
