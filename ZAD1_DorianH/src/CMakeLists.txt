cmake_minimum_required(VERSION 3.10)
project(DataCompressionTest VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(ZAD1_DorianH ZAD1_DorianH.cpp)

enable_testing()

function(add_compression_test test_name input_array expected_output)
    set(test_src "${CMAKE_CURRENT_BINARY_DIR}/${test_name}.cpp")
    
    file(WRITE ${test_src}
        "#include <cassert>\n"
        "#include <vector>\n"
        "#include <optional>\n"
        "#define UNIT_TESTING\n"
        "#include \"${CMAKE_CURRENT_SOURCE_DIR}/ZAD1_DorianH.cpp\"\n\n"
        "int main() {\n"
        "    Array2D<int> testArray = ${input_array};\n"
        "    auto compressedResult = compressData(testArray);\n"
        "    std::optional<CompressedData<int>> expected = ${expected_output};\n"
        "    assert(compressedResult.has_value() == expected.has_value());\n"
        "    if(compressedResult.has_value() && expected.has_value()) {\n"
        "        assert(compressedResult->size() == expected->size());\n"
        "        for (size_t i = 0; i < compressedResult->size(); ++i) {\n"
        "            assert((*compressedResult)[i].size() == (*expected)[i].size());\n"
        "            for (size_t j = 0; j < (*compressedResult)[i].size(); ++j) {\n"
        "                assert((*compressedResult)[i][j].value == (*expected)[i][j].value);\n"
        "                assert((*compressedResult)[i][j].count == (*expected)[i][j].count);\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "    return 0;\n"
        "}\n"
    )

    add_executable(${test_name} ${test_src})
    
    target_compile_definitions(${test_name} PRIVATE UNIT_TESTING)
    
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_compression_test(test1 "{{{1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1}}}" "std::optional<CompressedData<int>>{{{{1,7}},{{1,7}},{{1,7}}}}")
add_compression_test(test2 "{{{0, 0, 0, 1, 1, 2, 3}}}" "std::optional<CompressedData<int>>{{{{0,3},{1,2},{2,1},{3,1}}}}")
add_compression_test(test3 "{{{1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1}}}" "std::optional<CompressedData<int>>{{{{1,8}},{{1,8}},{{1,8}}}}")

