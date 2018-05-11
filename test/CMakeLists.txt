include_directories(
        ${CMAKE_SOURCE_DIR}/lib/src/GoogleTest/include
)
add_subdirectory(channel)

add_executable(polar-code-test
        PolarCode.cpp
        ../src/PolarCode.cpp
        ../src/GnCosetCode.cpp
        ../src/channel/BEC.cpp
        ../src/channel/Channel.cpp)
target_link_libraries(polar-code-test
        gtest
        gtest_main
        pthread
        )
add_test(
        NAME polar-code
        COMMAND $<TARGET_FILE:polar-code-test>
)

set_property(
        TEST polar-code
        PROPERTY LABELS lib polar-code
)