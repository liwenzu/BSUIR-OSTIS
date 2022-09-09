/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>

#include "WhatIsThatAgent.hpp"
#include "myodes/myodes.hpp"
#include "../AnswerVerificationModule/utils/Display.hpp"
#include "../AnswerVerificationModule/utils/IteratorUtilsLocal.hpp"

using namespace std;
using namespace utils;
using namespace answerVerificationModule;

namespace searchModule {

    SC_AGENT_IMPLEMENTATION(WhatIsThatAgent) {
        if (!edgeAddr.IsValid())
            return SC_RESULT_ERROR;

        ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
        ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);
        if (!param.IsValid())
            return SC_RESULT_ERROR_INVALID_PARAMS;

        ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, param);

        ScIterator3Ptr iterator3 = ms_context->Iterator3(param, ScType::EdgeDCommonConst, ScType::Unknown);
        while (iterator3->Next())
        {
            const auto &endAddr = iterator3->Get(2);
            const auto &arcAddr = iterator3->Get(1);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, endAddr);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);

            const auto &elementList = IteratorUtilsLocal::getAllWithTypeIn(ms_context.get(), endAddr, ScType::NodeConstClass);

            for (const auto &elem : elementList)
            {
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, elem);

                const auto &tempAddr = IteratorUtilsLocal::getArc(ms_context.get(), elem, endAddr);

                if (tempAddr.IsValid())
                {
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, tempAddr);
                }
            }

            const auto &relationAddr = IteratorUtilsLocal::getFirstFromSetByInReWithType(ms_context.get(), arcAddr, ScType::NodeNoRole);
            if (relationAddr.IsValid())
            {
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, relationAddr);

                const auto &tempAddr = IteratorUtilsLocal::getArc(ms_context.get(), relationAddr, arcAddr);
                if (tempAddr.IsValid())
                {
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, tempAddr);
                }
            }
        }

        const auto &elementList = IteratorUtils::getAllWithType(ms_context.get(), param, ScType::Unknown);
        for (const auto &elem : elementList)
        {
            if (elem.IsValid())
            {
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, elem);
                const auto &tempAddr = IteratorUtilsLocal::getArc(ms_context.get(), param, elem);
                if (tempAddr.IsValid())
                {
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, tempAddr);
                }

                const auto &relationAddr = IteratorUtilsLocal::getFirstFromSetByInReWithType(ms_context.get(), tempAddr, ScType::NodeRole);
                if (relationAddr.IsValid())
                {
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, relationAddr);
                    const auto &tempArcAddr = IteratorUtilsLocal::getArc(ms_context.get(), relationAddr, tempAddr);
                    if (tempArcAddr.IsValid())
                    {
                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, tempArcAddr);
                    }
                }

                //上面是关键元素的节点

                if (elem == Myodes::logic_rules_for_generating_test_questions ||
                    elem == Myodes::scope_of_test_questions)
                {
                    continue;
                }

                const auto &subElementList = IteratorUtils::getAllWithType(ms_context.get(), elem, ScType::Unknown);

                for (const auto &subElement : subElementList)
                {
                    if (subElement.IsValid())
                    {
                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, subElement);

                        const auto &tempArcAddr = IteratorUtilsLocal::getArc(ms_context.get(), elem, subElement);
                        if (tempArcAddr.IsValid())
                        {
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, tempArcAddr);
                        }

                        const  auto &powerAddr = IteratorUtils::getFirstByInRelation(ms_context.get(), subElement, Myodes::nrel_measure);
                        if (powerAddr.IsValid())
                        {
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, powerAddr);

                            const auto &subEdgeAddr = IteratorUtilsLocal::getEdge(ms_context.get(), powerAddr, subElement);
                            if (subEdgeAddr.IsValid())
                            {
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, subEdgeAddr);
                            }

                            const auto &arcAddr = IteratorUtilsLocal::getArc(ms_context.get(), Myodes::nrel_measure, subEdgeAddr);
                            if (arcAddr.IsValid())
                            {
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
                            }
                        }

                        const auto &numAddr = IteratorUtilsLocal::getArc(ms_context.get(), Myodes::number, subElement);

                        if (numAddr.IsValid())
                        {
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, numAddr);
                        }

                        const auto &questionList = IteratorUtils::getAllWithType(ms_context.get(), powerAddr, ScType::NodeConstClass);

                        for (const auto &currElem : questionList)
                        {
                            if (currElem.IsValid())
                            {
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, currElem);
                                const auto &arcAddr = IteratorUtilsLocal::getArc(ms_context.get(), powerAddr, currElem);
                                if (arcAddr.IsValid())
                                {
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
                                }
                            }
                        }
                    }
                }
                //创建关系到界面显示
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_measure);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::number);
            }
        }
        AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
        return SC_RESULT_OK;
    }
}
