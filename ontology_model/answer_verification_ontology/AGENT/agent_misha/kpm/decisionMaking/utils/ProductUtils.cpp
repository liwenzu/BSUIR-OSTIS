/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include <sc-memory/cpp/sc_memory.hpp>

#include "AgentUtils.hpp"
#include "ProductUtils.hpp"
#include "keynodes/decisionMakingKeynodes.hpp"


using namespace std;
using namespace decisionMaking;

namespace decisionMaking
{

bool ProductUtils::isProduct(ScMemoryContext * context, ScAddr & product)
{
  return product.IsValid()
         &&
         context->HelperCheckEdge(DecisionMakingKeynodes::concept_product, product, ScType::EdgeAccessConstPosPerm);
}

bool ProductUtils::isCorrectlyMarkedProduct(ScMemoryContext * context, ScAddr & product)
{
  vector<ScAddr> params = { product };
  utils::AgentUtils::initAgentAndWaitResult(
        context,
        DecisionMakingKeynodes::question_correct_marking,
        params);
  return context->HelperCheckEdge(
        DecisionMakingKeynodes::concept_correctly_marked_product,
        product,
        ScType::EdgeAccessConstPosPerm);
}

}
