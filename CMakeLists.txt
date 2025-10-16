# Vähimmäisversio CMakea Zephyr-projekteille
cmake_minimum_required(VERSION 3.21.0)

# Etsitään Zephyr-järjestelmä (tämä on pakollinen)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Projektin nimi (voit vaihtaa vapaasti)
project(RTOStehtava3)

# Lähdekoodit
target_sources(app PRIVATE
    src/main.c
    src/led_example.c
    src/dispatcher.c
    src/uartDef.c
    src/button_example.c
)
