set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/examples/")

if(RT_DEBUG_BUILD)
  set(RT_CXX_FLAGS  ${RT_CXX_FLAGS}  -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer)
  set(RT_LINK_FLAGS ${RT_LINK_FLAGS} -g -O0 -fsanitize=address,undefined)
else()
  set(RT_CXX_FLAGS  ${RT_CXX_FLAGS}  -O3)
  set(RT_LINK_FLAGS ${RT_LINK_FLAGS} -O3)
endif()

set(executables
        example_dot_product
        example_matrix_product
        example_eigen_dot_product
        example_eigen_matrix_product
        example_multiple_assign_bug
        example_division
        example_subtraction
        example_dyn_matrix_inverse
        example_eigen_llt
        example_AAD_112
        example_gaussian_elimination
        # example_matrix_function
        example_to_python
        example_llt_to_python
)

foreach(exec ${executables})
    # - Define executables ------
    add_executable(${exec} ${exec}.cpp)

    # - Set compile flags -------
    target_compile_options(${exec} PRIVATE ${RT_CXX_FLAGS})

    # - Set link flags ----------
    target_link_options(${exec} PRIVATE ${RT_LINK_FLAGS})

    # - Define include path -----
    target_include_directories(${exec}        PRIVATE ${CMAKE_SOURCE_DIR}/include/)
    target_include_directories(${exec} SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/ThirdParty/)
endforeach()
