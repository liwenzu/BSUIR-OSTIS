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
#include <random>



using namespace std;
using namespace utils;

namespace answerVerificationModule {

    bool cmp(ScAddr a, ScAddr b)
    {
        return a.Hash() > b.Hash();
    }
    bool equalScAddr(ScAddr a, ScAddr b)
    {
        return a.Hash() == b.Hash();
    }

    SC_AGENT_IMPLEMENTATION(QuestionGenerationAgent) {
        if (!edgeAddr.IsValid())
            return SC_RESULT_ERROR;
        ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
        ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);
        if (!param.IsValid())
            return SC_RESULT_ERROR_INVALID_PARAMS;
        ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
//      ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, param);
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

                ScAddr elemSubDomain = IteratorUtilsLocal::getFirstWithType(ms_context.get(), initStruct, ScType::NodeConstStruct);
                if (ms_context->HelperCheckEdge(param, GenKeynodes::nrel_inclusion, ScType::EdgeAccessConstPosPerm) ||
                    ms_context->HelperCheckEdge(param, GenKeynodes::nrel_strict_inclusion, ScType::EdgeAccessConstPosPerm)) {
                   ScAddr relationStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstNoRole);
                   if (ms_context->HelperCheckEdge(param, GenKeynodes::multiple_choice_questions_with_single_option, ScType::EdgeAccessConstPosPerm))
                   {
                       vector<ScAddr> elemDuplicate;
                       for (int i=0; i<searchResult.Size(); i++)
                       {
                           ScTemplateSearchResultItem searchResultItem = searchResult[i];
                           ScAddr keyElem = searchResultItem["_opkq"];
                           ScAddr elemRelation = searchResultItem["_nrel_inclusion"];
                           vector<ScAddr> keyElemList;
                           ScIterator5Ptr it_5 = ms_context->Iterator5(param, ScType::EdgeAccessConstPosPerm, GenKeynodes::choice_the_correct_option, ScType::EdgeAccessConstPosPerm, GenKeynodes::rrel_key_sc_element);
                           if (it_5->Next())
                           {
                               keyElemList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemSubDomain, GenKeynodes::rrel_not_maximum_studied_object_class);
                               auto  it = find(keyElemList.begin(), keyElemList.end(), keyElem);
                               if (it != keyElemList.end())
                                   keyElemList.erase(it);
                               int delI = 0;
                               for (auto elemLocal : keyElemList)
                               {
                                   ScIterator5Ptr  it_51 = ms_context->Iterator5(keyElem, ScType::EdgeDCommonConst, elemLocal, ScType::EdgeAccessConstPosPerm, relationStruct);
                                   if (it_51->Next())
                                       keyElemList.erase(keyElemList.begin()+delI);
                                   delI++;
                               }
                           }
                           else
                               keyElemList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElem, relationStruct);
                           auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                           if (keyElemList.size() >2 && elemRelation == relationStruct && itDup == elemDuplicate.end())
                           {
                               ScTemplate resultStructTemplate;
                               ScTemplateParams  templateParams;
                               ScAddr correctElem = searchResultItem["_opcs"];
                               templateParams.Add("_opkq", keyElem);
                               templateParams.Add("_opcs", correctElem);
                               string str = "123";
                               string str1="_op";
                               shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
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
                                   elemDuplicate.push_back(keyElem);
                               }
                           }
                       }
                   } else if (ms_context->HelperCheckEdge(param, GenKeynodes::multiple_choice_questions_with_multiple_options, ScType::EdgeAccessConstPosPerm))
                   {
                       vector<ScAddr> elemDuplicate;
                       for (int i=0; i<searchResult.Size(); i++)
                       {
                           ScTemplateSearchResultItem searchResultItem = searchResult[i];
                           ScAddr keyElem = searchResultItem["_opkq"];
                           ScAddr elemRelation = searchResultItem["_nrel_inclusion"];
                           ScAddr elemOptionCS = searchResultItem["_opcs1"];
                           ScAddr elemOption1 = searchResultItem["_op1"];
                           vector<ScAddr> keyElemList;
                           vector<ScAddr> keyElemListCorrect;
                           keyElemList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElem, relationStruct);
                           keyElemListCorrect = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemSubDomain, GenKeynodes::rrel_not_maximum_studied_object_class);
                           if ((!ms_context->HelperCheckEdge(keyElem, elemOptionCS, ScType::EdgeDCommonConst) && !ms_context->HelperCheckEdge(elemSubDomain, elemOptionCS, ScType::EdgeAccessConstPosPerm))
                               || (!ms_context->HelperCheckEdge(keyElem, elemOption1, ScType::EdgeDCommonConst) && !ms_context->HelperCheckEdge(elemSubDomain, elemOption1, ScType::EdgeAccessConstPosPerm)))
                           {
                               vector<ScAddr> elemCsListSub;
                               for (auto elemCs : keyElemList)
                               {
                                   vector<ScAddr> elemCsList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemCs, relationStruct);
                                   for (auto elemCsCp : elemCsList)
                                       elemCsListSub.push_back(elemCsCp);
                               }
                               keyElemList.insert(keyElemList.end(), elemCsListSub.begin(), elemCsListSub.end());
                           }
                           sort(keyElemList.begin(), keyElemList.end(), cmp);
                           keyElemList.erase(unique(keyElemList.begin(), keyElemList.end(), equalScAddr), keyElemList.end());
                           sort(keyElemListCorrect.begin(), keyElemListCorrect.end(), cmp);
                           keyElemListCorrect.erase(unique(keyElemListCorrect.begin(), keyElemListCorrect.end(), equalScAddr), keyElemListCorrect.end());
//Remove duplicate elements
                           for (auto itr = keyElemList.begin(); itr != keyElemList.end(); ++itr) {
                               auto itr1 = find(keyElemListCorrect.begin(), keyElemListCorrect.end(), *itr);
                               if (itr1 != keyElemListCorrect.end()) {
//                                    itr = keyElemList.erase(itr);
                                   keyElemListCorrect.erase(itr1);
                               }
//                                else
//                                    ++itr;
                           }
                           auto  it = find(keyElemList.begin(), keyElemList.end(), keyElem);
                           if (it != keyElemList.end())
                               keyElemList.erase(it);
                           it = find(keyElemListCorrect.begin(), keyElemListCorrect.end(), keyElem);
                           if (it != keyElemListCorrect.end())
                               keyElemListCorrect.erase(it);
                           auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                           if (keyElemList.size() > 2 && keyElemListCorrect.size() > 2 && elemRelation == relationStruct && itDup == elemDuplicate.end())
                           {
                               ScTemplate resultStructTemplate;
                               ScTemplateParams  templateParams;
                               templateParams.Add("_opkq", keyElem);
                               string str = "123";
                               string str1="_op";
                               string str2="_opcs";
                               shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
                               shuffle(keyElemListCorrect.begin(), keyElemListCorrect.end(), std::mt19937(std::random_device()()));
                               ScIterator5Ptr it_51 = ms_context->Iterator5(param, ScType::EdgeAccessConstPosPerm, GenKeynodes::choice_the_correct_option, ScType::EdgeAccessConstPosPerm, GenKeynodes::rrel_key_sc_element);
                               if (it_51->Next())
                               {
                                   for (int j=0; j<2; j++)
                                   {
                                       ScAddr elem = keyElemList[j];
                                       templateParams.Add(str2+str[j], elem);
                                       elem = keyElemListCorrect[j];
                                       templateParams.Add(str1+str[j], elem);
                                   }
                               } else{
                                   for (int j=0; j<2; j++)
                                   {
                                       ScAddr elem = keyElemList[j];
                                       templateParams.Add(str1+str[j], elem);
                                       elem = keyElemListCorrect[j];
                                       templateParams.Add(str2+str[j], elem);
                                   }
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
                                   elemDuplicate.push_back(keyElem);
                               }
                           }
                       }
                   }
                } else if (ms_context->HelperCheckEdge(param, GenKeynodes::nrel_subdividing,
                                                       ScType::EdgeAccessConstPosPerm)) {
                    vector<ScAddr> elemDuplicate;
                    for (int i = 0; i < searchResult.Size(); i++) {
                        ScTemplateSearchResultItem searchResultItem = searchResult[i];
                        ScAddr keyElem = searchResultItem["_opkq"];
                        ScAddr elemTuple1 = searchResultItem["_tuple1"];
                        ScAddr elemTuple2 = searchResultItem["_tuple2"];
                        vector<ScAddr> elemTuple1List = IteratorUtils::getAllWithType(ms_context.get(), elemTuple1, ScType::Unknown);
                        vector<ScAddr> elemTuple2List = IteratorUtils::getAllWithType(ms_context.get(), elemTuple2, ScType::Unknown);
                        auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                        if (itDup == elemDuplicate.end() && elemTuple1List.size() == 2 && elemTuple2List.size() == 2) {
                            ScTemplate resultStructTemplate;
                            ScTemplateParams templateParams;
                            templateParams.Add("_opkq", keyElem);
                            string str = "123";
                            string str1 = "_op";
                            string str2 = "_opcs";
                            for (int j = 0; j < 2; j++) {
                                ScAddr elem = searchResultItem[str1 + str[j]];
                                templateParams.Add(str1 + str[j], elem);
                                elem = searchResultItem[str2 + str[j]];
                                templateParams.Add(str2 + str[j], elem);
                            }
                            ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                            ScTemplateGenResult genResult;
                            if (ms_context->HelperGenTemplate(resultStructTemplate, genResult, templateParams)) {
                                cout << "Hello genResult" << endl;
                                cout << genResult.Size() << endl;
                                for (int k = 0; k < genResult.Size(); k++)
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, genResult[k]);
                                ScAddr elem = genResult["_question_number"];
                                vector<ScAddr> objectQuestion = IteratorUtils::getAllWithType(ms_context.get(), GenKeynodes::objective_questions, ScType::NodeConst);
                                int num = objectQuestion.size();
                                string strQuestion = "Generated_Question";
                                string strNum = to_string(num);
                                ms_context->HelperSetSystemIdtf(strQuestion + strNum, elem);
                                elemDuplicate.push_back(keyElem);
                            }
                        }
                    }
                } else if (ms_context->HelperCheckEdge(param, GenKeynodes::nrel_author, ScType::EdgeAccessConstPosPerm)) {
                    elemSubDomain = IteratorUtilsLocal::getFirstWithType(ms_context.get(), initStruct, ScType::NodeConstClass);
                    ScAddr relationStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstNoRole);
                    if (ms_context->HelperCheckEdge(param, GenKeynodes::multiple_choice_questions_with_single_option, ScType::EdgeAccessConstPosPerm)) {
                        vector<ScAddr> elemDuplicate;
                        for (int i = 0; i < searchResult.Size(); i++) {
                            ScTemplateSearchResultItem searchResultItem = searchResult[i];
                            ScAddr keyElem = searchResultItem["_opkqn"];
                            ScAddr elemRelation = searchResultItem["_nrel_inclusion"];
                            ScAddr elemOptionCS = searchResultItem["_opcsn"];
                            vector<ScAddr> keyElemList;
                            ScIterator5Ptr it_5 = ms_context->Iterator5(param, ScType::EdgeAccessConstPosPerm, GenKeynodes::choice_the_correct_option, ScType::EdgeAccessConstPosPerm, GenKeynodes::rrel_key_sc_element);
                            if (it_5->Next()) {
                                keyElemList = IteratorUtils::getAllWithType(ms_context.get(), elemSubDomain, ScType::NodeConst);
                                auto it = find(keyElemList.begin(), keyElemList.end(), elemOptionCS);
                                if (it != keyElemList.end())
                                    keyElemList.erase(it);
                            } else {
                                keyElemList = IteratorUtils::getAllByInRelation(ms_context.get(), keyElem, elemRelation);
                                vector<ScAddr> keyElemListCorrect = IteratorUtils::getAllWithType(ms_context.get(), elemSubDomain, ScType::NodeConst);
                                auto it = find(keyElemListCorrect.begin(), keyElemListCorrect.end(), keyElem);
                                if (it != keyElemListCorrect.end())
                                    keyElemListCorrect.erase(it);
                                shuffle(keyElemListCorrect.begin(), keyElemListCorrect.end(), std::mt19937(std::random_device()()));
                                elemOptionCS = IteratorUtils::getFirstByInRelation(ms_context.get(), keyElemListCorrect[0], elemRelation);
                            }
                            auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                            if (keyElemList.size() > 2 && elemRelation == relationStruct && itDup == elemDuplicate.end()) {
                                ScTemplate resultStructTemplate;
                                ScTemplateParams  templateParams;
                                templateParams.Add("_opkqn", keyElem);
                                templateParams.Add("_opcsn", elemOptionCS);
                                string str = "123";
                                string str1="_opn";
                                shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
                                for (int j = 0; j < 3; j++) {
                                    ScAddr elem = keyElemList[j];
                                    templateParams.Add(str1 + str[j], elem);
                                }
                                ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                                ScTemplateGenResult genResult;
                                if (ms_context->HelperGenTemplate(resultStructTemplate, genResult, templateParams)) {
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
                                    elemDuplicate.push_back(keyElem);
                                }
                            }
                        }
                    } else {
                        vector<ScAddr> elemDuplicate;
                        for (int i = 0; i < searchResult.Size(); i++) {
                            ScTemplateSearchResultItem searchResultItem = searchResult[i];
                            ScAddr keyElem = searchResultItem["_opkqn"];
                            ScAddr elemRelation = searchResultItem["_nrel_inclusion"];
                            auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                            vector<ScAddr> keyElemList = IteratorUtils::getAllByInRelation(ms_context.get(), keyElem, elemRelation);
                            vector<ScAddr> keyElemListCorrect = IteratorUtils::getAllWithType(ms_context.get(), elemSubDomain, ScType::NodeConst);
                            auto it = find(keyElemListCorrect.begin(), keyElemListCorrect.end(), keyElem);
                            if (it != keyElemListCorrect.end())
                                keyElemListCorrect.erase(it);
                            shuffle(keyElemListCorrect.begin(), keyElemListCorrect.end(), std::mt19937(std::random_device()()));
                            vector<ScAddr> keyElemListCorrectSub;
                            for (auto currElem : keyElemListCorrect)
                            {
                                keyElemListCorrectSub = IteratorUtils::getAllByInRelation(ms_context.get(), currElem, relationStruct);
                                if (keyElemListCorrectSub.size() >= 2)
                                    break;
                            }
                            if (itDup == elemDuplicate.end() && elemRelation == relationStruct && keyElemListCorrectSub.size() >= 2 && keyElemList.size() >= 2) {
                                ScTemplate resultStructTemplate;
                                ScTemplateParams templateParams;
                                templateParams.Add("_opkqn", keyElem);
                                string str = "123";
                                string str1 = "_opn";
                                string str2 = "_opcsn";
                                shuffle(keyElemListCorrectSub.begin(), keyElemListCorrectSub.end(), std::mt19937(std::random_device()()));
                                shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
                                ScIterator5Ptr it_51 = ms_context->Iterator5(param, ScType::EdgeAccessConstPosPerm, GenKeynodes::choice_the_correct_option, ScType::EdgeAccessConstPosPerm, GenKeynodes::rrel_key_sc_element);
                                if (it_51->Next()) {
                                    for (int j = 0; j < 2; j++) {
                                        templateParams.Add(str2 + str[j], keyElemList[j]);
                                        templateParams.Add(str1 + str[j], keyElemListCorrectSub[j]);
                                    }
                                } else{
                                    for (int j = 0; j < 2; j++) {
                                        templateParams.Add(str1 + str[j], keyElemList[j]);
                                        templateParams.Add(str2 + str[j], keyElemListCorrectSub[j]);
                                    }
                                }
                                ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                                ScTemplateGenResult genResult;
                                if (ms_context->HelperGenTemplate(resultStructTemplate, genResult, templateParams)) {
                                    cout << "Hello genResult" << endl;
                                    cout << genResult.Size() << endl;
                                    for (int k = 0; k < genResult.Size(); k++)
                                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, genResult[k]);
                                    ScAddr elem = genResult["_question_number"];
                                    vector<ScAddr> objectQuestion = IteratorUtils::getAllWithType(ms_context.get(), GenKeynodes::objective_questions, ScType::NodeConst);
                                    int num = objectQuestion.size();
                                    string strQuestion = "Generated_Question";
                                    string strNum = to_string(num);
                                    ms_context->HelperSetSystemIdtf(strQuestion + strNum, elem);
                                    elemDuplicate.push_back(keyElem);
                                }
                            }
                        }
                    }
                }






            }
        }

        cout << "Hello world" << endl;


        AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
        return SC_RESULT_OK;
    }
}