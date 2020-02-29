/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include <sc-memory/cpp/kpm/sc_agent.hpp>

#include "keynodes/decisionMakingKeynodes.hpp"
#include "keynodes/coreKeynodes.hpp"
#include "ADecisionMakingPreparation.generated.hpp"


namespace decisionMaking
{

class ADecisionMakingPreparation : public ScAgent
{
  SC_CLASS(Agent, Event(DecisionMakingKeynodes::question_decision_making_preparation, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  void addToCheckedProductSet(ScAddr & controlPoint, ScAddr & product);

  bool canStartDecisionMaking(ScAddr & controlPoint);

  ScAddr createProduct(ScAddr & markingLink);
};

} // namespace decisionMaking
