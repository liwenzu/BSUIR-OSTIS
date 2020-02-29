/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include <sc-memory/cpp/sc_stream.hpp>
#include <iostream>

#include "utils/IteratorUtils.hpp"
#include "utils/AgentUtils.hpp"
#include "utils/ProductUtils.hpp"
#include "utils/CommonUtils.hpp"
#include "ADecisionMakingPreparation.hpp"

using namespace utils;
using namespace std;

namespace decisionMaking
{

void ADecisionMakingPreparation::addToCheckedProductSet(ScAddr & controlPoint, ScAddr & product)
{
  ScAddr checkedProducts = IteratorUtils::getFirstByOutRelation(
        ms_context.get(),
        controlPoint,
        DecisionMakingKeynodes::nrel_checked_products);
  if (checkedProducts.IsValid())
  {
    ScAddr accessArc = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, checkedProducts, product);
    ScIterator5Ptr iterator5 = IteratorUtils::getIterator5(
          ms_context.get(),
          checkedProducts,
          CoreKeynodes::rrel_last_added_sc_element);
    if (iterator5->Next())
    {
      ms_context->EraseElement(iterator5->Get(3));
      ScAddr gotoArc = ms_context->CreateEdge(ScType::EdgeDCommonConst, iterator5->Get(2), product);
      ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, CoreKeynodes::nrel_basic_sequence, gotoArc);
    }
    else
      ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, CoreKeynodes::rrel_1, accessArc);

    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, CoreKeynodes::rrel_last_added_sc_element, accessArc);
  }
}

bool ADecisionMakingPreparation::canStartDecisionMaking(ScAddr & controlPoint)
{
  ScAddr currentProduct = IteratorUtils::getFirstByOutRelation(
        ms_context.get(),
        controlPoint,
        DecisionMakingKeynodes::nrel_product_for_check);
  return !currentProduct.IsValid();
}

ScAddr ADecisionMakingPreparation::createProduct(ScAddr & markingLink)
{
  ScAddr product = ms_context->CreateNode(ScType::NodeConst);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, DecisionMakingKeynodes::concept_product, product);

  ScAddr marking = ms_context->CreateNode(ScType::NodeConst);
  ScAddr arc = ms_context->CreateEdge(ScType::EdgeDCommonConst, product, marking);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, DecisionMakingKeynodes::nrel_marking, arc);

  arc = ms_context->CreateEdge(ScType::EdgeDCommonConst, marking, markingLink);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, CoreKeynodes::nrel_idtf, arc);
  return product;
}

SC_AGENT_IMPLEMENTATION(ADecisionMakingPreparation)
{
  if (!edgeAddr.IsValid())
    return SC_RESULT_ERROR;

  ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
  ScAddr markingLink = IteratorUtils::getFirstByOutRelation(ms_context.get(), questionNode, CoreKeynodes::rrel_1);
  ScAddr controlPoint = IteratorUtils::getFirstByOutRelation(ms_context.get(), questionNode, CoreKeynodes::rrel_2);
  if (!markingLink.IsValid() || !controlPoint.IsValid())
    return SC_RESULT_ERROR_INVALID_PARAMS;

  cout << CommonUtils::readString(ms_context.get(), markingLink) << endl;
  ScAddr product = createProduct(markingLink);
  // TODO: try to remove it
  while (!canStartDecisionMaking(controlPoint));

  ScAddr commonArc = ms_context->CreateEdge(ScType::EdgeDCommonConst, controlPoint, product);
  ScAddr accessArc = ms_context->CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        DecisionMakingKeynodes::nrel_product_for_check,
        commonArc);

  AgentUtils::initAgentAndWaitResult(ms_context.get(), DecisionMakingKeynodes::question_decision_making, { product });

  ms_context->EraseElement(accessArc);
  ms_context->EraseElement(commonArc);

  addToCheckedProductSet(controlPoint, product);

  AgentUtils::finishAgentWork(ms_context.get(), questionNode, product);
  return SC_RESULT_OK;
}
}
