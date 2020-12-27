set(SC_SEARCH_MODULE_SRC "${PROBLEM_SOLVER_ROOT}/searchModule")

set(SOURCES
        "searchModule.cpp"
        "myodes/myodes.cpp"
        "agents/InclusionSearchAgent.cpp"
        )

set(HEADERS
        "searchModule.hpp"
        "myodes/myodes.hpp"
        "agents/InclusionSearchAgent.hpp"
        )

include_directories(${SC_SEARCH_MODULE_SRC} ${SC_MEMORY_SRC} ${GLIB2_INCLUDE_DIRS})

add_library(searchModule SHARED ${SOURCES} ${HEADERS})
add_dependencies(searchModule sc-memory-cpp sc-kpm-common sc-agents-common)
target_link_libraries(searchModule sc-memory-cpp sc-kpm-common sc-agents-common)

sc_codegen(searchModule ${SC_SEARCH_MODULE_SRC})