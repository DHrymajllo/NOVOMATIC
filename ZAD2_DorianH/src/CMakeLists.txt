cmake_minimum_required(VERSION 3.10)
project(TicketMachineTest VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(ZAD2_DorianH ZAD2_DorianH.cpp)

enable_testing()

function(add_ticket_machine_test test_name input_sequence expected_output)
    set(test_src "${CMAKE_CURRENT_BINARY_DIR}/${test_name}.cpp")

    file(WRITE ${test_src}
        "#include <cassert>\n"
        "#include <iostream>\n"
        "#include <sstream>\n"
        "#ifdef UNIT_TESTING\n"
        "#undef UNIT_TESTING\n"
        "#endif\n"
        "#define UNIT_TESTING\n"
        "#include \"${CMAKE_CURRENT_SOURCE_DIR}/ZAD2_DorianH.cpp\"\n\n"
        "int main() {\n"
        "    TicketMachine machine;\n"
        "    std::istringstream iss(\"${input_sequence}\");\n"
        "    std::string input;\n"
        "    std::ostringstream oss;\n"
        "    std::streambuf* old_cout_stream_buf = std::cout.rdbuf();\n"
        "    std::cout.rdbuf(oss.rdbuf());\n"
        "    while (std::getline(iss, input, '\\n')) {\n"
        "        machine.processInput(input);\n"
        "    }\n"
        "    std::cout.rdbuf(old_cout_stream_buf);\n"
        "    assert(oss.str().find(\"${expected_output}\") != std::string::npos);\n"
        "    return 0;\n"
        "}\n"
    )

    add_executable(${test_name} ${test_src})
    target_compile_definitions(${test_name} PRIVATE UNIT_TESTING)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_ticket_machine_test("test_insert_money" "5" "Twoje saldo wynosi: 5")
add_ticket_machine_test("test_buy_ticket" "" "Bilet zostal wydrukowany")
