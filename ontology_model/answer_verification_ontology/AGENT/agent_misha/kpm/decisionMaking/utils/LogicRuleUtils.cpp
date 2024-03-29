/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include <sc-memory/cpp/sc_memory.hpp>

#include "AgentUtils.hpp"
#include "keynodes/coreKeynodes.hpp"
#include "IteratorUtils.hpp"
#include "LogicRuleUtils.hpp"


using namespace std;
using namespace decisionMaking;

namespace utils
{

ScAddr LogicRuleUtils::getIfStatement(ScMemoryContext * context, ScAddr & logicRule)
{
  ScAddr ifStatement;
  ScAddr implEdge;
  implEdge = IteratorUtils::getFirstByOutRelation(context, logicRule, CoreKeynodes::rrel_main_key_sc_element);
  if (context->HelperCheckEdge(CoreKeynodes::nrel_implication, implEdge, ScType::EdgeAccessConstPosPerm))
    ifStatement = context->GetEdgeSource(implEdge);
  return ifStatement;
}

ScAddr LogicRuleUtils::getElseStatement(ScMemoryContext * context, ScAddr & logicRule)
{
  ScAddr implEdge;
  ScAddr elseStatement;
  implEdge = IteratorUtils::getFirstByOutRelation(context, logicRule, CoreKeynodes::rrel_main_key_sc_element);
  if (context->HelperCheckEdge(CoreKeynodes::nrel_implication, implEdge, ScType::EdgeAccessConstPosPerm))
    elseStatement = context->GetEdgeTarget(implEdge);
  return elseStatement;
}


}
