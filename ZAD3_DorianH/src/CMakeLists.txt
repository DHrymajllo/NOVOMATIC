cmake_minimum_required(VERSION 3.10)
project(CollisionDetectionTest VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SFML_DIR "C:/Users/RyderX/Documents/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(ZAD3_DorianH ZAD3_DorianH.cpp)
target_link_libraries(ZAD3_DorianH sfml-graphics)

enable_testing()

function(add_collision_test test_name expected_output)
    set(test_src "${CMAKE_CURRENT_BINARY_DIR}/${test_name}.cpp")

    file(WRITE ${test_src}
        "#include <cassert>\n"
        "#include <iostream>\n"
        "#include <sstream>\n"
        "#ifdef UNIT_TESTING\n"
        "#undef UNIT_TESTING\n"
        "#endif\n"
        "#define UNIT_TESTING\n"
        "#include \"${CMAKE_CURRENT_SOURCE_DIR}/ZAD3_DorianH.cpp\"\n\n"
        "int main() {\n"
        "    triangle tri1 {{{{100, 100}, {150, 50}, {200, 100}}}};\n"
        "    triangle tri2 {{{{300, 300}, {350, 250}, {400, 300}}}};\n"
        "    std::ostringstream oss;\n"
        "    std::streambuf* old_cout_stream_buf = std::cout.rdbuf();\n"
        "    std::cout.rdbuf(oss.rdbuf());\n"
        "    bool colliding = isColliding(tri1, tri2);\n"
        "    std::cout.rdbuf(old_cout_stream_buf);\n"
        "    assert(oss.str().find(\"${expected_output}\") != std::string::npos == colliding);\n"
        "    return 0;\n"
        "}\n"
    )

    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} sfml-graphics)

    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_collision_test("no_collision_test" "false")
add_collision_test("collision_test" "true")
