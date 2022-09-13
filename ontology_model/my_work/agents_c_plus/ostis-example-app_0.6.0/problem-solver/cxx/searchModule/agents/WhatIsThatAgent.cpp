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
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, param);


// 答案验证的例子
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
            if (elem.IsValid()) {
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, elem);
                const auto &tempAddr = IteratorUtilsLocal::getArc(ms_context.get(), param, elem);
                if (tempAddr.IsValid()) {
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, tempAddr);
                }

                const auto &relationAddr = IteratorUtilsLocal::getFirstFromSetByInReWithType(ms_context.get(), tempAddr,
                                                                                             ScType::NodeRole);
                if (relationAddr.IsValid()) {
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, relationAddr);
                    const auto &tempArcAddr = IteratorUtilsLocal::getArc(ms_context.get(), relationAddr, tempAddr);
                    if (tempArcAddr.IsValid()) {
                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, tempArcAddr);
                    }
                }

                //上面是关键元素的节点

                if (elem == Myodes::semantic_model_exam_tickets)
                {
                    ScIterator3Ptr it3 = ms_context->Iterator3(elem, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
                    if (it3->Next())
                    {
                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, it3->Get(1));
                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, it3->Get(2));
                    }
                }
                else
                {
                    const auto &subElementList = IteratorUtils::getAllWithType(ms_context.get(), elem, ScType::Unknown);
                    for (const auto &subElement : subElementList) {
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
                            if (currElem.IsValid() && (currElem == Myodes::objective_questions ||
                                                        currElem == Myodes::subjective_questions))
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
        }


/*
//        欧几里得集合定义解释例子
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::Test_question_86);

        const auto &elemList = IteratorUtilsLocal::getAllWithTypeIn(ms_context.get(), Myodes::Test_question_86, ScType::NodeConst);
        for (const auto &elem : elemList)
        {
            if (!elem.IsValid())
            {
                continue;
            }

            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, elem);

            ScAddr arcAddr = IteratorUtilsLocal::getArc(ms_context.get(), elem, Myodes::Test_question_86);
            if (!arcAddr.IsValid())
            {
                continue;
            }
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        }

        ScAddr nodeAddr = IteratorUtils::getFirstByOutRelation(ms_context.get(), Myodes::Test_question_86, Myodes::rrel_key_sc_element);

        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, nodeAddr);

        ScAddr tupleAddr;

        ScIterator5Ptr it5 = ms_context->Iterator5(ScType::NodeConstTuple, ScType::EdgeDCommonConst, Myodes::Test_question_86,
                ScType::EdgeAccessConstPosPerm, Myodes::nrel_correct_option);
        if (it5->Next())
        {
            tupleAddr = it5->Get(0);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, tupleAddr);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, it5->Get(1));
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, it5->Get(3));
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_correct_option);
        }

        const auto &elemList1 = IteratorUtils::getAllWithType(ms_context.get(), tupleAddr, ScType::Unknown);
        for (const auto &elem : elemList1)
        {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, elem);
            ScAddr arcAddr = IteratorUtilsLocal::getArc(ms_context.get(), tupleAddr, elem);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
            arcAddr = IteratorUtilsLocal::getArc(ms_context.get(), Myodes::lang_ru, elem);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        }
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::lang_ru);

        it5 = ms_context->Iterator5(Myodes::Test_question_86, ScType::EdgeDCommonConst, ScType::NodeConstStruct,
                ScType::EdgeAccessConstPosPerm, Myodes::nrel_standard_answer);
        if (it5->Next())
        {
            tupleAddr = it5->Get(2);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, tupleAddr);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, it5->Get(1));
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, it5->Get(3));
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, it5->Get(4));
        }

        const auto &elemList2 = IteratorUtils::getAllWithType(ms_context.get(), tupleAddr, ScType::Unknown);
        for (const auto &elem : elemList2)
        {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, elem);
            ScAddr arcAddr = IteratorUtilsLocal::getArc(ms_context.get(), tupleAddr, elem);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        }

        it5 = ms_context->Iterator5(Myodes::Test_question_86, ScType::EdgeDCommonConst, ScType::NodeConstStruct,
                                    ScType::EdgeAccessConstPosPerm, Myodes::nrel_user_answer);
        if (it5->Next())
        {
            tupleAddr = it5->Get(2);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, tupleAddr);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, it5->Get(1));
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, it5->Get(3));
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, it5->Get(4));
        }

        const auto &elemList3 = IteratorUtils::getAllWithType(ms_context.get(), tupleAddr, ScType::Unknown);
        for (const auto &elem : elemList3)
        {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, elem);
            ScAddr arcAddr = IteratorUtilsLocal::getArc(ms_context.get(), tupleAddr, elem);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        }
*/

/*
        //考试票的语义模型
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::concept_examination_paper);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::Test_paper_1);
        ScAddr arcAddr = IteratorUtilsLocal::getArc(ms_context.get(), Myodes::concept_examination_paper, Myodes::Test_paper_1);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);

        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::rrel_1);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_basic_sequence);

        ScAddr nodeAddr = IteratorUtils::getFirstByOutRelation(ms_context.get(), Myodes::Test_paper_1, Myodes::rrel_1);

        if (nodeAddr.IsValid())
        {
            arcAddr = IteratorUtilsLocal::getArc(ms_context.get(), Myodes::Test_paper_1, nodeAddr);
            if (arcAddr.IsValid())
            {
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
                arcAddr = IteratorUtilsLocal::getArc(ms_context.get(), Myodes::rrel_1, arcAddr);
                if (arcAddr.IsValid())
                {
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
                }
            }
        }

        const auto &elemList = IteratorUtils::getAllWithType(ms_context.get(), Myodes::Test_paper_1, ScType::Unknown);

        for (const auto &elem : elemList)
        {
            if (!elem.IsValid())
            {
                continue;
            }
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, elem);
            arcAddr = IteratorUtilsLocal::getArc(ms_context.get(), Myodes::Test_paper_1, elem);
            if (!arcAddr.IsValid())
            {
                continue;
            }
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);

            nodeAddr = IteratorUtilsLocal::getFirstFromSetByInReWithType(ms_context.get(), elem, ScType::NodeConstClass);

            if (nodeAddr.IsValid())
            {
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, nodeAddr);
                arcAddr = IteratorUtilsLocal::getArc(ms_context.get(), nodeAddr, elem);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
            }

            nodeAddr = IteratorUtils::getFirstByOutRelation(ms_context.get(), elem, Myodes::nrel_basic_sequence);
            if (nodeAddr.IsValid())
            {
                ScAddr edge1Addr = IteratorUtilsLocal::getEdge(ms_context.get(), elem, nodeAddr);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, edge1Addr);
                arcAddr = IteratorUtilsLocal::getArc(ms_context.get(), Myodes::nrel_basic_sequence, edge1Addr);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
            }
        }
*/

/*  快速生成测试问题参数选择
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
*/


        AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
        return SC_RESULT_OK;
    }
}
