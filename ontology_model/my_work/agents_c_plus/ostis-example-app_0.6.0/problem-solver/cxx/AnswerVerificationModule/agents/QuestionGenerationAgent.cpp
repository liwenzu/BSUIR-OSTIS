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
#include "utils/IteratorUtilsLocal.hpp"
#include <algorithm>



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



                    for (int i=0; i<searchResult.Size(); i+=600)
                    {
                        ScTemplateSearchResultItem searchResultItem = searchResult[i];
                        ScAddr keyElem = searchResultItem["_opkq"];
                        vector<ScAddr> keyElemList;
                        ScIterator5Ptr it_5 = ms_context->Iterator5(param, ScType::EdgeAccessConstPosPerm, GenKeynodes::choice_the_correct_option, ScType::EdgeAccessConstPosPerm, GenKeynodes::rrel_key_sc_element);
                        if (it_5->Next())
                        {
                            keyElemList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), GenKeynodes::subject_domain_of_actions_and_tasks, GenKeynodes::rrel_not_maximum_studied_object_class);
                            auto  it = find(keyElemList.begin(), keyElemList.end(), keyElem);
                            if (it != keyElemList.end())
                                keyElemList.erase(it);
                        }
                        else
                            keyElemList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElem, GenKeynodes::nrel_inclusion);
                        if (keyElemList.size() >2 )
                        {
                            ScTemplate resultStructTemplate;
                            ScTemplateParams  templateParams;
                            ScAddr correctElem = searchResultItem["_opcs"];
                            templateParams.Add("_opkq", keyElem);
                            templateParams.Add("_opcs", correctElem);
                            string str = "123";
                            string str1="_op";

                            for (int j=0; j<3; j++)
                            {
                                ScAddr elem = keyElemList[j];
                                templateParams.Add(str1+str[j], elem);
                            }
                            ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                            ScTemplateGenResult genResult;
                            if (ms_context->HelperGenTemplate(resultStructTemplate, genResult, templateParams))
                            {
                                cout << "Hello genResult" << endl;
                                cout << genResult.Size() << endl;
                                for (int k=0; k<genResult.Size(); k++)
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, genResult[k]);
                                ScAddr elem = genResult["_question_number"];
                                vector<ScAddr> objectQuestion = IteratorUtils::getAllWithType(ms_context.get(), GenKeynodes::objective_questions, ScType::NodeConst);
                                int num = objectQuestion.size();
                                string strQuestion = "Generated_Question";
                                string strNum = to_string(num);
                                ms_context->HelperSetSystemIdtf(strQuestion + strNum, elem);
                            }
//                            if (i > 3) break;



                        }
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



/*                    ScTemplate resultStructTemplate;
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
                        ScAddr elem = genResult["_question_number"];
                        ms_context->HelperSetSystemIdtf("Q_Generated_1", elem);
                    }*/