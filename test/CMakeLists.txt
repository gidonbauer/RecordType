set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/test/")

set(RT_CXX_FLAGS  ${RT_CXX_FLAGS}  -O0 -fsanitize=address,undefined -fno-omit-frame-pointer)
set(RT_LINK_FLAGS ${RT_LINK_FLAGS} -O0 -fsanitize=address,undefined)


set(executables
        test_RT_RecordType_Construct
        test_RT_RecordType_Compare
        test_RT_RecordType_Add
        test_RT_RecordType_Mul
        test_RT_RecordType_Div
        test_RT_RecordType_Sub
        test_RT_RecordType_Sqrt
        test_RT_RecordType_Sin
        test_RT_RecordType_Cos
        test_RT_Graph
        test_RT_Graph_Unregistered
        test_RT_TypeTraits
)

include(GoogleTest)
foreach(exec ${executables})
    # - Define executables ------
    add_executable(${exec} ${exec}.cpp)

    # - Set compile flags -------
    target_compile_options(${exec} PRIVATE ${RT_CXX_FLAGS})

    # - Set link flags ----------
    target_link_options(${exec} PRIVATE ${RT_LINK_FLAGS})

    # - Define include path -----
    target_include_directories(${exec} PRIVATE ${CMAKE_SOURCE_DIR}/include/)

    # - Link libraries ---------
    target_link_libraries(${exec} PRIVATE GTest::gtest_main)
    gtest_discover_tests(${exec})
endforeach()
