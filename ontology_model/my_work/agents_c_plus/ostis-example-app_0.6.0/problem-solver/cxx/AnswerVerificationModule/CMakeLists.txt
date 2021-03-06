file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

include_directories(${CMAKE_CURRENT_LIST_DIR} ${SC_MEMORY_SRC} ${SC_KPM_SRC})

add_library (AnswerVerificationModule SHARED ${SOURCES})
target_link_libraries(AnswerVerificationModule sc-memory sc-agents-common)

sc_codegen_ex(AnswerVerificationModule ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

SET(CMAKE_BUILD_TYPE Debug)