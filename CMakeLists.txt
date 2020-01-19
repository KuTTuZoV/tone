set(CMAKE_PREFIX_PATH "D:/Qt/5.14.0/mingw73_32")

find_package(Qt5Core REQUIRED)

add_library(tone SHARED hue.cpp)

qt5_use_modules(hue Core)

