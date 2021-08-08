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
            unordered_map<ScAddr, pair<ScAddr,int>, ScAddrHashFunc< uint32_t >> &answerMapS, stack<int> &numberStructS)
    {

//深度优先搜索
        cout << "-----------------------------" << endl;
//        Display::printEl(ms_context, node);

        int tempNum = -1;


        if (!numberStructS.empty())
            cout << "The last number of structur:"  << numberStructS.top() << endl;



        bool flag = false;
        ScTemplate currTemplate;
        ScTemplateSearchResult currSearchResult;
        if (ms_context->HelperBuildTemplate(currTemplate, node) && ms_context->HelperSearchTemplate(currTemplate, currSearchResult)) {

//            cout << currSearchResult.Size() << endl;
            for (int i = 0; i < currSearchResult.Size(); i++) {




                ScTemplateSearchResultItem currSearchResultItem = currSearchResult[i];
                for (int j = 0; j < currSearchResultItem.Size(); j++) {
                    if(ms_context->GetElementType(currSearchResultItem[j]) != ScType::EdgeAccessConstPosPerm)
                        continue;
                    ScAddr elemStru = IteratorUtilsLocal::getFirstFromSetByInReWithType(ms_context, currSearchResultItem[j], ScType::NodeConstStruct);
                    if (elemStru.IsValid() && answerMapS.count(elemStru)) {
                        if (AnswerCheckProcess::allEleInStru(ms_context, currSearchResult[i], elemStru))
                        {
                            flag = true;

                            tempNum = answerMapS[elemStru].second;

                            cout << "AAAAAAAAAAAAAA" << endl;
                        }

                        break;
                    }
                }
                if (flag)
                    break;
            }
        }





        cout << "*******************************" << endl;



        vector<ScAddr> elemVec = IteratorUtils::getAllByInRelation(ms_context, node, Keynodes::nrel_basic_sequence);
        if (elemVec.empty())
            return;


//设置一个栈，在这里入栈(如果第一个遍历的结构不存在，则跳过这步，如果中间结构不匹配则选上一个结构);
        if (tempNum == -1 && !numberStructS.empty())
            numberStructS.push(numberStructS.top());
        else if (tempNum != -1)
            numberStructS.push(tempNum);



        for(auto const &currElem : elemVec)
            AnswerCheckProcess::traversalTemplate(ms_context, currElem, answerMapS, numberStructS);

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





}



