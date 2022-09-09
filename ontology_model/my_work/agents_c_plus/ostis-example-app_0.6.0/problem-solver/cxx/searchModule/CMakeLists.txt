
file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

include_directories(${CMAKE_CURRENT_LIST_DIR} ${SC_MEMORY_SRC} ${SC_KPM_SRC})

add_library (searchModule SHARED ${SOURCES})
target_link_libraries(searchModule AnswerVerificationModule sc-memory sc-agents-common)

sc_codegen_ex(searchModule ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)