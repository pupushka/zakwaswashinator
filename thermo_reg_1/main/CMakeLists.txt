# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS main.c init_ESP.c ssd1306.c xi2c.c fonts.c mcp3425a1.c enkoder_isr.c pid.c  wifi_sta.c tcp_ip_server.c interpreter.c    # list the source files of this component
    INCLUDE_DIRS        # optional, add here public include directories
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
    REQUIRES            # optional, list the public requirements (component names)
    PRIV_REQUIRES       # optional, list the private requirements
)
