set(NAME "tests")

include (${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_executable(${NAME} main.cpp)

target_compile_features(${NAME} PRIVATE cxx_std_20)
target_link_libraries(${NAME}
    PRIVATE
        project_warnings
    PUBLIC
        GTest::gtest_main
)
