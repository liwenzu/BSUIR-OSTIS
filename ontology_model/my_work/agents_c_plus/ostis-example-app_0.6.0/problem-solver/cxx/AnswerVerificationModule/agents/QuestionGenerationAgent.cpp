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
#include "utils/QuestionGenerationProcess.hpp"
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
//        ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);

        ScAddr param = IteratorUtils::getFirstByOutRelation(ms_context.get(), questionNode, Keynodes::rrel_1);
        ScAddr paramSubDom = IteratorUtils::getFirstByOutRelation(ms_context.get(), questionNode, Keynodes::rrel_2);
        ScAddr paramRolRel = IteratorUtils::getFirstByOutRelation(ms_context.get(), questionNode, Keynodes::rrel_3);
        ScAddr paramLanguage = QuestionGenerationProcess::usedLanguage(ms_context.get());
        if (!param.IsValid() || !paramSubDom.IsValid() || !paramRolRel.IsValid())
            return SC_RESULT_ERROR_INVALID_PARAMS;
        ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
//      ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, param);
        ScAddr initStruct = LogicRuleUtils::getIfStatement(ms_context.get(), param);
        ScAddr resultStruct = LogicRuleUtils::getElseStatement(ms_context.get(), param);
        ScTemplateParams  templateParamSearch;
        templateParamSearch.Add("_subject_domain_variable", paramSubDom);
        templateParamSearch.Add("_role_relation_variable", paramRolRel);
        ScTemplate initStructTemplate;
        if (ms_context->HelperBuildTemplate(initStructTemplate, initStruct, templateParamSearch))
        {
            ScTemplateSearchResult searchResult;
            if (ms_context->HelperSearchTemplate(initStructTemplate, searchResult))
            {

                cout << "Hello searchResult" << endl;
                cout << searchResult.Size() << endl;

                if (ms_context->HelperCheckEdge(param, GenKeynodes::multiple_choice_questions, ScType::EdgeAccessConstPosPerm)) {
//                    ScAddr elemSubDomain = IteratorUtilsLocal::getFirstWithType(ms_context.get(), initStruct, ScType::NodeConstStruct);
                    ScAddr elemSubDomain = paramSubDom;
                    ScAddr roleStruct = paramRolRel;
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
                                    keyElemList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemSubDomain, paramRolRel);
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
                                sort(keyElemList.begin(), keyElemList.end(), cmp);
                                keyElemList.erase(unique(keyElemList.begin(), keyElemList.end(), equalScAddr), keyElemList.end());
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
                                    QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
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
                                keyElemListCorrect = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemSubDomain, roleStruct);
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
                                for (auto itr : keyElemList) {
                                    auto itr1 = find(keyElemListCorrect.begin(), keyElemListCorrect.end(), itr);
                                    if (itr1 != keyElemListCorrect.end()) {
                                        keyElemListCorrect.erase(itr1);
                                    }
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
                                    QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
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
                                QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
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
                                    QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
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
                                    QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                }
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::nrel_idtf, ScType::EdgeAccessConstPosPerm)) {
                        elemSubDomain = paramSubDom;
                        roleStruct = paramRolRel;
                        ScAddr relationStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstNoRole);
                        if (ms_context->HelperCheckEdge(param, GenKeynodes::multiple_choice_questions_based_on_set_identifier, ScType::EdgeAccessConstPosPerm)) {
                            if (ms_context->HelperCheckEdge(param, GenKeynodes::multiple_choice_questions_with_single_option, ScType::EdgeAccessConstPosPerm)) {
                                vector<ScAddr> elemDuplicate;
                                for (int i = 0; i < searchResult.Size(); i++) {
                                    ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                    ScAddr keyElem = searchResultItem["_opkq"];
                                    ScAddr elemOptionCS = searchResultItem["_opcsn"];
                                    if (!ms_context->HelperCheckEdge(paramLanguage, elemOptionCS, ScType::EdgeAccessConstPosPerm)) {
                                        ScIterator5Ptr it_5 = ms_context->Iterator5(keyElem, ScType::EdgeDCommonConst, ScType::Unknown, ScType::EdgeAccessConstPosPerm, relationStruct);
                                        bool flag = false;
                                        while (it_5->Next()) {
                                            if (ms_context->HelperCheckEdge(paramLanguage, it_5->Get(2), ScType::EdgeAccessConstPosPerm)) {
                                                elemOptionCS = it_5->Get(2);
                                                flag = true;
                                                break;
                                            }
                                        }
                                        if (!flag)
                                            continue;
                                    }
                                    vector<ScAddr> keyElemList;
                                    ScIterator5Ptr it_5 = ms_context->Iterator5(param, ScType::EdgeAccessConstPosPerm, GenKeynodes::choice_the_correct_option, ScType::EdgeAccessConstPosPerm, GenKeynodes::rrel_key_sc_element);
                                    if (it_5->Next()) {
                                        keyElemList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemSubDomain, roleStruct);
                                        auto it = find(keyElemList.begin(), keyElemList.end(), keyElem);
                                        if (it != keyElemList.end())
                                            keyElemList.erase(it);
                                    } else {
                                        ScIterator5Ptr it_5l = ms_context->Iterator5(keyElem, ScType::EdgeDCommonConst, ScType::Unknown, ScType::EdgeAccessConstPosPerm, relationStruct);
                                        while (it_5l->Next()) {
                                            if (ms_context->HelperCheckEdge(paramLanguage, it_5l->Get(2), ScType::EdgeAccessConstPosPerm))
                                                keyElemList.push_back(it_5l->Get(2));
                                        }
                                        vector<ScAddr> keyElemListSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemSubDomain, roleStruct);
                                        auto it = find(keyElemListSub.begin(), keyElemListSub.end(), keyElem);
                                        if (it != keyElemListSub.end())
                                            keyElemListSub.erase(it);
                                        shuffle(keyElemListSub.begin(), keyElemListSub.end(), std::mt19937(std::random_device()()));
                                        ScAddr elemSub = keyElemListSub[0];
                                        it_5l = ms_context->Iterator5(elemSub, ScType::EdgeDCommonConst, ScType::Unknown, ScType::EdgeAccessConstPosPerm, GenKeynodes::nrel_main_idtf);
                                        while (it_5l->Next()) {
                                            if (ms_context->HelperCheckEdge(paramLanguage, it_5l->Get(2), ScType::EdgeAccessConstPosPerm)) {
                                                elemOptionCS = it_5l->Get(2);
                                                break;
                                            }
                                        }
                                    }
                                    auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                    if (keyElemList.size() > 2 && itDup == elemDuplicate.end()) {
                                        ScTemplate resultStructTemplate;
                                        ScTemplateParams templateParams;
                                        templateParams.Add("_opkq", keyElem);
                                        templateParams.Add("_opcsn", elemOptionCS);
                                        string str = "123";
                                        string str1 = "_opn";
                                        shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
                                        ScIterator5Ptr it_5c = ms_context->Iterator5(param, ScType::EdgeAccessConstPosPerm, GenKeynodes::choice_the_correct_option, ScType::EdgeAccessConstPosPerm, GenKeynodes::rrel_key_sc_element);
                                        if (it_5c->Next()) {
                                            for (int j = 0; j < 3; j++) {
                                                ScAddr elem = keyElemList[j];
                                                vector<ScAddr> idtfList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elem, GenKeynodes::nrel_main_idtf);
                                                for (auto elemCp : idtfList) {
                                                    if (ms_context->HelperCheckEdge(paramLanguage, elemCp, ScType::EdgeAccessConstPosPerm)) {
                                                        elem = elemCp;
                                                        break;
                                                    }
                                                }
                                                templateParams.Add(str1 + str[j], elem);
                                            }
                                        } else{
                                            for (int j = 0; j < 3; j++)
                                                templateParams.Add(str1 + str[j], keyElemList[j]);
                                        }
                                        ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                                        QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                    }
                                }
                            } else{
                                vector<ScAddr> elemDuplicate;
                                for (int i = 0; i < searchResult.Size(); i++) {
                                    ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                    ScAddr keyElem = searchResultItem["_opkq"];
                                    vector<ScAddr> keyElemList;
                                    vector<ScAddr> keyElemListCorrect;
                                    vector<ScAddr> keyElemListSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElem, relationStruct);
                                    for (auto currElem : keyElemListSub){
                                        if (ms_context->HelperCheckEdge(paramLanguage, currElem, ScType::EdgeAccessConstPosPerm))
                                            keyElemList.push_back(currElem);
                                    }
                                    vector<ScAddr> keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemSubDomain, roleStruct);
                                    auto it = find(keyElemListCorrectSub.begin(), keyElemListCorrectSub.end(), keyElem);
                                    if (it != keyElemListCorrectSub.end())
                                        keyElemListCorrectSub.erase(it);
                                    shuffle(keyElemListCorrectSub.begin(), keyElemListCorrectSub.end(), std::mt19937(std::random_device()()));
                                    for (auto j : keyElemListCorrectSub) {
                                        vector<ScAddr> idtfList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), j, relationStruct);
                                        for (auto elemCp : idtfList) {
                                            if (ms_context->HelperCheckEdge(paramLanguage, elemCp, ScType::EdgeAccessConstPosPerm)) {
                                                keyElemListCorrect.push_back(elemCp);
                                            }
                                        }
                                        if (keyElemListCorrect.size() >= 2)
                                            break;
                                    }
                                    auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                    if (itDup == elemDuplicate.end() && keyElemListCorrect.size() >= 2 && keyElemList.size() >= 2) {
                                        ScTemplate resultStructTemplate;
                                        ScTemplateParams templateParams;
                                        templateParams.Add("_opkq", keyElem);
                                        string str = "123";
                                        string str1 = "_opn";
                                        string str2 = "_opcsn";
                                        shuffle(keyElemListCorrect.begin(), keyElemListCorrect.end(), std::mt19937(std::random_device()()));
                                        shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
                                        ScIterator5Ptr it_5c = ms_context->Iterator5(param, ScType::EdgeAccessConstPosPerm, GenKeynodes::choice_the_correct_option, ScType::EdgeAccessConstPosPerm, GenKeynodes::rrel_key_sc_element);
                                        if (it_5c->Next()) {
                                            for (int j = 0; j < 2; j++) {
                                                templateParams.Add(str2 + str[j], keyElemList[j]);
                                                templateParams.Add(str1 + str[j], keyElemListCorrect[j]);
                                            }
                                        } else {
                                            for (int j = 0; j < 2; j++) {
                                                templateParams.Add(str2 + str[j], keyElemListCorrect[j]);
                                                templateParams.Add(str1 + str[j], keyElemList[j]);
                                            }
                                        }
                                        ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                                        QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                    }
                                }
                            }
                        } else {
                            if (ms_context->HelperCheckEdge(param, GenKeynodes::multiple_choice_questions_with_single_option, ScType::EdgeAccessConstPosPerm)) {
                                vector<ScAddr> elemDuplicate;
                                for (int i = 0; i < searchResult.Size(); i++) {
                                    ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                    ScAddr keyElem = searchResultItem["_opkqr"];
                                    ScAddr elemOptionCS = searchResultItem["_opcsn"];
                                    if (!ms_context->HelperCheckEdge(paramLanguage, elemOptionCS, ScType::EdgeAccessConstPosPerm)) {
                                        ScIterator5Ptr it_5 = ms_context->Iterator5(keyElem, ScType::EdgeDCommonConst, ScType::Unknown, ScType::EdgeAccessConstPosPerm, relationStruct);
                                        bool flag = false;
                                        while (it_5->Next()) {
                                            if (ms_context->HelperCheckEdge(paramLanguage, it_5->Get(2), ScType::EdgeAccessConstPosPerm)) {
                                                elemOptionCS = it_5->Get(2);
                                                flag = true;
                                                break;
                                            }
                                        }
                                        if (!flag)
                                            continue;
                                    }
                                    vector<ScAddr> keyElemList;
                                    ScIterator5Ptr it_5 = ms_context->Iterator5(param, ScType::EdgeAccessConstPosPerm, GenKeynodes::choice_the_correct_option, ScType::EdgeAccessConstPosPerm, GenKeynodes::rrel_key_sc_element);
                                    if (it_5->Next()) {
                                        keyElemList = IteratorUtilsLocal::getAllByOutRelationWithType(ms_context.get(), elemSubDomain, roleStruct, ScType::NodeConstNoRole);
                                        auto it = find(keyElemList.begin(), keyElemList.end(), keyElem);
                                        if (it != keyElemList.end())
                                            keyElemList.erase(it);
                                    } else {
                                        ScIterator5Ptr it_5l = ms_context->Iterator5(keyElem, ScType::EdgeDCommonConst, ScType::Unknown, ScType::EdgeAccessConstPosPerm, relationStruct);
                                        while (it_5l->Next()) {
                                            if (ms_context->HelperCheckEdge(paramLanguage, it_5l->Get(2), ScType::EdgeAccessConstPosPerm))
                                                keyElemList.push_back(it_5l->Get(2));
                                        }
                                        vector<ScAddr> keyElemListSub = IteratorUtilsLocal::getAllByOutRelationWithType(ms_context.get(), elemSubDomain, roleStruct, ScType::NodeConstNoRole);
                                        auto it = find(keyElemListSub.begin(), keyElemListSub.end(), keyElem);
                                        if (it != keyElemListSub.end())
                                            keyElemListSub.erase(it);
                                        shuffle(keyElemListSub.begin(), keyElemListSub.end(), std::mt19937(std::random_device()()));
                                        ScAddr elemSub = keyElemListSub[0];
                                        it_5l = ms_context->Iterator5(elemSub, ScType::EdgeDCommonConst, ScType::Unknown, ScType::EdgeAccessConstPosPerm, GenKeynodes::nrel_main_idtf);
                                        while (it_5l->Next()) {
                                            if (ms_context->HelperCheckEdge(paramLanguage, it_5l->Get(2), ScType::EdgeAccessConstPosPerm)) {
                                                elemOptionCS = it_5l->Get(2);
                                                break;
                                            }
                                        }
                                    }
                                    auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                    if (keyElemList.size() > 2 && itDup == elemDuplicate.end()) {
                                        ScTemplate resultStructTemplate;
                                        ScTemplateParams templateParams;
                                        templateParams.Add("_opkqr", keyElem);
                                        templateParams.Add("_opcsn", elemOptionCS);
                                        string str = "123";
                                        string str1 = "_opn";
                                        shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
                                        ScIterator5Ptr it_5c = ms_context->Iterator5(param, ScType::EdgeAccessConstPosPerm, GenKeynodes::choice_the_correct_option, ScType::EdgeAccessConstPosPerm, GenKeynodes::rrel_key_sc_element);
                                        if (it_5c->Next()) {
                                            for (int j = 0; j < 3; j++) {
                                                ScAddr elem = keyElemList[j];
                                                vector<ScAddr> idtfList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elem, GenKeynodes::nrel_main_idtf);
                                                for (auto elemCp : idtfList) {
                                                    if (ms_context->HelperCheckEdge(paramLanguage, elemCp, ScType::EdgeAccessConstPosPerm)) {
                                                        templateParams.Add(str1 + str[j], elemCp);
                                                        break;
                                                    }
                                                }
                                            }
                                        } else{
                                            for (int j = 0; j < 3; j++)
                                                templateParams.Add(str1 + str[j], keyElemList[j]);
                                        }
                                        ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                                        QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                    }
                                }
                            } else {
                                vector<ScAddr> elemDuplicate;
                                for (int i = 0; i < searchResult.Size(); i++) {
                                    ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                    ScAddr keyElem = searchResultItem["_opkqr"];
                                    vector<ScAddr> keyElemList;
                                    vector<ScAddr> keyElemListCorrect;
                                    vector<ScAddr> keyElemListSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElem, relationStruct);
                                    for (auto currElem : keyElemListSub) {
                                        if (ms_context->HelperCheckEdge(paramLanguage, currElem, ScType::EdgeAccessConstPosPerm))
                                            keyElemList.push_back(currElem);
                                    }
                                    vector<ScAddr> keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelationWithType(ms_context.get(), elemSubDomain, roleStruct, ScType::NodeConstNoRole);
                                    auto it = find(keyElemListCorrectSub.begin(), keyElemListCorrectSub.end(), keyElem);
                                    if (it != keyElemListCorrectSub.end())
                                        keyElemListCorrectSub.erase(it);
                                    shuffle(keyElemListCorrectSub.begin(), keyElemListCorrectSub.end(), std::mt19937(std::random_device()()));
                                    for (auto j : keyElemListCorrectSub) {
                                        vector<ScAddr> idtfList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), j, relationStruct);
                                        for (auto elemCp : idtfList) {
                                            if (ms_context->HelperCheckEdge(paramLanguage, elemCp, ScType::EdgeAccessConstPosPerm)) {
                                                keyElemListCorrect.push_back(elemCp);
                                            }
                                        }
                                        if (keyElemListCorrect.size() >= 2)
                                            break;
                                    }
                                    auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                    if (itDup == elemDuplicate.end() && keyElemListCorrect.size() >= 2 && keyElemList.size() >= 2) {
                                        ScTemplate resultStructTemplate;
                                        ScTemplateParams templateParams;
                                        templateParams.Add("_opkqr", keyElem);
                                        string str = "123";
                                        string str1 = "_opn";
                                        string str2 = "_opcsn";
                                        shuffle(keyElemListCorrect.begin(), keyElemListCorrect.end(),std::mt19937(std::random_device()()));
                                        shuffle(keyElemList.begin(), keyElemList.end(),std::mt19937(std::random_device()()));
                                        ScIterator5Ptr it_5c = ms_context->Iterator5(param, ScType::EdgeAccessConstPosPerm, GenKeynodes::choice_the_correct_option, ScType::EdgeAccessConstPosPerm, GenKeynodes::rrel_key_sc_element);
                                        if (it_5c->Next()) {
                                            for (int j = 0; j < 2; j++) {
                                                templateParams.Add(str2 + str[j], keyElemList[j]);
                                                templateParams.Add(str1 + str[j], keyElemListCorrect[j]);
                                            }
                                        } else {
                                            for (int j = 0; j < 2; j++) {
                                                templateParams.Add(str2 + str[j], keyElemListCorrect[j]);
                                                templateParams.Add(str1 + str[j], keyElemList[j]);
                                            }
                                        }
                                        ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                                        QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                    }
                                }
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::multiple_choice_questions_based_on_axiomatics, ScType::EdgeAccessConstPosPerm)) {
                        elemSubDomain = IteratorUtilsLocal::getFirstWithType(ms_context.get(), initStruct, ScType::NodeConstStruct);
                        roleStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), initStruct, ScType::NodeConstRole);
                        ScAddr relationStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstNoRole);
                        vector<ScAddr> elemDuplicate;
                        for (int i = 0; i < searchResult.Size(); i++) {
                            ScTemplateSearchResultItem searchResultItem = searchResult[i];
                            ScAddr keyElem = searchResultItem["_opkqn"];
                            ScAddr elemOptionCS = searchResultItem["_opcsn"];
                            elemOptionCS = IteratorUtilsLocal::getFirstFromSetWithType(ms_context.get(), elemOptionCS, ScType::Link);
                            vector<ScAddr> keyElemList;
                            vector<ScAddr> keyElemListSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemSubDomain, roleStruct);
                            auto it = find(keyElemListSub.begin(), keyElemListSub.end(), keyElem);
                            if (it != keyElemListSub.end())
                                keyElemListSub.erase(it);
                            for (auto currElem : keyElemListSub)
                            {
                                ScAddr currElemCp = IteratorUtils::getFirstByInRelation(ms_context.get(), currElem, relationStruct);
                                ScAddr currElemCp1 = IteratorUtilsLocal::getFirstFromSetWithType(ms_context.get(), currElemCp, ScType::Link);
                                keyElemList.push_back(currElemCp1);
                            }
                            auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                            if (keyElemList.size() > 2 && itDup == elemDuplicate.end() && ms_context->HelperCheckEdge(GenKeynodes::observation, keyElem, ScType::EdgeAccessConstPosPerm)) {
                                ScTemplate resultStructTemplate;
                                ScTemplateParams templateParams;
                                templateParams.Add("_opkqn", keyElem);
                                string str = "123";
                                string str1 = "_opn";
                                string str2 = "_opcsn";
                                shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
                                ScIterator5Ptr it_5 = ms_context->Iterator5(param, ScType::EdgeAccessConstPosPerm, GenKeynodes::multiple_choice_questions_with_single_option, ScType::EdgeAccessConstPosPerm, GenKeynodes::rrel_key_sc_element);
                                if (it_5->Next()) {
                                    templateParams.Add("_opcsn", elemOptionCS);
                                    for (int j = 0; j < 3; j++)
                                        templateParams.Add(str1 + str[j], keyElemList[j]);
                                } else {
                                    templateParams.Add(str1, elemOptionCS);
                                    for (int j = 0; j < 3; j++)
                                        templateParams.Add(str2 + str[j], keyElemList[j]);
                                }
                                ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                                QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::multiple_choice_questions_based_on_image_examples, ScType::EdgeAccessConstPosPerm)) {
                        elemSubDomain = IteratorUtilsLocal::getFirstWithType(ms_context.get(), initStruct, ScType::NodeConstStruct);
                        roleStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstRole);
                        vector<ScAddr> elemDuplicate;
                        for (int i = 0; i < searchResult.Size(); i++) {
                            ScTemplateSearchResultItem searchResultItem = searchResult[i];
                            ScAddr keyElem = searchResultItem["_opkqn"];
                            ScAddr elemOptionCS = searchResultItem["_opcs"];
                            ScAddr roleStructCp;
                            ScIterator3Ptr it_3 = ms_context->Iterator3(initStruct, ScType::EdgeAccessConstPosPerm, ScType::NodeConstRole);
                            while  (it_3->Next())
                            {
                                if(it_3->Get(2) != roleStruct)
                                    roleStructCp = it_3->Get(2);
                            }
                            vector<ScAddr> keyElemList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemSubDomain, roleStructCp);
                            auto it = find(keyElemList.begin(), keyElemList.end(), elemOptionCS);
                            if (it != keyElemList.end())
                                keyElemList.erase(it);
                            auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                            if (keyElemList.size() > 2 && itDup == elemDuplicate.end()) {
                                ScTemplate resultStructTemplate;
                                ScTemplateParams templateParams;
                                templateParams.Add("_opkqn", keyElem);
                                string str = "123";
                                string str1 = "_op";
                                string str2 = "_opcs";
                                shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
                                ScIterator5Ptr it_5 = ms_context->Iterator5(param, ScType::EdgeAccessConstPosPerm, GenKeynodes::multiple_choice_questions_with_single_option, ScType::EdgeAccessConstPosPerm, GenKeynodes::rrel_key_sc_element);
                                if (it_5->Next()) {
                                    templateParams.Add(str2, elemOptionCS);
                                    for (int j=0; j<3; j++)
                                        templateParams.Add(str1 + str[j], keyElemList[j]);
                                } else {
                                    templateParams.Add(str1, elemOptionCS);
                                    for (int j = 0; j < 3; j++)
                                        templateParams.Add(str2 + str[j], keyElemList[j]);
                                }
                                ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                                QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::multiple_choice_questions_based_on_relation_attributes, ScType::EdgeAccessConstPosPerm)) {
                        if (ms_context->GetElementType(resultStruct) == ScType::NodeConstTuple) {
                            if (ms_context->HelperCheckEdge(param, GenKeynodes::multiple_choice_questions_with_single_option, ScType::EdgeAccessConstPosPerm)) {
                                vector<ScAddr> elemDuplicate;
                                for (int i = 0; i < searchResult.Size(); i++) {
                                    ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                    ScAddr keyElem = searchResultItem["_opkqr"];
                                    vector<ScAddr> keyElemList = IteratorUtilsLocal::getAllWithTypeIn(ms_context.get(), keyElem, ScType::NodeConstClass);
                                    sort(keyElemList.begin(), keyElemList.end(), cmp);
                                    keyElemList.erase(unique(keyElemList.begin(), keyElemList.end(), equalScAddr), keyElemList.end());
                                    auto itr = find(keyElemList.begin(), keyElemList.end(), GenKeynodes::relation);
                                    if (itr != keyElemList.end())
                                        keyElemList.erase(itr);
                                    itr = find(keyElemList.begin(), keyElemList.end(), GenKeynodes::binary_relation);
                                    if (itr != keyElemList.end())
                                        keyElemList.erase(itr);
                                    for (itr = keyElemList.begin(); itr !=keyElemList.end();) {
                                        ScIterator3Ptr it_3 = ms_context->Iterator3(ScType::NodeConstTuple, ScType::EdgeAccessConstPosPerm, *itr);
                                        if (!it_3->Next())
                                            itr = keyElemList.erase(itr);
                                        else
                                            itr++;
                                    }
                                    auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                    if (keyElemList.size() > 2 && itDup == elemDuplicate.end()) {
                                        ScTemplate resultStructTemplate;
                                        ScTemplateParams templateParams;
                                        ScAddr resultStructCp;
                                        templateParams.Add("_opkqr", keyElem);
                                        shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
                                        ScIterator5Ptr it_5 = ms_context->Iterator5(param, ScType::EdgeAccessConstPosPerm, GenKeynodes::choice_the_correct_option, ScType::EdgeAccessConstPosPerm, GenKeynodes::rrel_key_sc_element);
                                        if (it_5->Next()) {
                                            templateParams.Add("_opcs", keyElemList.back());
                                            keyElemList.pop_back();
                                            vector<ScAddr> keyElemListCorrect;
                                            for (auto currElem : keyElemList) {
                                                ScAddr elemTup;
                                                vector<ScAddr> elemTupList = IteratorUtilsLocal::getAllWithTypeIn(ms_context.get(), currElem, ScType::NodeConstTuple);
                                                for (auto tup : elemTupList) {
                                                    if (ms_context->HelperCheckEdge(tup, GenKeynodes::binary_relation, ScType::EdgeDCommonConst) ||
                                                        ms_context->HelperCheckEdge(tup, GenKeynodes::relation, ScType::EdgeDCommonConst)) {
                                                        elemTup = tup;
                                                        break;
                                                    }
                                                }
                                                vector<ScAddr> keyElemListCorrectSub = IteratorUtils::getAllWithType(ms_context.get(), elemTup, ScType::NodeConstClass);
                                                for (auto currElemCp : keyElemListCorrectSub) {
                                                    string strElem = ms_context->HelperGetSystemIdtf(currElemCp);
                                                    if (currElem != currElemCp && strElem[0] != 'p') {
                                                        keyElemListCorrect.push_back(currElemCp);
                                                        break;
                                                    }
                                                }
                                            }
                                            string str = "123";
                                            string str1 = "_op";
                                            shuffle(keyElemListCorrect.begin(), keyElemListCorrect.end(), std::mt19937(std::random_device()()));
                                            if (keyElemListCorrect.size() == 2)
                                            {
                                                resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, Keynodes::rrel_2);
                                                for (int j = 0; j < 2; j++)
                                                    templateParams.Add(str1 + str[j], keyElemListCorrect[j]);
                                            } else{
                                                resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, Keynodes::rrel_3);
                                                for (int j = 0; j < 3; j++)
                                                    templateParams.Add(str1 + str[j], keyElemListCorrect[j]);
                                            }
                                        } else {
                                            ScAddr elemOptionCS;
                                            ScAddr elemBack = keyElemList.back();
                                            keyElemList.pop_back();
                                            ScAddr elemTup;
                                            vector<ScAddr> elemTupList = IteratorUtilsLocal::getAllWithTypeIn(ms_context.get(), elemBack, ScType::NodeConstTuple);
                                            for (auto tup : elemTupList) {
                                                if (ms_context->HelperCheckEdge(tup, GenKeynodes::binary_relation, ScType::EdgeDCommonConst) ||
                                                    ms_context->HelperCheckEdge(tup, GenKeynodes::relation, ScType::EdgeDCommonConst)) {
                                                    elemTup = tup;
                                                    break;
                                                }
                                            }
                                            vector<ScAddr> keyElemListCorrectSub = IteratorUtils::getAllWithType(ms_context.get(), elemTup, ScType::NodeConstClass);
                                            shuffle(keyElemListCorrectSub.begin(), keyElemListCorrectSub.end(), std::mt19937(std::random_device()()));
                                            for (auto currElemCp : keyElemListCorrectSub) {
                                                string strElem = ms_context->HelperGetSystemIdtf(currElemCp);
                                                if (elemBack != currElemCp && strElem[0] != 'p') {
                                                    elemOptionCS = currElemCp;
                                                    break;
                                                }
                                            }
                                            templateParams.Add("_opcs", elemOptionCS);
                                            string str = "123";
                                            string str1 = "_op";
                                            if (keyElemList.size() == 2)
                                            {
                                                resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, Keynodes::rrel_2);
                                                for (int j = 0; j < 2; j++)
                                                    templateParams.Add(str1 + str[j], keyElemList[j]);
                                            } else{
                                                resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, Keynodes::rrel_3);
                                                for (int j = 0; j < 3; j++)
                                                    templateParams.Add(str1 + str[j], keyElemList[j]);
                                            }
                                        }
                                        ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                        QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                    }
                                }
                            } else {
                                vector<ScAddr> elemDuplicate;
                                for (int i = 0; i < searchResult.Size(); i++) {
                                    ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                    ScAddr keyElem = searchResultItem["_opkqr"];
                                    vector <ScAddr> keyElemList = IteratorUtilsLocal::getAllWithTypeIn(ms_context.get(), keyElem, ScType::NodeConstClass);
                                    sort(keyElemList.begin(), keyElemList.end(), cmp);
                                    keyElemList.erase(unique(keyElemList.begin(), keyElemList.end(), equalScAddr), keyElemList.end());
                                    auto itr = find(keyElemList.begin(), keyElemList.end(), GenKeynodes::relation);
                                    if (itr != keyElemList.end())
                                        keyElemList.erase(itr);
                                    itr = find(keyElemList.begin(), keyElemList.end(), GenKeynodes::binary_relation);
                                    if (itr != keyElemList.end())
                                        keyElemList.erase(itr);
                                    for (itr = keyElemList.begin(); itr !=keyElemList.end();) {
                                        ScIterator3Ptr it_3 = ms_context->Iterator3(ScType::NodeConstTuple, ScType::EdgeAccessConstPosPerm, *itr);
                                        if (!it_3->Next())
                                            itr = keyElemList.erase(itr);
                                        else
                                            itr++;
                                    }
                                    auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                    if (keyElemList.size() > 2 && itDup == elemDuplicate.end()) {
                                        ScTemplate resultStructTemplate;
                                        ScTemplateParams templateParams;
                                        ScAddr resultStructCp;
                                        templateParams.Add("_opkqr", keyElem);
                                        vector <ScAddr> keyElemListCorrect;
                                        vector <ScAddr> keyElemListIncorrect;
                                        shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
                                        std::random_device rd;
                                        std::mt19937_64 eng(rd());
                                        string str = "1234";
                                        string str1 = "_opcs";
                                        string str2 = "_op";
                                        if (keyElemList.size() == 3) {
                                            ScAddr resultStructCpSub = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, Keynodes::rrel_3);
                                            std::uniform_int_distribution<unsigned long long> distr(2, 3);
                                            int randNumber = distr(eng);
                                            if (ms_context->HelperCheckEdge(param, GenKeynodes::choice_the_correct_option, ScType::EdgeAccessConstPosPerm)) {
                                                for (int r = randNumber; r > 0; r--) {
                                                    keyElemListCorrect.push_back(keyElemList.back());
                                                    keyElemList.pop_back();
                                                }
                                                for (auto currElem : keyElemList) {
                                                    ScAddr elemTup;
                                                    vector<ScAddr> elemTupList = IteratorUtilsLocal::getAllWithTypeIn(ms_context.get(), currElem, ScType::NodeConstTuple);
                                                    for (auto tup : elemTupList) {
                                                        if (ms_context->HelperCheckEdge(tup, GenKeynodes::binary_relation, ScType::EdgeDCommonConst) ||
                                                            ms_context->HelperCheckEdge(tup, GenKeynodes::relation, ScType::EdgeDCommonConst)) {
                                                            elemTup = tup;
                                                            break;
                                                        }
                                                    }
                                                    vector<ScAddr> keyElemListCorrectSub = IteratorUtils::getAllWithType(ms_context.get(), elemTup, ScType::NodeConstClass);
                                                    for (auto currElemCp : keyElemListCorrectSub) {
                                                        string strElem = ms_context->HelperGetSystemIdtf(currElemCp);
                                                        if (currElem != currElemCp && strElem[0] != 'p') {
                                                            keyElemListIncorrect.push_back(currElemCp);
                                                            break;
                                                        }
                                                    }
                                                }
                                                if (randNumber == 2) {
                                                    resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructCpSub, Keynodes::rrel_2);
                                                    for (int j = 0; j < randNumber; j++)
                                                        templateParams.Add(str1 + str[j], keyElemListCorrect[j]);
                                                    for (int j = 0; j < 3-randNumber; j++)
                                                        templateParams.Add(str2 + str[j], keyElemListIncorrect[j]);
                                                } else {
                                                    resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructCpSub, Keynodes::rrel_3);
                                                    for (int j = 0; j < randNumber; j++)
                                                        templateParams.Add(str1 + str[j], keyElemListCorrect[j]);
                                                }
                                            } else {
                                                for (int r = 3-randNumber; r > 0; r--) {
                                                    keyElemListCorrect.push_back(keyElemList.back());
                                                    keyElemList.pop_back();
                                                }
                                                for (auto currElem : keyElemList) {
                                                    ScAddr elemTup;
                                                    vector<ScAddr> elemTupList = IteratorUtilsLocal::getAllWithTypeIn(ms_context.get(), currElem, ScType::NodeConstTuple);
                                                    for (auto tup : elemTupList) {
                                                        if (ms_context->HelperCheckEdge(tup, GenKeynodes::binary_relation, ScType::EdgeDCommonConst) ||
                                                            ms_context->HelperCheckEdge(tup, GenKeynodes::relation, ScType::EdgeDCommonConst)) {
                                                            elemTup = tup;
                                                            break;
                                                        }
                                                    }
                                                    vector<ScAddr> keyElemListCorrectSub = IteratorUtils::getAllWithType(ms_context.get(), elemTup, ScType::NodeConstClass);
                                                    for (auto currElemCp : keyElemListCorrectSub) {
                                                        string strElem = ms_context->HelperGetSystemIdtf(currElemCp);
                                                        if (currElem != currElemCp && strElem[0] != 'p') {
                                                            keyElemListIncorrect.push_back(currElemCp);
                                                            break;
                                                        }
                                                    }
                                                }
                                                if (randNumber == 2) {
                                                    resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructCpSub, Keynodes::rrel_2);
                                                    for (int j = 0; j < randNumber; j++)
                                                        templateParams.Add(str1 + str[j], keyElemListIncorrect[j]);
                                                    for (int j = 0; j < 3-randNumber; j++)
                                                        templateParams.Add(str2 + str[j], keyElemListCorrect[j]);
                                                } else {
                                                    resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructCpSub, Keynodes::rrel_3);
                                                    for (int j = 0; j < randNumber; j++)
                                                        templateParams.Add(str1 + str[j], keyElemListIncorrect[j]);
                                                }
                                            }
                                        } else {
                                            ScAddr resultStructCpSub = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, Keynodes::rrel_4);
                                            std::uniform_int_distribution<unsigned long long> distr(2, 4);
                                            int randNumber = distr(eng);
                                            if (ms_context->HelperCheckEdge(param, GenKeynodes::choice_the_correct_option, ScType::EdgeAccessConstPosPerm)) {
                                                for (int r = randNumber; r > 0; r--) {
                                                    keyElemListCorrect.push_back(keyElemList.back());
                                                    keyElemList.pop_back();
                                                }
                                                for (auto currElem : keyElemList) {
                                                    ScAddr elemTup;
                                                    vector<ScAddr> elemTupList = IteratorUtilsLocal::getAllWithTypeIn(ms_context.get(), currElem, ScType::NodeConstTuple);
                                                    for (auto tup : elemTupList) {
                                                        if (ms_context->HelperCheckEdge(tup, GenKeynodes::binary_relation, ScType::EdgeDCommonConst) ||
                                                            ms_context->HelperCheckEdge(tup, GenKeynodes::relation, ScType::EdgeDCommonConst)) {
                                                            elemTup = tup;
                                                            break;
                                                        }
                                                    }
                                                    vector<ScAddr> keyElemListCorrectSub = IteratorUtils::getAllWithType(ms_context.get(), elemTup, ScType::NodeConstClass);
                                                    for (auto currElemCp : keyElemListCorrectSub) {
                                                        string strElem = ms_context->HelperGetSystemIdtf(currElemCp);
                                                        if (currElem != currElemCp && strElem[0] != 'p') {
                                                            keyElemListIncorrect.push_back(currElemCp);
                                                            break;
                                                        }
                                                    }
                                                }
                                                if (randNumber == 2) {
                                                    resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructCpSub, Keynodes::rrel_2);
                                                    for (int j = 0; j < randNumber; j++)
                                                        templateParams.Add(str1 + str[j], keyElemListCorrect[j]);
                                                    for (int j = 0; j < 4-randNumber; j++)
                                                        templateParams.Add(str2 + str[j], keyElemListIncorrect[j]);
                                                }
                                                else if (randNumber == 3) {
                                                    resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructCpSub, Keynodes::rrel_3);
                                                    for (int j = 0; j < randNumber; j++)
                                                        templateParams.Add(str1 + str[j], keyElemListCorrect[j]);
                                                    for (int j = 0; j < 4-randNumber; j++)
                                                        templateParams.Add(str2 + str[j], keyElemListIncorrect[j]);
                                                }
                                                else {
                                                    resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructCpSub, Keynodes::rrel_4);
                                                    for (int j = 0; j < randNumber; j++)
                                                        templateParams.Add(str1 + str[j], keyElemListCorrect[j]);
                                                }
                                            } else {
                                                for (int r = 4-randNumber; r > 0; r--) {
                                                    keyElemListCorrect.push_back(keyElemList.back());
                                                    keyElemList.pop_back();
                                                }
                                                for (auto currElem : keyElemList) {
                                                    ScAddr elemTup;
                                                    vector<ScAddr> elemTupList = IteratorUtilsLocal::getAllWithTypeIn(ms_context.get(), currElem, ScType::NodeConstTuple);
                                                    for (auto tup : elemTupList) {
                                                        if (ms_context->HelperCheckEdge(tup, GenKeynodes::binary_relation, ScType::EdgeDCommonConst) ||
                                                            ms_context->HelperCheckEdge(tup, GenKeynodes::relation, ScType::EdgeDCommonConst)) {
                                                            elemTup = tup;
                                                            break;
                                                        }
                                                    }
                                                    vector<ScAddr> keyElemListCorrectSub = IteratorUtils::getAllWithType(ms_context.get(), elemTup, ScType::NodeConstClass);
                                                    for (auto currElemCp : keyElemListCorrectSub) {
                                                        string strElem = ms_context->HelperGetSystemIdtf(currElemCp);
                                                        if (currElem != currElemCp && strElem[0] != 'p') {
                                                            keyElemListIncorrect.push_back(currElemCp);
                                                            break;
                                                        }
                                                    }
                                                }
                                                if (randNumber == 2) {
                                                    resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructCpSub, Keynodes::rrel_2);
                                                    for (int j = 0; j < randNumber; j++)
                                                        templateParams.Add(str1 + str[j], keyElemListIncorrect[j]);
                                                    for (int j = 0; j < 4-randNumber; j++)
                                                        templateParams.Add(str2 + str[j], keyElemListCorrect[j]);
                                                }
                                                else if (randNumber == 3) {
                                                    resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructCpSub, Keynodes::rrel_3);
                                                    for (int j = 0; j < randNumber; j++)
                                                        templateParams.Add(str1 + str[j], keyElemListIncorrect[j]);
                                                    for (int j = 0; j < 4-randNumber; j++)
                                                        templateParams.Add(str2 + str[j], keyElemListCorrect[j]);
                                                }
                                                else {
                                                    resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructCpSub, Keynodes::rrel_4);
                                                    for (int j = 0; j < randNumber; j++)
                                                        templateParams.Add(str1 + str[j], keyElemListIncorrect[j]);
                                                }
                                            }
                                        }
                                        ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                        QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                    }
                                }
                            }
                        }
                    }
                } else if (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions, ScType::EdgeAccessConstPosPerm)) {
                    if (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_based_on_inclusion_relation, ScType::EdgeAccessConstPosPerm) ||
                        ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_based_on_strict_inclusion_relation, ScType::EdgeAccessConstPosPerm)) {
                        ScAddr relationStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstNoRole);
                        if (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_a_blank, ScType::EdgeAccessConstPosPerm) &&
                            ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_unique_answers, ScType::EdgeAccessConstPosPerm)) {
                            vector<ScAddr> elemDuplicate;
                            for (int i = 0; i < searchResult.Size(); i++) {
                                ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                ScAddr keyElem = searchResultItem["_opkq"];
                                ScAddr elemOptionCS = searchResultItem["_opcs"];
                                ScAddr elemRelation = searchResultItem["_nrel_inclusion"];
                                vector<ScAddr> keyElemList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElem, relationStruct);
                                vector<ScAddr> keyElemListCorrect = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemOptionCS, relationStruct);
                                auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                if (keyElemList.size() == 1 && keyElemListCorrect.empty() && elemRelation == relationStruct && itDup == elemDuplicate.end()) {
                                    vector<ScAddr> keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElemList[0], GenKeynodes::nrel_main_idtf);
                                    for (auto currElem : keyElemListCorrectSub) {
                                        if (ms_context->HelperCheckEdge(paramLanguage, currElem, ScType::EdgeAccessConstPosPerm)) {
                                            elemOptionCS = currElem;
                                            break;
                                        }
                                    }
                                    ScTemplate resultStructTemplate;
                                    ScTemplateParams templateParams;
                                    templateParams.Add("_opkq", keyElem);
                                    templateParams.Add("_opcsn", elemOptionCS);
                                    ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                                    QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                }
                            }
                        } else if (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_multiple_blanks, ScType::EdgeAccessConstPosPerm)) {
                            vector<ScAddr> elemDuplicate;
                            for (int i = 0; i < searchResult.Size(); i++) {
                                ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                ScAddr keyElem = searchResultItem["_opkq"];
                                ScAddr elemRelation = searchResultItem["_nrel_inclusion"];
                                vector<ScAddr> keyElemList;
                                vector<ScAddr> keyElemListCorrect;
                                QuestionGenerationProcess::findAllElement(ms_context.get(), keyElem, keyElemList, relationStruct);
                                auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                if (((keyElemList.size() > 1 && ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_unique_answers, ScType::EdgeAccessConstPosPerm)) ||
                                     (keyElemList.size() > 2 && ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_several_possible_answers, ScType::EdgeAccessConstPosPerm)))
                                    && keyElemList.size() < 6 && elemRelation == relationStruct && itDup == elemDuplicate.end()) {
                                    for (auto currElem : keyElemList) {
                                        vector<ScAddr> keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), currElem, GenKeynodes::nrel_main_idtf);
                                        for (auto currElemCp : keyElemListCorrectSub) {
                                            if (ms_context->HelperCheckEdge(paramLanguage, currElemCp, ScType::EdgeAccessConstPosPerm)) {
                                                keyElemListCorrect.push_back(currElemCp);
                                                break;
                                            }
                                        }
                                    }
                                    ScTemplate resultStructTemplate;
                                    ScTemplateParams templateParams;
                                    templateParams.Add("_opkq", keyElem);
                                    string strRole = "rrel_";
                                    string strArrySize = to_string(keyElemListCorrect.size());
                                    ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                    ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                    string str = "12345";
                                    string str1 = "_opcsn";
                                    for (int j=0; j<keyElemListCorrect.size(); j++)
                                        templateParams.Add(str1 + str[j], keyElemListCorrect[j]);
                                    ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                    QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                }
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_based_on_subdividing_relation, ScType::EdgeAccessConstPosPerm)) {
                        ScAddr relationStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstNoRole);
                        if (ms_context->HelperCheckEdge(param, GenKeynodes::rrel_known_answer, ScType::EdgeAccessConstPosPerm)) {
                            std::random_device rd;
                            std::mt19937_64 eng(rd());
                            vector<ScAddr> elemDuplicate;
                            for (int i = 0; i < searchResult.Size(); i++) {
                                ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                ScAddr keyElem = searchResultItem["_opkq"];
                                ScAddr elemRelation = searchResultItem["_nrel_inclusion"];
                                vector<ScAddr> tupleList = IteratorUtils::getAllByInRelation(ms_context.get(), keyElem, relationStruct);
                                auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                if (!tupleList.empty() && elemRelation == relationStruct && itDup == elemDuplicate.end()) {
                                    ScTemplate resultStructTemplate;
                                    ScTemplateParams templateParams;
                                    templateParams.Add("_opkq", keyElem);
                                    int randNumber;
                                    vector<ScAddr> tupleListSub;
                                    if (tupleList.size() == 1) {
                                        randNumber = 1;
                                        tupleListSub = tupleList;
                                    }
                                    else {
                                        std::uniform_int_distribution<unsigned long long> distr(1, 2);
                                        randNumber = distr(eng);
                                        shuffle(tupleList.begin(), tupleList.end(), std::mt19937(std::random_device()()));
                                        for (int j = 0; j < randNumber; j++) {
                                            tupleListSub.push_back(tupleList.back());
                                            tupleList.pop_back();
                                        }
                                    }
                                    string str = "12345678";
                                    string str1 = "_opcsn";
                                    string strRole = "rrel_";
                                    int allNumberOption;
                                    if (tupleListSub.size() == 1 ) {
                                        vector<ScAddr> keyElemListSub = IteratorUtils::getAllWithType(ms_context.get(), tupleListSub[0], ScType::Unknown);
                                        allNumberOption = keyElemListSub.size();
                                        shuffle(keyElemListSub.begin(), keyElemListSub.end(), std::mt19937(std::random_device()()));
                                        vector<ScAddr> keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElemListSub.back(), GenKeynodes::nrel_main_idtf);
                                        for (auto currElemCp : keyElemListCorrectSub) {
                                            if (ms_context->HelperCheckEdge(paramLanguage, currElemCp, ScType::EdgeAccessConstPosPerm)) {
                                                templateParams.Add(str1+str[0], currElemCp);
                                                break;
                                            }
                                        }
                                        keyElemListSub.pop_back();
                                        for (int j = 0; j < keyElemListSub.size(); j++) {
                                            keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElemListSub[j], GenKeynodes::nrel_main_idtf);
                                            for (auto currElemCp : keyElemListCorrectSub) {
                                                if (ms_context->HelperCheckEdge(paramLanguage, currElemCp, ScType::EdgeAccessConstPosPerm)) {
                                                    templateParams.Add(str1+str[j+1], currElemCp);
                                                    break;
                                                }
                                            }
                                        }
                                    } else {
                                        vector<ScAddr> keyElemListSub1 = IteratorUtils::getAllWithType(ms_context.get(), tupleListSub[0], ScType::Unknown);
                                        vector<ScAddr> keyElemListSub2 = IteratorUtils::getAllWithType(ms_context.get(), tupleListSub[1], ScType::Unknown);
                                        allNumberOption = keyElemListSub1.size();
                                        allNumberOption += keyElemListSub2.size();
                                        shuffle(keyElemListSub1.begin(), keyElemListSub1.end(), std::mt19937(std::random_device()()));
                                        shuffle(keyElemListSub2.begin(), keyElemListSub2.end(), std::mt19937(std::random_device()()));
                                        vector<ScAddr> keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElemListSub1.back(), GenKeynodes::nrel_main_idtf);
                                        for (auto currElemCp : keyElemListCorrectSub) {
                                            if (ms_context->HelperCheckEdge(paramLanguage, currElemCp, ScType::EdgeAccessConstPosPerm)) {
                                                templateParams.Add(str1+str[0], currElemCp);
                                                break;
                                            }
                                        }
                                        keyElemListSub1.pop_back();
                                        keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElemListSub2.back(), GenKeynodes::nrel_main_idtf);
                                        for (auto currElemCp : keyElemListCorrectSub) {
                                            if (ms_context->HelperCheckEdge(paramLanguage, currElemCp, ScType::EdgeAccessConstPosPerm)) {
                                                templateParams.Add(str1+str[1], currElemCp);
                                                break;
                                            }
                                        }
                                        keyElemListSub2.pop_back();
                                        keyElemListSub1.insert(keyElemListSub1.end(), keyElemListSub2.begin(), keyElemListSub2.end());
                                        for (int j = 0; j < keyElemListSub1.size(); j++) {
                                            keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElemListSub1[j], GenKeynodes::nrel_main_idtf);
                                            for (auto currElemCp : keyElemListCorrectSub) {
                                                if (ms_context->HelperCheckEdge(paramLanguage, currElemCp, ScType::EdgeAccessConstPosPerm)) {
                                                    templateParams.Add(str1+str[j+2], currElemCp);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    string strArrySize = to_string(randNumber);
                                    ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                    ScAddr resultStructSub = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                    strArrySize = to_string(allNumberOption);
                                    elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                    ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructSub, elementRole);
                                    ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                    QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                }
                            }
                        } else {
                            vector<ScAddr> elemDuplicate;
                            for (int i = 0; i < searchResult.Size(); i++) {
                                ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                ScAddr keyElem = searchResultItem["_opkq"];
                                ScAddr elemRelation = searchResultItem["_nrel_inclusion"];
                                vector<ScAddr> tupleList = IteratorUtils::getAllByInRelation(ms_context.get(), keyElem, relationStruct);
                                auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                if (tupleList.size() == 1 && elemRelation == relationStruct && itDup == elemDuplicate.end()) {
                                    ScTemplate resultStructTemplate;
                                    ScTemplateParams templateParams;
                                    templateParams.Add("_opkq", keyElem);
                                    string str = "1234";
                                    string str1 = "_opcsn";
                                    string strRole = "rrel_";
                                    vector<ScAddr> keyElemListCorrect;
                                    vector<ScAddr> keyElemList = IteratorUtils::getAllWithType(ms_context.get(), tupleList[0], ScType::Unknown);
                                    if (keyElemList.size() > 4 || keyElemList.empty() )
                                        continue;
                                    for (auto currElem : keyElemList) {
                                        vector<ScAddr> keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), currElem, GenKeynodes::nrel_main_idtf);
                                        for (auto currElemCp : keyElemListCorrectSub) {
                                            if (ms_context->HelperCheckEdge(paramLanguage, currElemCp, ScType::EdgeAccessConstPosPerm)) {
                                                keyElemListCorrect.push_back(currElemCp);
                                                break;
                                            }
                                        }
                                    }
                                    for (int j = 0; j < keyElemListCorrect.size(); j++)
                                        templateParams.Add(str1 + str[j], keyElemListCorrect[j]);
                                    string strArrySize = to_string(keyElemList.size());
                                    ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                    ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                    ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                    QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                }
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_based_on_binary_relation, ScType::EdgeAccessConstPosPerm)) {
                        ScAddr relationStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstNoRole);
                        if (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_a_blank, ScType::EdgeAccessConstPosPerm)) {
                            vector<ScAddr> elemDuplicate;
                            for (int i = 0; i < searchResult.Size(); i++) {
                                ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                ScAddr keyElem = searchResultItem["_opkqn"];
                                ScAddr elemRelation = searchResultItem["_nrel_inclusion"];
                                ScAddr elemOptionCS = searchResultItem["_opcsn"];
                                auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                if (elemRelation == relationStruct && itDup == elemDuplicate.end()) {
                                    ScTemplate resultStructTemplate;
                                    ScTemplateParams templateParams;
                                    templateParams.Add("_opkqn", keyElem);
                                    vector<ScAddr> keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemOptionCS, GenKeynodes::nrel_main_idtf);
                                    for (auto currElemCp : keyElemListCorrectSub) {
                                        if (ms_context->HelperCheckEdge(paramLanguage, currElemCp, ScType::EdgeAccessConstPosPerm)) {
                                            elemOptionCS =  currElemCp;
                                            break;
                                        }
                                    }
                                    templateParams.Add("_opcsn", elemOptionCS);
                                    ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                                    QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                }
                            }
                        } else {
                            vector<ScAddr> elemDuplicate;
                            for (int i = 0; i < searchResult.Size(); i++) {
                                ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                ScAddr keyElem = searchResultItem["_opkqn"];
                                ScAddr elemRelation = searchResultItem["_nrel_inclusion"];
                                vector<ScAddr> keyElementCorrectList = IteratorUtils::getAllByInRelation(ms_context.get(), keyElem, relationStruct);
                                auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                if (elemRelation == relationStruct && itDup == elemDuplicate.end() && keyElementCorrectList.size() < 6 &&
                                    ((!keyElementCorrectList.empty() && ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_unique_answers, ScType::EdgeAccessConstPosPerm)) ||
                                    (keyElementCorrectList.size() > 2 && ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_several_possible_answers, ScType::EdgeAccessConstPosPerm)))             ) {
                                    ScTemplate resultStructTemplate;
                                    ScTemplateParams templateParams;
                                    templateParams.Add("_opkqn", keyElem);
                                    string strRole = "rrel_";
                                    string str = "12345";
                                    string str1 = "_opcsn";
                                    vector<ScAddr> keyElemList;
                                    for (auto currElem : keyElementCorrectList) {
                                        vector<ScAddr> keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), currElem, GenKeynodes::nrel_main_idtf);
                                        for (auto currElemCp : keyElemListCorrectSub) {
                                            if (ms_context->HelperCheckEdge(paramLanguage, currElemCp, ScType::EdgeAccessConstPosPerm)) {
                                                keyElemList.push_back(currElemCp);
                                                break;
                                            }
                                        }
                                    }
                                    if ((keyElemList.empty() && ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_unique_answers, ScType::EdgeAccessConstPosPerm)) ||
                                        (keyElemList.size() < 3 && ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_several_possible_answers, ScType::EdgeAccessConstPosPerm)))
                                        continue;
                                    for (int j = 0; j < keyElemList.size(); j++)
                                        templateParams.Add(str1 + str[j], keyElemList[j]);
                                    string strArrySize = to_string(keyElemList.size());
                                    ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                    ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                    ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                    QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                }
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_based_on_set_identifier, ScType::EdgeAccessConstPosPerm)) {
                        ScAddr relationStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstNoRole);
                        vector<ScAddr> elemDuplicate;
                        for (int i = 0; i < searchResult.Size(); i++) {
                            ScTemplateSearchResultItem searchResultItem = searchResult[i];
                            ScAddr keyElem = searchResultItem["_opkq"];
                            vector<ScAddr> keyElementCorrectList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElem, relationStruct);
                            auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                            if (itDup == elemDuplicate.end() && keyElementCorrectList.size() < 7 &&
                                ((ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_unique_answers, ScType::EdgeAccessConstPosPerm) && !keyElementCorrectList.empty()) ||
                                 (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_several_possible_answers, ScType::EdgeAccessConstPosPerm) && keyElementCorrectList.size() > 2))) {
                                ScTemplate resultStructTemplate;
                                ScTemplateParams templateParams;
                                templateParams.Add("_opkq", keyElem);
                                string strRole = "rrel_";
                                string str = "123456";
                                string str1 = "_opcsn";
                                for (auto it = keyElementCorrectList.begin(); it != keyElementCorrectList.end();) {
                                    if (!ms_context->HelperCheckEdge(paramLanguage, *it, ScType::EdgeAccessConstPosPerm))
                                        keyElementCorrectList.erase(it);
                                    else
                                        it++;
                                }
                                if ((ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_unique_answers, ScType::EdgeAccessConstPosPerm) && keyElementCorrectList.empty()) ||
                                    (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_several_possible_answers, ScType::EdgeAccessConstPosPerm) && keyElementCorrectList.size() < 3))
                                    continue;
                                for (int j = 0; j < keyElementCorrectList.size(); j++)
                                    templateParams.Add(str1 + str[j], keyElementCorrectList[j]);
                                string strArrySize = to_string(keyElementCorrectList.size());
                                ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole + strArrySize, ScType::NodeConstRole);
                                ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_based_on_relation_identifier, ScType::EdgeAccessConstPosPerm)) {
                        ScAddr relationStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstNoRole);
                        vector<ScAddr> elemDuplicate;
                        for (int i = 0; i < searchResult.Size(); i++) {
                            ScTemplateSearchResultItem searchResultItem = searchResult[i];
                            ScAddr keyElem = searchResultItem["_opkqr"];
                            vector<ScAddr> keyElementCorrectList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElem, relationStruct);
                            auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                            if (itDup == elemDuplicate.end() && keyElementCorrectList.size() < 7 &&
                                ((ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_unique_answers, ScType::EdgeAccessConstPosPerm) && !keyElementCorrectList.empty()) ||
                                (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_several_possible_answers, ScType::EdgeAccessConstPosPerm) && keyElementCorrectList.size() > 2))) {
                                ScTemplate resultStructTemplate;
                                ScTemplateParams templateParams;
                                templateParams.Add("_opkqr", keyElem);
                                string strRole = "rrel_";
                                string str = "123456";
                                string str1 = "_opcsn";
                                for (auto it = keyElementCorrectList.begin(); it != keyElementCorrectList.end();) {
                                    if (!ms_context->HelperCheckEdge(paramLanguage, *it, ScType::EdgeAccessConstPosPerm))
                                        keyElementCorrectList.erase(it);
                                    else
                                        it++;
                                }
                                if ((ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_unique_answers, ScType::EdgeAccessConstPosPerm) && keyElementCorrectList.empty()) ||
                                    (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_several_possible_answers, ScType::EdgeAccessConstPosPerm) && keyElementCorrectList.size() < 3))
                                    continue;
                                for (int j = 0; j < keyElementCorrectList.size(); j++)
                                    templateParams.Add(str1 + str[j], keyElementCorrectList[j]);
                                string strArrySize = to_string(keyElementCorrectList.size());
                                ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_based_on_axiomatics, ScType::EdgeAccessConstPosPerm)) {
                        vector<ScAddr> elemDuplicate;
                        for (int i = 0; i < searchResult.Size(); i++) {
                            ScTemplateSearchResultItem searchResultItem = searchResult[i];
                            ScAddr keyElem = searchResultItem["_opkqn"];
                            ScAddr elemOptionCS = searchResultItem["_opcsn"];
                            keyElem = IteratorUtilsLocal::getFirstFromSetWithType(ms_context.get(), keyElem, ScType::Link);
                            vector<ScAddr> keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemOptionCS, GenKeynodes::nrel_main_idtf);
                            for (auto currElemCp : keyElemListCorrectSub) {
                                if (ms_context->HelperCheckEdge(paramLanguage, currElemCp, ScType::EdgeAccessConstPosPerm)) {
                                    elemOptionCS = currElemCp;
                                    break;
                                }
                            }
                            auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                            if (itDup == elemDuplicate.end() && keyElem.IsValid() && elemOptionCS.IsValid()) {
                                ScTemplate resultStructTemplate;
                                ScTemplateParams templateParams;
                                templateParams.Add("_opkqn", keyElem);
                                templateParams.Add("_opcsn", elemOptionCS);
                                ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                                QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_based_on_image_examples, ScType::EdgeAccessConstPosPerm)) {
                        vector<ScAddr> elemDuplicate;
                        for (int i = 0; i < searchResult.Size(); i++) {
                            ScTemplateSearchResultItem searchResultItem = searchResult[i];
                            ScAddr keyElem = searchResultItem["_opkqn"];
                            ScAddr elemOptionCS = searchResultItem["_opcs"];
                            vector<ScAddr> keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemOptionCS, GenKeynodes::nrel_main_idtf);
                            for (auto currElemCp : keyElemListCorrectSub) {
                                if (ms_context->HelperCheckEdge(paramLanguage, currElemCp, ScType::EdgeAccessConstPosPerm)) {
                                    elemOptionCS = currElemCp;
                                    break;
                                }
                            }
                            auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                            if (itDup == elemDuplicate.end() && keyElem.IsValid() && elemOptionCS.IsValid()) {
                                ScTemplate resultStructTemplate;
                                ScTemplateParams templateParams;
                                templateParams.Add("_opkqn", keyElem);
                                templateParams.Add("_opcsn", elemOptionCS);
                                ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                                QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_based_on_relation_attributes, ScType::EdgeAccessConstPosPerm)) {
                        vector<ScAddr> elemDuplicate;
                        for (int i = 0; i < searchResult.Size(); i++) {
                            ScTemplateSearchResultItem searchResultItem = searchResult[i];
                            ScAddr keyElem = searchResultItem["_opkqr"];
                            vector<ScAddr> keyElemList = IteratorUtilsLocal::getAllWithTypeIn(ms_context.get(), keyElem, ScType::NodeConstClass);
                            sort(keyElemList.begin(), keyElemList.end(), cmp);
                            keyElemList.erase(unique(keyElemList.begin(), keyElemList.end(), equalScAddr), keyElemList.end());
                            auto itr = find(keyElemList.begin(), keyElemList.end(), GenKeynodes::relation);
                            if (itr != keyElemList.end())
                                keyElemList.erase(itr);
                            itr = find(keyElemList.begin(), keyElemList.end(), GenKeynodes::binary_relation);
                            if (itr != keyElemList.end())
                                keyElemList.erase(itr);
                            for (itr = keyElemList.begin(); itr !=keyElemList.end();) {
                                ScIterator3Ptr it_3 = ms_context->Iterator3(ScType::NodeConstTuple, ScType::EdgeAccessConstPosPerm, *itr);
                                if (!it_3->Next())
                                    itr = keyElemList.erase(itr);
                                else {
                                    if (!ms_context->HelperCheckEdge(it_3->Get(0), GenKeynodes::binary_relation, ScType::EdgeDCommonConst))
                                        itr = keyElemList.erase(itr);
                                    else
                                        itr++;
                                }
                            }
                            auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                            if (itDup == elemDuplicate.end() && ((!keyElemList.empty() &&  ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_unique_answers, ScType::EdgeAccessConstPosPerm)) ||
                                (ms_context->HelperCheckEdge(param, GenKeynodes::fill_in_the_blank_questions_with_several_possible_answers, ScType::EdgeAccessConstPosPerm) && keyElemList.size() > 1))) {
                                ScTemplate resultStructTemplate;
                                ScTemplateParams templateParams;
                                templateParams.Add("_opkqr", keyElem);
                                string strRole = "rrel_";
                                string str = "123456";
                                string str1 = "_opcsn";
                                vector<ScAddr> keyElemtCorrectList;
                                for (auto currElem : keyElemList) {
                                    vector<ScAddr> keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), currElem, GenKeynodes::nrel_main_idtf);
                                    for (auto currElemCp : keyElemListCorrectSub) {
                                        if (ms_context->HelperCheckEdge(paramLanguage, currElemCp, ScType::EdgeAccessConstPosPerm)) {
                                            keyElemtCorrectList.push_back(currElemCp);
                                            break;
                                        }
                                    }
                                }
                                for (int j = 0; j < keyElemtCorrectList.size(); j++)
                                    templateParams.Add(str1 + str[j], keyElemtCorrectList[j]);
                                string strArrySize = to_string(keyElemtCorrectList.size());
                                ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                            }
                        }
                    }
                } else if (ms_context->HelperCheckEdge(param, GenKeynodes::judgment_questions, ScType::EdgeAccessConstPosPerm)) {
                    if (ms_context->HelperCheckEdge(param, GenKeynodes::judgment_questions_based_on_inclusion_relation, ScType::EdgeAccessConstPosPerm) ||
                        ms_context->HelperCheckEdge(param, GenKeynodes::judgment_questions_based_on_strict_inclusion_relation, ScType::EdgeAccessConstPosPerm)) {
                        ScAddr relationStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstNoRole);
                        ScAddr elemSubDomain = paramSubDom;
                        ScAddr roleStruct = paramRolRel;
                        std::random_device rd;
                        std::mt19937_64 eng(rd());
                        if (!ms_context->HelperCheckEdge(param, GenKeynodes::judgment_questions_of_choosing_false, ScType::EdgeAccessConstPosPerm) &&
                            !ms_context->HelperCheckEdge(param, GenKeynodes::judgment_questions_of_choosing_true, ScType::EdgeAccessConstPosPerm)) {
                            vector<ScAddr> elemDuplicate;
                            for (int i = 0; i < searchResult.Size(); i++) {
                                ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                ScAddr keyElem = searchResultItem["_opkq"];
                                ScAddr elemRelation = searchResultItem["_nrel_inclusion"];
                                std::uniform_int_distribution<unsigned long long> distr(0, 1);
                                int trueOrFalse = distr(eng);
                                if (trueOrFalse) {
                                    vector<ScAddr> keyElemList;
                                    QuestionGenerationProcess::findAllElement(ms_context.get(), keyElem, keyElemList, relationStruct);
                                    sort(keyElemList.begin(), keyElemList.end(), cmp);
                                    keyElemList.erase(unique(keyElemList.begin(), keyElemList.end(), equalScAddr), keyElemList.end());
                                    auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                    if (!keyElemList.empty() && elemRelation == relationStruct && itDup == elemDuplicate.end()) {
                                        ScTemplate resultStructTemplate;
                                        ScTemplateParams  templateParams;
                                        templateParams.Add("_opkq", keyElem);
                                        string str = "123456";
                                        string str1= "_op";
                                        string strRole = "rrel_";
                                        int randNumber;
                                        if (keyElemList.size() > 5) {
                                            std::uniform_int_distribution<unsigned long long> distrLocal(1, 5);
                                            randNumber = distrLocal(eng);
                                        } else {
                                            std::uniform_int_distribution<unsigned long long> distrLocal(1, keyElemList.size());
                                            randNumber = distrLocal(eng);
                                        }
                                        shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
                                        for (int j = 0; j < randNumber; j++)
                                            templateParams.Add(str1 + str[j], keyElemList[j]);
                                        string strArrySize = to_string(trueOrFalse);
                                        ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                        ScAddr resultStructSub = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                        strArrySize = to_string(randNumber);
                                        elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                        ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructSub, elementRole);
                                        ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                        QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                    }
                                }
                                else {
                                    vector<ScAddr> keyElemList;
                                    QuestionGenerationProcess::findAllElement(ms_context.get(), keyElem, keyElemList, relationStruct);
                                    sort(keyElemList.begin(), keyElemList.end(), cmp);
                                    keyElemList.erase(unique(keyElemList.begin(), keyElemList.end(), equalScAddr), keyElemList.end());
                                    vector<ScAddr> keyElemListCorrectSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemSubDomain, roleStruct);
                                    sort(keyElemListCorrectSub.begin(), keyElemListCorrectSub.end(), cmp);
                                    keyElemListCorrectSub.erase(unique(keyElemListCorrectSub.begin(), keyElemListCorrectSub.end(), equalScAddr), keyElemListCorrectSub.end());
                                    auto it = find(keyElemListCorrectSub.begin(), keyElemListCorrectSub.end(), keyElem);
                                    if (it != keyElemListCorrectSub.end())
                                        keyElemListCorrectSub.erase(it);
                                    for (auto itr : keyElemList) {
                                        auto itr1 = find(keyElemListCorrectSub.begin(), keyElemListCorrectSub.end(), itr);
                                        if (itr1 != keyElemListCorrectSub.end()) {
                                            keyElemListCorrectSub.erase(itr1);
                                        }
                                    }
                                    auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                    if (!keyElemList.empty() && !keyElemListCorrectSub.empty() && elemRelation == relationStruct && itDup == elemDuplicate.end()) {
                                        ScTemplate resultStructTemplate;
                                        ScTemplateParams  templateParams;
                                        templateParams.Add("_opkq", keyElem);
                                        string str = "123456";
                                        string str1= "_op";
                                        string strRole = "rrel_";
                                        int randNumber = 0, arrySize = keyElemList.size(), randInNumber = 0;
                                        if (arrySize > 5)
                                            arrySize = 5;
                                        if (arrySize > 1) {
                                            std::uniform_int_distribution<unsigned long long> distrLocal(1, arrySize - 1);
                                            randNumber = distrLocal(eng);
                                        }
                                        randInNumber = arrySize - randNumber;
                                        if (randInNumber > keyElemListCorrectSub.size())
                                            continue;
                                        shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
                                        shuffle(keyElemListCorrectSub.begin(), keyElemListCorrectSub.end(), std::mt19937(std::random_device()()));
                                        for (int j=0; j < arrySize; j++) {
                                            if (j < randNumber)
                                                templateParams.Add(str1 + str[j], keyElemList[j]);
                                            else
                                                templateParams.Add(str1 + str[j], keyElemListCorrectSub[j]);
                                        }
                                        string strArrySize = to_string(trueOrFalse);
                                        ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                        ScAddr resultStructSub = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                        strArrySize = to_string(arrySize);
                                        elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                        ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructSub, elementRole);
                                        ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                        QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                    }
                                }
                            }
                        } else if (ms_context->HelperCheckEdge(param, GenKeynodes::judgment_questions_of_choosing_false, ScType::EdgeAccessConstPosPerm)) {
                            vector<ScAddr> elemDuplicate;
                            for (int i = 0; i < searchResult.Size(); i++) {
                                ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                ScAddr keyElem = searchResultItem["_opkq"];
                                ScAddr elemRelation = searchResultItem["_nrel_inclusion"];
                                ScAddr elemOptionCS = searchResultItem["_op"];
                                auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                if (keyElem.IsValid() && elemOptionCS.IsValid() && elemRelation == relationStruct && itDup == elemDuplicate.end()) {
                                    ScTemplate resultStructTemplate;
                                    ScTemplateParams templateParams;
                                    templateParams.Add("_opkq", keyElem);
                                    templateParams.Add("_op", elemOptionCS);
                                    ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct);
                                    QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                }
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::judgment_questions_based_on_subdividing_relation, ScType::EdgeAccessConstPosPerm)) {
                        ScAddr elemSubDomain = paramSubDom;
                        ScAddr roleStruct = paramRolRel;
                        ScAddr relationStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstNoRole);
                        std::random_device rd;
                        std::mt19937_64 eng(rd());
                        vector<ScAddr> elemDuplicate;
                        for (int i = 0; i < searchResult.Size(); i++) {
                            ScTemplateSearchResultItem searchResultItem = searchResult[i];
                            ScAddr keyElem = searchResultItem["_opkq"];
                            ScAddr elemRelation = searchResultItem["_nrel_inclusion"];
                            vector<ScAddr> tupleList = IteratorUtils::getAllByInRelation(ms_context.get(), keyElem, relationStruct);
                            if (tupleList.empty())
                                continue;
                            vector<ScAddr> keyElemListCorrect = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemSubDomain, roleStruct);
                            auto it = find(keyElemListCorrect.begin(), keyElemListCorrect.end(), keyElem);
                            if (it != keyElemListCorrect.end())
                                keyElemListCorrect.erase(it);
                            vector<ScAddr> tupleListCorrect;
                            for (auto ele : keyElemListCorrect) {
                                vector<ScAddr> tupleListSub =  IteratorUtils::getAllByInRelation(ms_context.get(), ele, relationStruct);
                                tupleListCorrect.insert(tupleListCorrect.end(), tupleListSub.begin(), tupleListSub.end());
                            }
                            vector<ScAddr> keyElemList;
                            std::uniform_int_distribution<unsigned long long> distr(0, 1);
                            int trueOrFalse = distr(eng);
                            if (tupleListCorrect.empty())
                                trueOrFalse = 1;
                            if (trueOrFalse) {
                                int randNumber = 0;
                                if (tupleList.size() == 1)
                                    randNumber = 1;
                                else if (tupleList.size() > 1) {
                                    std::uniform_int_distribution<unsigned long long> distrLocal(1, 2);
                                    randNumber = distrLocal(eng);
                                }
                                shuffle(tupleList.begin(), tupleList.end(), std::mt19937(std::random_device()()));
                                for (int j = 0; j < randNumber; j++) {
                                    vector<ScAddr> elementList = IteratorUtils::getAllWithType(ms_context.get(), tupleList[j], ScType::Unknown);
                                    keyElemList.insert(keyElemList.end(), elementList.begin(), elementList.end());
                                }
                                auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                if (keyElemList.size() > 1 && keyElemList.size() < 9 && elemRelation == relationStruct && itDup == elemDuplicate.end()) {
                                    ScTemplate resultStructTemplate;
                                    ScTemplateParams  templateParams;
                                    templateParams.Add("_opkq", keyElem);
                                    string str = "12345678";
                                    string str1= "_op";
                                    string strRole = "rrel_";
                                    for (int j = 0; j < keyElemList.size(); j++)
                                        templateParams.Add(str1 + str[j], keyElemList[j]);
                                    string strArrySize = to_string(trueOrFalse);
                                    ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                    ScAddr resultStructSub = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                    strArrySize = to_string(keyElemList.size());
                                    elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                    ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructSub, elementRole);
                                    ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                    QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                }
                            } else {
                                std::uniform_int_distribution<unsigned long long> distrLocal(1, 2);
                                int randNumber = distrLocal(eng);
                                shuffle(tupleList.begin(), tupleList.end(), std::mt19937(std::random_device()()));
                                shuffle(tupleListCorrect.begin(), tupleListCorrect.end(), std::mt19937(std::random_device()()));
                                if (randNumber == 1)
                                    keyElemList = IteratorUtils::getAllWithType(ms_context.get(), tupleListCorrect.front(), ScType::Unknown);
                                else {
                                    keyElemList = IteratorUtils::getAllWithType(ms_context.get(), tupleList.front(), ScType::Unknown);
                                    vector<ScAddr> elementList = IteratorUtils::getAllWithType(ms_context.get(), tupleListCorrect.front(), ScType::Unknown);
                                    keyElemList.insert(keyElemList.end(), elementList.begin(), elementList.end());
                                }
                                auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                auto itDupCp = find(keyElemList.begin(), keyElemList.end(), keyElem);
                                if (keyElemList.size() > 1 && keyElemList.size() < 9 && elemRelation == relationStruct && itDup == elemDuplicate.end() && itDupCp == keyElemList.end()) {
                                    ScTemplate resultStructTemplate;
                                    ScTemplateParams  templateParams;
                                    templateParams.Add("_opkq", keyElem);
                                    string str = "12345678";
                                    string str1= "_op";
                                    string strRole = "rrel_";
                                    for (int j = 0; j < keyElemList.size(); j++)
                                        templateParams.Add(str1 + str[j], keyElemList[j]);
                                    string strArrySize = to_string(trueOrFalse);
                                    ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                    ScAddr resultStructSub = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                    strArrySize = to_string(keyElemList.size());
                                    elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                    ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructSub, elementRole);
                                    ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                    QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                }
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::judgment_questions_based_on_binary_relation, ScType::EdgeAccessConstPosPerm)) {
                        ScAddr elemSubDomain = IteratorUtilsLocal::getFirstWithType(ms_context.get(), initStruct, ScType::NodeConstClass);
                        ScAddr relationStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstNoRole);
                        std::random_device rd;
                        std::mt19937_64 eng(rd());
                        if (ms_context->HelperCheckEdge(param, Keynodes::rrel_1, ScType::EdgeAccessConstPosPerm)) {
                            vector<ScAddr> elemDuplicate;
                            for (int i = 0; i < searchResult.Size(); i++) {
                                ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                ScAddr keyElem = searchResultItem["_opkqn"];
                                ScAddr elemOptionCS = searchResultItem["_opcsn"];
                                ScAddr elemRelation = searchResultItem["_nrel_inclusion"];
                                vector<ScAddr> keyElemCorrectList = IteratorUtils::getAllWithType(ms_context.get(), elemSubDomain, ScType::NodeConst);
                                auto it = find(keyElemCorrectList.begin(), keyElemCorrectList.end(), elemOptionCS);
                                if (it != keyElemCorrectList.end())
                                    keyElemCorrectList.erase(it);
                                std::uniform_int_distribution<unsigned long long> distr(0, 1);
                                int trueOrFalse = distr(eng);
                                auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                if (elemRelation == relationStruct && itDup == elemDuplicate.end() && !keyElemCorrectList.empty()) {
                                    ScTemplate resultStructTemplate;
                                    ScTemplateParams  templateParams;
                                    templateParams.Add("_opkqn", keyElem);
                                    string strRole = "rrel_";
                                    if (!trueOrFalse) {
                                        shuffle(keyElemCorrectList.begin(), keyElemCorrectList.end(), std::mt19937(std::random_device()()));
                                        elemOptionCS = keyElemCorrectList.front();
                                    }
                                    templateParams.Add("_opn", elemOptionCS);
                                    string strArrySize = to_string(trueOrFalse);
                                    ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                    ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                    ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                    QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                }
                            }
                        } else if (ms_context->HelperCheckEdge(param, Keynodes::rrel_2, ScType::EdgeAccessConstPosPerm)) {
                            vector<ScAddr> elemDuplicate;
                            for (int i = 0; i < searchResult.Size(); i++) {
                                ScTemplateSearchResultItem searchResultItem = searchResult[i];
                                ScAddr keyElem = searchResultItem["_opkqn"];
                                ScAddr elemRelation = searchResultItem["_nrel_inclusion"];
                                vector<ScAddr> keyElemList = IteratorUtils::getAllByInRelation(ms_context.get(), keyElem, relationStruct);
                                vector<ScAddr> keyElemCorrectList = IteratorUtils::getAllWithType(ms_context.get(), elemSubDomain, ScType::NodeConst);
                                auto it = find(keyElemCorrectList.begin(), keyElemCorrectList.end(), keyElem);
                                if (it != keyElemCorrectList.end())
                                    keyElemCorrectList.erase(it);
                                std::uniform_int_distribution<unsigned long long> distr(0, 1);
                                int trueOrFalse = distr(eng);
                                auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                                if (elemRelation == relationStruct && itDup == elemDuplicate.end() && ((trueOrFalse && !keyElemList.empty()) || (!trueOrFalse && !keyElemCorrectList.empty()))) {
                                    ScTemplate resultStructTemplate;
                                    ScTemplateParams  templateParams;
                                    templateParams.Add("_opkqn", keyElem);
                                    string str = "12345678";
                                    string str1= "_opn";
                                    string strRole = "rrel_";
                                    int ArrySize = 0;
                                    if (trueOrFalse) {
                                        ArrySize = keyElemList.size();
                                        if (ArrySize > 5)
                                            continue;
                                        for (int j = 0; j < ArrySize; j++)
                                            templateParams.Add(str1 + str[j], keyElemList[j]);
                                    } else {
                                        shuffle(keyElemCorrectList.begin(), keyElemCorrectList.end(), std::mt19937(std::random_device()()));
                                        vector<ScAddr> keyElemListSub = IteratorUtils::getAllByInRelation(ms_context.get(), keyElemCorrectList.front(), relationStruct);
                                        ArrySize = keyElemListSub.size();
                                        if (ArrySize > 5 || ArrySize == 0)
                                            continue;
                                        for (int j = 0; j < ArrySize; j++)
                                            templateParams.Add(str1 + str[j], keyElemListSub[j]);
                                    }
                                    string strArrySize = to_string(trueOrFalse);
                                    ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                    ScAddr resultStructSub = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                    strArrySize = to_string(ArrySize);
                                    elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                    ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructSub, elementRole);
                                    ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                    QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                                }
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::judgment_questions_based_on_set_identifiers, ScType::EdgeAccessConstPosPerm)) {
                        ScAddr elemSubDomain = paramSubDom;
                        ScAddr roleStruct = paramRolRel;
                        ScAddr relationStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstNoRole);
                        std::random_device rd;
                        std::mt19937_64 eng(rd());
                        vector<ScAddr> elemDuplicate;
                        for (int i = 0; i < searchResult.Size(); i++) {
                            ScTemplateSearchResultItem searchResultItem = searchResult[i];
                            ScAddr keyElem = searchResultItem["_opkq"];
                            vector<ScAddr> keyElemList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElem, relationStruct);
                            for (auto it = keyElemList.begin(); it != keyElemList.end();) {
                                if (!ms_context->HelperCheckEdge(paramLanguage, *it, ScType::EdgeAccessConstPosPerm))
                                    keyElemList.erase(it);
                                else
                                    it++;
                            }
                            vector<ScAddr> keyElemListCorrect = IteratorUtilsLocal::getAllByOutRelationWithType(ms_context.get(), elemSubDomain, roleStruct, ScType::NodeConstClass);
                            auto it = find(keyElemListCorrect.begin(), keyElemListCorrect.end(), keyElem);
                            if (it != keyElemListCorrect.end())
                                keyElemListCorrect.erase(it);
                            std::uniform_int_distribution<unsigned long long> distr(0, 1);
                            int trueOrFalse = distr(eng);
                            auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                            if (itDup == elemDuplicate.end() && ((trueOrFalse && !keyElemList.empty()) || (!trueOrFalse && !keyElemListCorrect.empty()))) {
                                ScTemplate resultStructTemplate;
                                ScTemplateParams  templateParams;
                                templateParams.Add("_opkq", keyElem);
                                string str = "12345678";
                                string str1= "_opn";
                                string strRole = "rrel_";
                                int ArrySize = keyElemList.size();
                                int randNumber = 0;
                                if (trueOrFalse) {
                                    if (ArrySize > 6)
                                        ArrySize = 6;
                                    std::uniform_int_distribution<unsigned long long> distrLocal(1, ArrySize);
                                    randNumber = distrLocal(eng);
                                    for (int j = 0; j < randNumber; j++)
                                        templateParams.Add(str1 + str[j], keyElemList[j]);
                                } else {
                                    shuffle(keyElemListCorrect.begin(), keyElemListCorrect.end(), std::mt19937(std::random_device()()));
                                    vector<ScAddr> idtfList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElemListCorrect.front(), GenKeynodes::nrel_main_idtf);
                                    vector<ScAddr> idtfListSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElemListCorrect.front(), relationStruct);
                                    idtfList.insert(idtfList.end(), idtfListSub.begin(), idtfListSub.end());
                                    for (auto it1 = idtfList.begin(); it1 != idtfList.end();) {
                                        if (!ms_context->HelperCheckEdge(paramLanguage, *it1, ScType::EdgeAccessConstPosPerm))
                                            idtfList.erase(it1);
                                        else
                                            it1++;
                                    }
                                    if (idtfList.empty())
                                        continue;
                                    ArrySize = idtfList.size();
                                    if (ArrySize > 6)
                                        ArrySize = 6;
                                    std::uniform_int_distribution<unsigned long long> distrLocal(1, ArrySize);
                                    randNumber = distrLocal(eng);
                                    for (int j = 0; j < randNumber; j++)
                                        templateParams.Add(str1 + str[j], idtfList[j]);
//TODO 查找一个新的节点以及对应的标识符，用于替换当前节点的标识符
                                }
                                string strArrySize = to_string(trueOrFalse);
                                ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                ScAddr resultStructSub = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                strArrySize = to_string(randNumber);
                                elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructSub, elementRole);
                                ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::judgment_questions_based_on_relation_identifiers, ScType::EdgeAccessConstPosPerm)) {
                        ScAddr elemSubDomain = paramSubDom;
                        ScAddr roleStruct = paramRolRel;
                        ScAddr relationStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstNoRole);
                        std::random_device rd;
                        std::mt19937_64 eng(rd());
                        vector<ScAddr> elemDuplicate;
                        for (int i = 0; i < searchResult.Size(); i++) {
                            ScTemplateSearchResultItem searchResultItem = searchResult[i];
                            ScAddr keyElem = searchResultItem["_opkqr"];
                            vector<ScAddr> keyElemList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElem, relationStruct);
                            for (auto it = keyElemList.begin(); it != keyElemList.end();) {
                                if (!ms_context->HelperCheckEdge(paramLanguage, *it, ScType::EdgeAccessConstPosPerm))
                                    keyElemList.erase(it);
                                else
                                    it++;
                            }
                            vector<ScAddr> keyElemListCorrect = IteratorUtilsLocal::getAllByOutRelationWithType(ms_context.get(), elemSubDomain, roleStruct, ScType::NodeConstNoRole);
                            auto it = find(keyElemListCorrect.begin(), keyElemListCorrect.end(), keyElem);
                            if (it != keyElemListCorrect.end())
                                keyElemListCorrect.erase(it);
                            std::uniform_int_distribution<unsigned long long> distr(0, 1);
                            int trueOrFalse = distr(eng);
                            auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                            if (itDup == elemDuplicate.end() && ((trueOrFalse && !keyElemList.empty()) || (!trueOrFalse && !keyElemListCorrect.empty()))) {
                                ScTemplate resultStructTemplate;
                                ScTemplateParams  templateParams;
                                templateParams.Add("_opkqr", keyElem);
                                string str = "12345678";
                                string str1= "_opn";
                                string strRole = "rrel_";
                                int ArrySize = keyElemList.size();
                                int randNumber = 0;
                                if (trueOrFalse) {
                                    if (ArrySize > 6)
                                        ArrySize = 6;
                                    std::uniform_int_distribution<unsigned long long> distrLocal(1, ArrySize);
                                    randNumber = distrLocal(eng);
                                    for (int j = 0; j < randNumber; j++)
                                        templateParams.Add(str1 + str[j], keyElemList[j]);
                                } else {
                                    shuffle(keyElemListCorrect.begin(), keyElemListCorrect.end(), std::mt19937(std::random_device()()));
                                    vector<ScAddr> idtfList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElemListCorrect.front(), GenKeynodes::nrel_main_idtf);
                                    vector<ScAddr> idtfListSub = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), keyElemListCorrect.front(), relationStruct);
                                    idtfList.insert(idtfList.end(), idtfListSub.begin(), idtfListSub.end());
                                    for (auto it1 = idtfList.begin(); it1 != idtfList.end();) {
                                        if (!ms_context->HelperCheckEdge(paramLanguage, *it1, ScType::EdgeAccessConstPosPerm))
                                            idtfList.erase(it1);
                                        else
                                            it1++;
                                    }
                                    if (idtfList.empty())
                                        continue;
                                    ArrySize = idtfList.size();
                                    if (ArrySize > 6)
                                        ArrySize = 6;
                                    std::uniform_int_distribution<unsigned long long> distrLocal(1, ArrySize);
                                    randNumber = distrLocal(eng);
                                    for (int j = 0; j < randNumber; j++)
                                        templateParams.Add(str1 + str[j], idtfList[j]);
//TODO 查找一个新的节点以及对应的标识符，用于替换当前节点的标识符
                                }
                                string strArrySize = to_string(trueOrFalse);
                                ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                ScAddr resultStructSub = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                strArrySize = to_string(randNumber);
                                elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructSub, elementRole);
                                ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::judgment_questions_based_on_axiomatics, ScType::EdgeAccessConstPosPerm)) {
                        ScAddr elemSubDomain = IteratorUtilsLocal::getFirstWithType(ms_context.get(), initStruct, ScType::NodeConstStruct);
                        ScAddr roleStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), initStruct, ScType::NodeConstRole);
                        std::random_device rd;
                        std::mt19937_64 eng(rd());
                        vector<ScAddr> elemDuplicate;
                        for (int i = 0; i < searchResult.Size(); i++) {
                            ScTemplateSearchResultItem searchResultItem = searchResult[i];
                            ScAddr keyElem = searchResultItem["_opkqn"];
                            ScAddr elemOptionCS = searchResultItem["_opcsn"];
                            vector<ScAddr> keyElemList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemSubDomain, roleStruct);
                            auto it = find(keyElemList.begin(), keyElemList.end(), elemOptionCS);
                            if (it != keyElemList.end())
                                keyElemList.erase(it);
                            keyElem = IteratorUtilsLocal::getFirstFromSetWithType(ms_context.get(), keyElem, ScType::Link);
                            std::uniform_int_distribution<unsigned long long> distr(0, 1);
                            int trueOrFalse = distr(eng);
                            auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                            if (itDup == elemDuplicate.end() && keyElem.IsValid() && ((trueOrFalse && elemOptionCS.IsValid()) || (!trueOrFalse && !keyElemList.empty()))) {
                                ScTemplate resultStructTemplate;
                                ScTemplateParams  templateParams;
                                string strRole = "rrel_";
                                templateParams.Add("_opkqn", keyElem);
                                if (trueOrFalse)
                                    templateParams.Add("_opcsn", elemOptionCS);
                                else {
                                    shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
                                    templateParams.Add("_opcsn", keyElemList.front());
                                }
                                string strArrySize = to_string(trueOrFalse);
                                ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::judgment_questions_based_on_image_examples, ScType::EdgeAccessConstPosPerm)) {
                        ScAddr elemSubDomain = IteratorUtilsLocal::getFirstWithType(ms_context.get(), initStruct, ScType::NodeConstStruct);
                        ScAddr roleStruct = IteratorUtilsLocal::getFirstWithType(ms_context.get(), param, ScType::NodeConstRole);
                        std::random_device rd;
                        std::mt19937_64 eng(rd());
                        vector<ScAddr> elemDuplicate;
                        for (int i = 0; i < searchResult.Size(); i++) {
                            ScTemplateSearchResultItem searchResultItem = searchResult[i];
                            ScAddr keyElem = searchResultItem["_opkqn"];
                            ScAddr elemOptionCS = searchResultItem["_opcs"];
                            vector<ScAddr> keyElemList = IteratorUtilsLocal::getAllByOutRelation(ms_context.get(), elemSubDomain, roleStruct);
                            auto it = find(keyElemList.begin(), keyElemList.end(), elemOptionCS);
                            if (it != keyElemList.end())
                                keyElemList.erase(it);
                            std::uniform_int_distribution<unsigned long long> distr(0, 1);
                            int trueOrFalse = distr(eng);
                            auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                            if (itDup == elemDuplicate.end() && keyElem.IsValid() && ((trueOrFalse && elemOptionCS.IsValid()) || (!trueOrFalse && !keyElemList.empty()))) {
                                ScTemplate resultStructTemplate;
                                ScTemplateParams  templateParams;
                                string strRole = "rrel_";
                                templateParams.Add("_opkqn", keyElem);
                                if (trueOrFalse)
                                    templateParams.Add("_opcs", elemOptionCS);
                                else {
                                    shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
                                    templateParams.Add("_opcs", keyElemList.front());
                                }
                                string strArrySize = to_string(trueOrFalse);
                                ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
                            }
                        }
                    } else if (ms_context->HelperCheckEdge(param, GenKeynodes::judgment_questions_based_on_relation_attributes, ScType::EdgeAccessConstPosPerm)) {
                        std::random_device rd;
                        std::mt19937_64 eng(rd());
                        vector<ScAddr> elemDuplicate;
                        for (int i = 0; i < searchResult.Size(); i++) {
                            ScTemplateSearchResultItem searchResultItem = searchResult[i];
                            ScAddr keyElem = searchResultItem["_opkqr"];
                            vector<ScAddr> keyElemList = IteratorUtilsLocal::getAllWithTypeIn(ms_context.get(), keyElem, ScType::NodeConstClass);
                            sort(keyElemList.begin(), keyElemList.end(), cmp);
                            keyElemList.erase(unique(keyElemList.begin(), keyElemList.end(), equalScAddr), keyElemList.end());
                            for (auto it = keyElemList.begin(); it != keyElemList.end();) {
                                vector<ScAddr> conTupleList = IteratorUtilsLocal::getAllWithTypeIn(ms_context.get(), *it, ScType::NodeConstTuple);
                                if (conTupleList.empty())
                                    it = keyElemList.erase(it);
                                else {
                                    int flagTuple = 0;
                                    for (auto currElem : conTupleList) {
                                        if (ms_context->HelperCheckEdge(currElem, GenKeynodes::binary_relation, ScType::EdgeDCommonConst)) {
                                            flagTuple = 1;
                                            break;
                                        }
                                    }
                                    if (!flagTuple)
                                        it = keyElemList.erase(it);
                                    else
                                        it++;
                                }
                            }
                            std::uniform_int_distribution<unsigned long long> distr(0, 1);
                            int trueOrFalse = distr(eng);
                            auto itDup = find(elemDuplicate.begin(), elemDuplicate.end(), keyElem);
                            if (itDup == elemDuplicate.end() && !keyElemList.empty() && keyElemList.size() < 5 ) {
                                ScTemplate resultStructTemplate;
                                ScTemplateParams  templateParams;
                                templateParams.Add("_opkqr", keyElem);
                                string str = "12345";
                                string str1= "_op";
                                string strRole = "rrel_";
                                std::uniform_int_distribution<unsigned long long> distrLocal(1, keyElemList.size());
                                int randNumber = distrLocal(eng);
                                shuffle(keyElemList.begin(), keyElemList.end(), std::mt19937(std::random_device()()));
                                if (trueOrFalse) {
                                    for (int j = 0; j < randNumber; j++)
                                        templateParams.Add(str1 + str[j], keyElemList[j]);
                                } else {
                                    std::uniform_int_distribution<unsigned long long> distrCp(1, randNumber);
                                    int randNumberCp = distrCp(eng);
                                    for (int j = 0; j < randNumber; j++) {
                                        if (j < randNumberCp) {
                                            ScAddr elemTup;
                                            vector<ScAddr> elemTupList = IteratorUtilsLocal::getAllWithTypeIn(ms_context.get(), keyElemList[j], ScType::NodeConstTuple);
                                            for (auto tup: elemTupList) {
                                                if (ms_context->HelperCheckEdge(tup, GenKeynodes::binary_relation, ScType::EdgeDCommonConst)) {
                                                    elemTup = tup;
                                                    break;
                                                }
                                            }
                                            vector<ScAddr> keyElemListCorrect = IteratorUtils::getAllWithType(ms_context.get(), elemTup, ScType::NodeConstClass);
                                            for (auto currElemCp : keyElemListCorrect) {
                                                string strElem = ms_context->HelperGetSystemIdtf(currElemCp);
                                                if (keyElemList[j]!= currElemCp && strElem[0] != 'p') {
                                                    templateParams.Add(str1 + str[j], currElemCp);
                                                    break;
                                                }
                                            }
                                        } else
                                            templateParams.Add(str1 + str[j], keyElemList[j]);
                                    }
                                }
                                string strArrySize = to_string(trueOrFalse);
                                ScAddr elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                ScAddr resultStructSub = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStruct, elementRole);
                                strArrySize = to_string(randNumber);
                                elementRole = ms_context->HelperResolveSystemIdtf(strRole+strArrySize, ScType::NodeConstRole);
                                ScAddr resultStructCp = IteratorUtils::getFirstByOutRelation(ms_context.get(), resultStructSub, elementRole);
                                ms_context->HelperBuildTemplate(resultStructTemplate, resultStructCp);
                                QuestionGenerationProcess::templateGeneration(ms_context.get(), resultStructTemplate, templateParams, elemDuplicate, answer, keyElem);
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