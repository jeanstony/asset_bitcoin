
add_library(Qt5::QTextToSpeechPluginSapi MODULE IMPORTED)

_populate_TextToSpeech_plugin_properties(QTextToSpeechPluginSapi RELEASE "texttospeech/libqtexttospeech_sapi.a")

list(APPEND Qt5TextToSpeech_PLUGINS Qt5::QTextToSpeechPluginSapi)
