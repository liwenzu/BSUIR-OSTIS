set(PROBLEM_SOLVER_ROOT ${CMAKE_CURRENT_LIST_DIR})

file(MAKE_DIRECTORY ${SC_EXTENSION_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSION_DIRECTORY})

# for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${SC_BIN_PATH}/extensions")
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${SC_BIN_PATH}/extensions")
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

add_subdirectory(exampleModule)
add_subdirectory(searchModule)
add_subdirectory(factual_knowledge_similarity_calculation)