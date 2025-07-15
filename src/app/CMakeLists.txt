set(NAME "App")

include (${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_executable(${NAME} main.cpp)

target_compile_features(${NAME} PRIVATE cxx_std_23)
target_link_libraries(${NAME} PRIVATE project_warnings Vulkan::Vulkan spdlog::spdlog Engine)

# Check if Python is available on the system
execute_process(COMMAND python3 --version RESULT_VARIABLE PYTHON_RESULT OUTPUT_VARIABLE PYTHON_OUTPUT)

if(PYTHON_RESULT EQUAL 0)
    message(STATUS "Found Python: ${PYTHON_OUTPUT}")

    add_custom_target(
        CompileShaders
        COMMAND "python3 ${CMAKE_SOURCE_DIR}/scripts/build_shaders.py"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Compiling shaders..."
    )
else()
    message(WARNING "Python not found on the system. Shaders won't be compiled automatically.")
endif()
