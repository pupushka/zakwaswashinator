# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/dida/esp/esp-idf/components/bootloader/subproject"
  "/home/dida/eclipseESP32/thermo_reg_1/build/bootloader"
  "/home/dida/eclipseESP32/thermo_reg_1/build/bootloader-prefix"
  "/home/dida/eclipseESP32/thermo_reg_1/build/bootloader-prefix/tmp"
  "/home/dida/eclipseESP32/thermo_reg_1/build/bootloader-prefix/src/bootloader-stamp"
  "/home/dida/eclipseESP32/thermo_reg_1/build/bootloader-prefix/src"
  "/home/dida/eclipseESP32/thermo_reg_1/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/dida/eclipseESP32/thermo_reg_1/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
