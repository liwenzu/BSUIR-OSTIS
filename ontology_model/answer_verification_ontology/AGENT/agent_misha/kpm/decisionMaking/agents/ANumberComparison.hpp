/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include <sc-memory/cpp/kpm/sc_agent.hpp>

#include "keynodes/decisionMakingKeynodes.hpp"
#include "keynodes/coreKeynodes.hpp"
#include "ANumberComparison.generated.hpp"

namespace decisionMaking
{

class ANumberComparison : public ScAgent
{
  SC_CLASS(Agent, Event(DecisionMakingKeynodes::question_numbers_comparison, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()
};

} // namespace decisionMaking
