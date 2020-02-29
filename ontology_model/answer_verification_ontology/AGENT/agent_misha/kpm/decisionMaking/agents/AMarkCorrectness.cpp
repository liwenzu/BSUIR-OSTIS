/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include <iostream>
#include <algorithm>

#include <sc-memory/cpp/sc_stream.hpp>
#include <sc-memory/cpp/sc_link.hpp>

#include "AMarkCorrectness.hpp"
#include "utils/IteratorUtils.hpp"
#include "utils/AgentUtils.hpp"
#include "utils/ProductUtils.hpp"
#include "utils/CommonUtils.hpp"

using namespace utils;
using namespace std;

namespace decisionMaking
{

bool AMarkCorrectness::isSimilarMarking(const string marking, const string standardMarking)
{
  bool result = false;
  if (marking.length() == standardMarking.length())
  {
    //TODO: Refactor it
    int notSimilarSymbolsNumber = 0;
    for (size_t i = 0; i < marking.length(); i++)
    {
      if (marking[i] != standardMarking[i])
      {
        if (standardMarking[i] == 'D')
        {
          if (!isdigit(marking[i]))
            notSimilarSymbolsNumber++;
        }
        else
          notSimilarSymbolsNumber++;
      }
    }
    if (notSimilarSymbolsNumber <= 2)
      result = true;
  }
  return result;
}

ScAddr AMarkCorrectness::createAnswer(ScAddr & product, ScAddr & correctlyMarkedArc)
{
  ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, product);
  ms_context->CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        answer,
        DecisionMakingKeynodes::concept_correctly_marked_product);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, correctlyMarkedArc);

  return answer;
}

SC_AGENT_IMPLEMENTATION(AMarkCorrectness)
{
  if (!edgeAddr.IsValid())
    return SC_RESULT_ERROR;

  ScAddr question = ms_context->GetEdgeTarget(edgeAddr);
  ScAddr product = IteratorUtils::getFirstFromSet(ms_context.get(), question);
  if (!ProductUtils::isProduct(ms_context.get(), product))
  {
    cout << "No product provided" << endl;
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  ScAddr marking = IteratorUtils::getFirstByOutRelation(
        ms_context.get(),
        product,
        DecisionMakingKeynodes::nrel_marking);
  string markingValue = CommonUtils::getIdtfValue(ms_context.get(), marking, CoreKeynodes::nrel_idtf);
  markingValue = utils::StringUtils::ReplaceAll(markingValue, " ", "");

  ScType isMarkedArc = markingValue.length() == 0 ? ScType::EdgeAccessConstNegPerm : ScType::EdgeAccessConstPosPerm;
  ms_context->CreateEdge(isMarkedArc, DecisionMakingKeynodes::concept_marked_product, product);

  ScAddr controlPoint = IteratorUtils::getFirstByInRelation(
        ms_context.get(),
        product,
        DecisionMakingKeynodes::nrel_product_for_check);
  if (!controlPoint.IsValid())
  {
    std::cout << "Product is not attached to control point" << endl;
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  ScAddr standardMarking = IteratorUtils::getFirstByOutRelation(
        ms_context.get(),
        controlPoint,
        DecisionMakingKeynodes::nrel_standard_marking);
  string standardMarkingValue = CommonUtils::getIdtfValue(ms_context.get(), standardMarking, CoreKeynodes::nrel_idtf);
  standardMarkingValue = utils::StringUtils::ReplaceAll(standardMarkingValue, " ", "");

  bool similarMarkingFlag = isSimilarMarking(markingValue, standardMarkingValue);

  ScAddr correctlyMarkedArc;
  //Refactor this block
  if (similarMarkingFlag)
  {
    correctlyMarkedArc = ms_context->CreateEdge(
          ScType::EdgeAccessConstPosPerm,
          DecisionMakingKeynodes::concept_correctly_marked_product,
          product);
  }
  else
  {
    ScAddr unmarkedSet = IteratorUtils::getFirstByOutRelation(
          ms_context.get(),
          controlPoint,
          DecisionMakingKeynodes::nrel_consecutive_unmarked_products);
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, unmarkedSet, product);
    correctlyMarkedArc = ms_context->CreateEdge(
          ScType::EdgeAccessConstNegPerm,
          DecisionMakingKeynodes::concept_correctly_marked_product,
          product);
  }

  ScAddr answer = createAnswer(product, correctlyMarkedArc);
  AgentUtils::finishAgentWork(ms_context.get(), question, answer);
  return SC_RESULT_OK;
}

} // namespace decisionMaking
