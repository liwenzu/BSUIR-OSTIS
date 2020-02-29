/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "decisionMaking.hpp"
#include "keynodes/decisionMakingKeynodes.hpp"
#include "keynodes/coreKeynodes.hpp"
#include "agents/ANumberComparison.hpp"
#include "agents/ADecisionMakingPreparation.hpp"
#include "agents/ADecisionMaking.hpp"
#include "agents/AMarkCorrectness.hpp"
#include "agents/ATemplateGeneration.hpp"
#include "agents/AReverseInference.hpp"

using namespace decisionMaking;

SC_IMPLEMENT_MODULE(decisionMakingModule)

sc_result decisionMakingModule::InitializeImpl()
{
  if (!decisionMaking::DecisionMakingKeynodes::InitGlobal())
    return SC_RESULT_ERROR;

  if (!decisionMaking::CoreKeynodes::InitGlobal())
    return SC_RESULT_ERROR;

  SC_AGENT_REGISTER(ADecisionMakingPreparation)
  SC_AGENT_REGISTER(ADecisionMaking)
  SC_AGENT_REGISTER(ANumberComparison)
  SC_AGENT_REGISTER(AMarkCorrectness)
  SC_AGENT_REGISTER(ATemplateGeneration)
  SC_AGENT_REGISTER(AReverseInference)

  return SC_RESULT_OK;
}

sc_result decisionMakingModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(ADecisionMakingPreparation)
  SC_AGENT_UNREGISTER(ADecisionMaking)
  SC_AGENT_UNREGISTER(AMarkCorrectness)
  SC_AGENT_UNREGISTER(ANumberComparison)
  SC_AGENT_UNREGISTER(ATemplateGeneration)
  SC_AGENT_UNREGISTER(AReverseInference)

  return SC_RESULT_OK;
}
