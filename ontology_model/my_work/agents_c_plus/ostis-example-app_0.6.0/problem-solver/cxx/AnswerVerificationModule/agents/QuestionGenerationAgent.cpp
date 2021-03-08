//
// Created by root on 3/6/21.
//
/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include "utils/Display.hpp"
#include "QuestionGenerationAgent.hpp"
#include "keynodes/keynodes.hpp"
#include "sc-agents-common/utils/LogicRuleUtils.hpp"

using namespace std;
using namespace utils;

namespace answerVerificationModule {

    SC_AGENT_IMPLEMENTATION(QuestionGenerationAgent) {
            if (!edgeAddr.IsValid())
                return SC_RESULT_ERROR;
            ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
            ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);
            if (!param.IsValid())
                return SC_RESULT_ERROR_INVALID_PARAMS;
            ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
//            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, param);

            ScAddr initStruct = LogicRuleUtils::getIfStatement(ms_context.get(), param);
            ScAddr resultStruct = LogicRuleUtils::getElseStatement(ms_context.get(), param);

            ScTemplate initStructTemplate;
            if (ms_context->HelperBuildTemplate(initStructTemplate, initStruct))
            {
                ScTemplateSearchResult searchResult;
                if (ms_context->HelperSearchTemplate(initStructTemplate, searchResult))
                {

                    cout << "Hello searchResult" << endl;
                    cout << searchResult.Size() << endl;

                    ScTemplate resultStructTemplate;
                    ScTemplateParams  templateParams;
                    string str = "12345";
                    string str1="_op";
                    ScTemplateSearchResultItem searchResultItem(searchResult[0]);
                    for (int i=0; i<5; i++)
                    {
                        ScAddr elem = searchResultItem[str1+str[i]];
                        templateParams.Add(str1+str[i], elem);
                    }
                    ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                    ScTemplateGenResult genResult;
                    if (ms_context->HelperGenTemplate(resultStructTemplate, genResult, templateParams))
                    {
                        cout << "Hello genResult" << endl;
                        cout << genResult.Size() << endl;

                        for (int i=0; i<genResult.Size(); i++)
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, genResult[i]);
                    }
                }
            }

            cout << "Hello world" << endl;


            AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
            return SC_RESULT_OK;
    }
}



//                    for (int i=0; i<1; i++)
//                    {
//                        ScTemplateSearchResultItem searchResultItem = searchResult[i];
//
//                        ScAddr elem1 = searchResultItem["_op1"];
//                        Display::printNl(ms_context.get(), elem1);
//                        cout << " " << endl;
//                        ScAddr elem2 = searchResultItem["_op2"];
//                        Display::printNl(ms_context.get(), elem2);
//                        cout << " " << endl;
//                        ScAddr elem3 = searchResultItem["_op3"];
//                        Display::printNl(ms_context.get(), elem3);
//                        cout << " " << endl;
//                        ScAddr elem4 = searchResultItem["_op4"];
//                        Display::printNl(ms_context.get(), elem4);
//                        cout << " " << endl;
//                        ScAddr elem5 = searchResultItem["_op5"];
//                        Display::printNl(ms_context.get(), elem5);
//                        cout << " " << endl;
//                        for (int j=0; j<searchResultItem.Size();j++)
//                        {
//                            ScAddr elem = searchResultItem[j];
//                            if (!ms_context->HelperCheckEdge(answer, elem, ScType::EdgeAccessConstPosPerm))
//                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, elem);
//                        }
//                    }