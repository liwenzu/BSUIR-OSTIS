set(SC_FA_SIM_CAL_SRC "${PROBLEM_SOLVER_ROOT}/factual_knowledge_similarity_calculation")

set(SOURCES
        "factual_knowledge_similarity_calculation.cpp"
        "keynodes/keynodes.cpp"
        "agents/FasimilarityAgent.cpp"
        )

set(HEADERS
        "factual_knowledge_similarity_calculation.hpp"
        "keynodes/keynodes.hpp"
        "agents/FasimilarityAgent.hpp"
        )

include_directories(${SC_FA_SIM_CAL_SRC} ${SC_MEMORY_SRC} ${GLIB2_INCLUDE_DIRS})

add_library(factual_knowledge_similarity_calculation SHARED ${SOURCES} ${HEADERS})
add_dependencies(factual_knowledge_similarity_calculation sc-memory-cpp sc-kpm-common sc-agents-common)
target_link_libraries(factual_knowledge_similarity_calculation sc-memory-cpp sc-kpm-common sc-agents-common)

SET(CMAKE_BUILD_TYPE Debug)

sc_codegen(factual_knowledge_similarity_calculation ${SC_FA_SIM_CAL_SRC})