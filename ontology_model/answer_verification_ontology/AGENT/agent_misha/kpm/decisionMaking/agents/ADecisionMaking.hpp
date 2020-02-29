/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include <sc-memory/cpp/kpm/sc_agent.hpp>

#include "keynodes/decisionMakingKeynodes.hpp"
#include "keynodes/coreKeynodes.hpp"
#include "ADecisionMaking.generated.hpp"


namespace decisionMaking
{

class ADecisionMaking : public ScAgent
{
  SC_CLASS(Agent, Event(DecisionMakingKeynodes::question_decision_making, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  void checkUnmarkedProductLimit(ScAddr & product, ScAddr & controlPoint);

  void clearUnmarkedProductSet(ScAddr & controlPoint);

  ScAddr makeDecision(ScAddr & product);

  ScAddr createAnswer(ScAddr & product);
};

} // namespace decisionMaking
