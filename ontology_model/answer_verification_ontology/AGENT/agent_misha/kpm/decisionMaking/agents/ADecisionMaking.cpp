/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "ADecisionMaking.hpp"
#include "utils/IteratorUtils.hpp"
#include "utils/AgentUtils.hpp"
#include "utils/ProductUtils.hpp"
#include "utils/CommonUtils.hpp"

#include <sc-memory/cpp/sc_stream.hpp>
#include <iostream>

using namespace utils;
using namespace std;

namespace decisionMaking
{

ScAddr ADecisionMaking::createAnswer(ScAddr & decision)
{
  ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
  if (decision.IsValid())
  {
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, decision);
    IteratorUtils::addNodeWithOutRelationToOutline(
          ms_context.get(),
          decision,
          DecisionMakingKeynodes::nrel_decision_object,
          answer);
    IteratorUtils::addNodeWithOutRelationToOutline(
          ms_context.get(),
          decision,
          DecisionMakingKeynodes::nrel_decision_messages,
          answer);
    IteratorUtils::addNodeWithOutRelationToOutline(
          ms_context.get(),
          decision,
          DecisionMakingKeynodes::nrel_decision_programs,
          answer);
    IteratorUtils::addNodeWithOutRelationToOutline(
          ms_context.get(),
          decision,
          DecisionMakingKeynodes::nrel_decision_tree,
          answer);
  }
  return answer;
}

ScAddr ADecisionMaking::makeDecision(ScAddr & product)
{
  ScAddr request = ms_context->CreateNode(ScType::NodeConstStruct);
  ScAddr decisionVar = ms_context->CreateNode(ScType::NodeVar);
  ScAddr commonArc = ms_context->CreateEdge(ScType::EdgeDCommonVar, decisionVar, product);
  ScAddr accessArc = ms_context->CreateEdge(
        ScType::EdgeAccessVarPosPerm,
        decisionMaking::DecisionMakingKeynodes::nrel_decision_object,
        commonArc);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, request, decisionVar);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, request, commonArc);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, request, accessArc);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, request, product);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, request, DecisionMakingKeynodes::nrel_decision_object);
  vector<ScAddr> params = { request };
  return AgentUtils::initAgentAndWaitResult(
        ms_context.get(),
        DecisionMakingKeynodes::question_reverse_inference,
        params);
}

void ADecisionMaking::checkUnmarkedProductLimit(ScAddr & product, ScAddr & controlPoint)
{
  if (controlPoint.IsValid())
  {
    ScAddr unmarkedProductSet = IteratorUtils::getFirstByOutRelation(
          ms_context.get(),
          controlPoint,
          DecisionMakingKeynodes::nrel_consecutive_unmarked_products);
    int power = CommonUtils::getPowerOfSet(ms_context.get(), unmarkedProductSet);

    ScAddr unmarkedProductLimit = IteratorUtils::getFirstByOutRelation(
          ms_context.get(),
          controlPoint,
          DecisionMakingKeynodes::nrel_unmarked_products_limit);
    int limit = CommonUtils::readNumber(ms_context.get(), unmarkedProductLimit);
    if (power > limit)
    {
      ms_context->CreateEdge(
            ScType::EdgeAccessConstPosPerm,
            DecisionMakingKeynodes::concept_critical_bad_marking_product,
            product);
      // TODO: Remove dublicate
      ms_context->ForEachIter3(
            unmarkedProductSet,
            ScType::EdgeAccessConstPosPerm, ScType::Unknown,
            [](ScAddr const & src, ScAddr const & edge, ScAddr const & trg)
            {
              ms_context->EraseElement(edge);
            });
    }
  }
}

void ADecisionMaking::clearUnmarkedProductSet(ScAddr & controlPoint)
{
  if (controlPoint.IsValid())
  {
    ScAddr unmarkedProductSet = IteratorUtils::getFirstByOutRelation(
          ms_context.get(),
          controlPoint,
          DecisionMakingKeynodes::nrel_consecutive_unmarked_products);
    if (unmarkedProductSet.IsValid())
      ms_context->ForEachIter3(
            unmarkedProductSet,
            ScType::EdgeAccessConstPosPerm, ScType::Unknown,
            [](ScAddr const & src, ScAddr const & edge, ScAddr const & trg)
            {
              ms_context->EraseElement(edge);
            });
  }
}

SC_AGENT_IMPLEMENTATION(ADecisionMaking)
{
  if (!edgeAddr.IsValid())
    return SC_RESULT_ERROR;

  ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
  ScAddr product = IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);
  if (!ProductUtils::isProduct(ms_context.get(), product))
    return SC_RESULT_ERROR_INVALID_PARAMS;

  ScAddr decision;
  if (!ms_context->HelperCheckEdge(DecisionMakingKeynodes::concept_correctly_marked_product, product,
                                   ScType::EdgeAccess))
  {
    ScAddr controlPoint = IteratorUtils::getFirstByInRelation(
          ms_context.get(),
          product,
          DecisionMakingKeynodes::nrel_product_for_check);
    if (ProductUtils::isCorrectlyMarkedProduct(ms_context.get(), product))
    {
      clearUnmarkedProductSet(controlPoint);
      // Refactor it
      decision = ms_context->CreateNode(ScType::NodeConst);
      ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, decision, DecisionMakingKeynodes::good_flow);
    }
    else
    {
      cout << "Process not correctly marked product" << endl;
      // Move this check to inference level
      checkUnmarkedProductLimit(product, controlPoint);
      ScAddr solutionTree = makeDecision(product);
      decision = IteratorUtils::getFirstByInRelation(
            ms_context.get(),
            product,
            DecisionMakingKeynodes::nrel_decision_object);

      ScAddr arc = ms_context->CreateEdge(ScType::EdgeDCommonConst, decision, solutionTree);
      ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, DecisionMakingKeynodes::nrel_decision_tree, arc);

      ScIterator3Ptr iter3 = ms_context->Iterator3(
            DecisionMakingKeynodes::concept_marked_product,
            ScType::EdgeAccessConstPosPerm,
            product);
      while (iter3->Next())
      {
        ms_context->EraseElement(iter3->Get(1));
        break;
      }
    }
  }

  ScAddr answer = createAnswer(decision);
  AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
  return SC_RESULT_OK;
}
}
