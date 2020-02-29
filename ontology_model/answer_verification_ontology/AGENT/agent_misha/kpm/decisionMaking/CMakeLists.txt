set(SC_DECISION_MAKING_SRC "${KPM_ROOT}/decisionMaking")

set(SOURCES
		"decisionMaking.cpp"
		"keynodes/decisionMakingKeynodes.cpp"
		"keynodes/coreKeynodes.cpp"
		"agents/ADecisionMakingPreparation.cpp"
		"agents/ADecisionMaking.cpp"
		"agents/ATemplateGeneration.cpp"
		"agents/ANumberComparison.cpp"
		"agents/AMarkCorrectness.cpp"
		"agents/AReverseInference.cpp"
		"utils/ProductUtils.cpp"
		"utils/IteratorUtils.cpp"
		"utils/AgentUtils.cpp"
		"utils/CommonUtils.cpp"
		"utils/LogicRuleUtils.cpp"
		)

set(HEADERS
		"decisionMaking.hpp"
		"keynodes/decisionMakingKeynodes.hpp"
		"keynodes/coreKeynodes.hpp"
		"agents/ADecisionMakingPreparation.hpp"
		"agents/ADecisionMaking.hpp"
		"agents/ATemplateGeneration.hpp"
		"agents/ANumberComparison.hpp"
		"agents/AMarkCorrectness.hpp"
		"agents/AReverseInference.hpp"
		"utils/ProductUtils.hpp"
		"utils/IteratorUtils.hpp"
		"utils/AgentUtils.hpp"
		"utils/CommonUtils.hpp"
		"utils/LogicRuleUtils.hpp"
		)

include_directories(${SC_DECISION_MAKING_SRC} ${SC_MEMORY_SRC} ${GLIB2_INCLUDE_DIRS})

add_library(decisionMaking SHARED ${SOURCES} ${HEADERS})
add_dependencies(decisionMaking sc-memory-cpp sc-kpm-common)
target_link_libraries(decisionMaking sc-memory-cpp sc-kpm-common)

sc_codegen(decisionMaking ${SC_DECISION_MAKING_SRC})

