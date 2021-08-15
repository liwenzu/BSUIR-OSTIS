//
// Created by root on 8/5/21.
//
//
// Created by root on 4/15/21.
//
#include <iostream>
#include "Foprocess.hpp"
#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "utils/IteratorUtilsLocal.hpp"
#include "keynodes/keynodes.hpp"

using namespace utils;
namespace answerVerificationModule
{

    void AnswerCheckProcess::answerNumbering(ScMemoryContext *ms_context, const ScAddr &node,
                                unordered_map<ScAddr, pair<ScAddr,int>, ScAddrHashFunc< uint32_t >> &answerMapS) {
        int num = 1;
        ScAddr elemStru = IteratorUtils::getFirstByOutRelation(ms_context, node, Keynodes::nrel_result);
        if (elemStru.IsValid())
            answerMapS[elemStru] = make_pair(node, num);
        ScAddr currNode = IteratorUtils::getFirstByOutRelation(ms_context, node, Keynodes::nrel_sequence_of_actions);
        while (currNode.IsValid()) {
            elemStru = IteratorUtils::getFirstByOutRelation(ms_context, currNode, Keynodes::nrel_result);
            if (elemStru.IsValid())
                answerMapS[elemStru] = make_pair(currNode, ++num);
            currNode = IteratorUtils::getFirstByOutRelation(ms_context, currNode, Keynodes::nrel_sequence_of_actions);
        }
    }



    void AnswerCheckProcess::traversalTemplate(ScMemoryContext *ms_context, const ScAddr &node,
                                               unordered_map<ScAddr, pair<ScAddr,int>, ScAddrHashFunc< uint32_t >> &answerMapS,
                                               stack<int> &numberStructS, vector<list<ScAddr>> &answerMatchStatusS,
                                               unordered_map<ScAddr, bool, ScAddrHashFunc< uint32_t >> &flagMapS,
                                               unordered_map<ScAddr, int, ScAddrHashFunc< uint32_t >> &tempMatchStatusS) {
//深度优先搜索
        cout << "-----------------------------" << endl;


        ScAddr elemStruFlag;
        int tempNum = -1;


        if (!numberStructS.empty())
            cout << "The last number of structur:"  << numberStructS.top() << endl;


        bool flag = false;
        ScTemplate currTemplate;
        ScTemplateSearchResult currSearchResult;
        if (ms_context->HelperBuildTemplate(currTemplate, node) && ms_context->HelperSearchTemplate(currTemplate, currSearchResult)) {
            for (int i = 0; i < currSearchResult.Size(); i++) {
                ScTemplateSearchResultItem currSearchResultItem = currSearchResult[i];
                for (int j = 0; j < currSearchResultItem.Size(); j++) {
                    if(ms_context->GetElementType(currSearchResultItem[j]) != ScType::EdgeAccessConstPosPerm)
                        continue;
                    ScAddr elemStru = IteratorUtilsLocal::getFirstFromSetByInReWithType(ms_context, currSearchResultItem[j], ScType::NodeConstStruct);
                    if (elemStru.IsValid() && answerMapS.count(elemStru)) {
                        if (AnswerCheckProcess::allEleInStru(ms_context, currSearchResult[i], elemStru)) {
                            ScIterator5Ptr it5 = ms_context->Iterator5(node, ScType::EdgeUCommonConst, ScType::NodeConstStruct, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_equality);
                            ScIterator5Ptr it51 = ms_context->Iterator5(ScType::NodeConstStruct, ScType::EdgeUCommonConst, node, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_equality);
                            if(it5->Next() || it51->Next()) {
                                if (!flagMapS[elemStru] &&
                                    AnswerCheckProcess::theoremChecking(ms_context, node, answerMapS[elemStru].first) &&
                                    AnswerCheckProcess::conditionChecking(ms_context, node, elemStru) &&
                                    AnswerCheckProcess::resultChecking(ms_context, node, currSearchResultItem) &&
                                    (numberStructS.empty() || (!numberStructS.empty() && tempNum < numberStructS.top()))) {
                                    flagMapS[elemStru] = true;
                                    flag = true;
                                    tempNum = answerMapS[elemStru].second;
                                    answerMatchStatusS[0].push_back(elemStru);
                                    elemStruFlag = elemStru;

                                    cout << "BBBBBBBBBBBBB" << endl;
                                } else
                                    continue;
                            } else {
                                flag = true;
                                tempNum = answerMapS[elemStru].second;
//检查答案步骤逻辑是否正确(入过逻辑步骤不正确，或许可以将tempNum设为-1,即使用上一个编号替代当前编号，需要后续具体讨论)
                                if (!numberStructS.empty() && tempNum >= numberStructS.top()) {
                                    answerMatchStatusS[1].push_back(elemStru);
                                    break;
                                }
//检查答案使用的定理正确吗
                                if(!AnswerCheckProcess::theoremChecking(ms_context, node, answerMapS[elemStru].first)) {
                                    answerMatchStatusS[2].push_back(elemStru);
                                    break;
                                }
//检查使用的条件正确吗
                                if(!AnswerCheckProcess::conditionChecking(ms_context, node, elemStru)) {
                                    answerMatchStatusS[3].push_back(elemStru);
                                    break;
                                }
//检查结论正确吗,即链接正确吗
                                if (!AnswerCheckProcess::resultChecking(ms_context, node, currSearchResultItem)) {
                                    answerMatchStatusS[4].push_back(elemStru);
                                    break;
                                }
//记录正确的答案
                                answerMatchStatusS[0].push_back(elemStru);
                                elemStruFlag = elemStru;

                                cout << "AAAAAAAAAAAAAA" << endl;
                            }
                        }
                        break;
                    }
                }
                if (flag)
                    break;
            }
        }

        cout << "*******************************" << endl;


//记录模板和答案匹配关系
        tempMatchStatusS[node] = tempNum;
//判断是叶子节点吗
        vector<ScAddr> elemVec = IteratorUtils::getAllByInRelation(ms_context, node, Keynodes::nrel_basic_sequence);
        if (elemVec.empty())
            return;
//设置一个栈，在这里入栈(如果第一个遍历的结构不存在，则跳过这步，如果中间结构不匹配则选上一个结构);
        if (tempNum == -1 && !numberStructS.empty())
            numberStructS.push(numberStructS.top());
        else if (tempNum != -1)
            numberStructS.push(tempNum);
//递归调用
        for(auto const &currElem : elemVec) {
            if (tempMatchStatusS.count(currElem)) {
                if (elemStruFlag.IsValid() && (tempNum <= AnswerCheckProcess::traversalSubtree(ms_context, currElem, tempMatchStatusS))) {
                    answerMatchStatusS[1].push_back(answerMatchStatusS[0].back());
                    answerMatchStatusS[0].pop_back();
                }
                continue;
            }
            AnswerCheckProcess::traversalTemplate(ms_context, currElem, answerMapS, numberStructS, answerMatchStatusS, flagMapS, tempMatchStatusS);
        }
//循环结束出栈;
        if (!numberStructS.empty())
            numberStructS.pop();
    }


    bool AnswerCheckProcess::allEleInStru(ScMemoryContext *ms_context, ScTemplateSearchResultItem currSearchResultItemS, const ScAddr &node) {
        bool flag = true;
        for (int i=0;i<currSearchResultItemS.Size();i++) {
            if (!ms_context->HelperCheckEdge(node, currSearchResultItemS[i], ScType::EdgeAccessConstPosPerm)) {
                flag = false;
                break;
            }
        }
        return flag;
    }

    bool AnswerCheckProcess::theoremChecking(ScMemoryContext *ms_context, const ScAddr &nodeS, const ScAddr &actionNode) {
        ScAddr elemTheorem = IteratorUtils::getFirstByOutRelation(ms_context, nodeS, Keynodes::nrel_axioms_used);
        if (!elemTheorem.IsValid())
            return true;
        ScIterator5Ptr it5 = ms_context->Iterator5(actionNode, ScType::EdgeAccessConstPosPerm, elemTheorem, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_2);
        return it5->Next();
    }

    bool AnswerCheckProcess::conditionChecking(ScMemoryContext *ms_context, const ScAddr &nodeS, const ScAddr &actionNode) {
        ScAddr currElem = IteratorUtils::getFirstByOutRelation(ms_context, nodeS, Keynodes::nrel_key_sc_element);
        if (!currElem.IsValid())
            return true;
        vector<ScAddr> currElemList = IteratorUtils::getAllWithType(ms_context, currElem, ScType::Unknown);
        for (auto const &elem : currElemList) {
            ScIterator3Ptr it3 = ms_context->Iterator3(elem, ScType::EdgeAccessConstPosPerm, ScType::Unknown);
            bool flag = false;
            while (it3->Next()) {
                if (ms_context->HelperCheckEdge(actionNode, it3->Get(2), ScType::EdgeAccessConstPosPerm)) {
                    flag = true;
                    break;
                }
            }
            if (!flag)
                return false;
        }
        return true;
    }

    bool AnswerCheckProcess::resultChecking(ScMemoryContext *ms_context, const ScAddr &nodeS, const ScTemplateSearchResultItem &currSearchResultItemS) {
        ScAddr currElem = IteratorUtils::getFirstByOutRelation(ms_context, nodeS, Keynodes::nrel_result);
        if (!currElem.IsValid())
            return true;
        ScAddr currElemCp = currSearchResultItemS["_value_link"];
        string str1 = CommonUtils::readString(ms_context, currElem);
        string str2 = CommonUtils::readString(ms_context, currElemCp);
        if (str1.empty() || str2.empty())
            return false;
        stringstream streamStr1(str1);
        stringstream streamStr2(str2);
        float number1, number2;
        streamStr1 >> number1;
        streamStr2 >> number2;
        return number1 == number2;
    }

    int AnswerCheckProcess::traversalSubtree(ScMemoryContext *ms_context, const ScAddr &nodeS,
            unordered_map<ScAddr, int, ScAddrHashFunc< uint32_t >> &tempMatchStatusSS) {
        int number = tempMatchStatusSS[nodeS];
        vector<ScAddr> elemVec = IteratorUtils::getAllByInRelation(ms_context, nodeS, Keynodes::nrel_basic_sequence);
        if (elemVec.empty()) {
            return number;
        }
        for(const auto &currElem : elemVec)
            number = max(AnswerCheckProcess::traversalSubtree(ms_context, currElem, tempMatchStatusSS), number);
        return number;
    }


}



