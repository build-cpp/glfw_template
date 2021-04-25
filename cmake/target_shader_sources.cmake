function(target_shader_sources target scope)
    target_sources(${target} ${scope} ${ARGN})
    target_include_directories(${target} ${scope} "${CMAKE_CURRENT_BINARY_DIR}/include")
    foreach(source ${ARGN})
        get_filename_component(SHADER_NAME ${source} NAME_WE)
        set(SHADER_HEADER "include/shaders/${SHADER_NAME}.h")
        configure_file("${source}" "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_HEADER}")
        file(READ "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_HEADER}" SHADER_CONTENTS)
        file(GENERATE
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_HEADER}"
            CONTENT "namespace shaders {\n\tstatic const char* ${SHADER_NAME} = R\"GLSL(${SHADER_CONTENTS})GLSL\";\n}"
        )
        target_sources(${target} ${scope} "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_HEADER}")
        message(STATUS "Generated ${SHADER_HEADER}")
    endforeach()
    
endfunction()