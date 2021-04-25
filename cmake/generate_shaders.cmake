# Enumerates the target sources and automatically generates include/shaders/${SHADER_NAME}.h from all .glsl files
function(generate_shaders target)
    get_property(TARGET_SOURCES
        TARGET ${target}
        PROPERTY SOURCES
    )
    foreach(SOURCE ${TARGET_SOURCES})
        if(SOURCE MATCHES ".glsl$")
            get_filename_component(SHADER_NAME "${SOURCE}" NAME_WE)
            set(SHADER_HEADER "include/shaders/${SHADER_NAME}.h")
            configure_file("${SOURCE}" "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_HEADER}")
            file(READ "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_HEADER}" SHADER_CONTENTS)
            file(GENERATE
                OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_HEADER}"
                CONTENT "namespace shaders {\n\tstatic const char* ${SHADER_NAME} = R\"GLSL(${SHADER_CONTENTS})GLSL\";\n}"
            )
            message(STATUS "Generated ${SHADER_HEADER}")
        endif()
    endforeach()
endfunction()