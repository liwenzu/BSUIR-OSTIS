file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

add_library (AnswerVerificationPyModule SHARED ${SOURCES})

target_include_directories(AnswerVerificationPyModule PRIVATE ${SC_MEMORY_SRC})
target_link_libraries(AnswerVerificationPyModule sc-memory)

add_dependencies(AnswerVerificationPyModule sc-memory)

sc_codegen(AnswerVerificationPyModule ${CMAKE_CURRENT_LIST_DIR})