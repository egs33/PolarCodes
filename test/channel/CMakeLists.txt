add_executable(channel-test bec.cpp bsc.cpp ../../src/channel/Channel.cpp ../../src/channel/BEC.cpp ../../src/channel/BSC.cpp)
target_link_libraries(channel-test
        gtest
        gtest_main
        pthread
        )
add_test(
        NAME channel
        COMMAND $<TARGET_FILE:channel-test>
)

set_property(
        TEST channel
        PROPERTY LABELS lib channel
)