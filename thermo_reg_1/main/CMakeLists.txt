# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS main.cpp utils.cpp init_ESP.cpp ssd1306.cpp xi2c.cpp fonts.cpp mcp3425a1.cpp enkoder_isr.cpp pid.cpp  wifi_sta.cpp interpreter.cpp tcp_ip_server.cpp   # list the source files of this component
    INCLUDE_DIRS        # optional, add here public include directories
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
    REQUIRES            # optional, list the public requirements (component names)
    PRIV_REQUIRES       # optional, list the private requirements
)
