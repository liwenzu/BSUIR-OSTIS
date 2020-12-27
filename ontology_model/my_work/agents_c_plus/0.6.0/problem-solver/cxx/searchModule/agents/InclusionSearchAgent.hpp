/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include <sc-memory/kpm/sc_agent.hpp>

#include "myodes/myodes.hpp"
#include "InclusionSearchAgent.generated.hpp"

namespace searchModule
{

class InclusionSearchAgent : public ScAgent
{
  SC_CLASS(Agent, Event(Myodes::question_find_inclusion, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()
};

} // namespace exampleModule
