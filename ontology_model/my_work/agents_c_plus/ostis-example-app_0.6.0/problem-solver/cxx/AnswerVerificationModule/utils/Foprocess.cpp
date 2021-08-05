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

    void AnswerCheckProcess::traversalTemplate(ScMemoryContext *ms_context, const ScAddr &node)
    {

//深度优先搜索
        cout << "-----------------------------" << endl;
//        Display::printEl(ms_context, node);




        ScTemplate currTemplate;
        if (ms_context->HelperBuildTemplate(currTemplate, node)) {
            ScTemplateSearchResult currSearchResult;
            if (ms_context->HelperSearchTemplate(currTemplate, currSearchResult))
                cout << currSearchResult.Size() << endl;
        }


        cout << "*******************************" << endl;



        vector<ScAddr> elemVec = IteratorUtils::getAllByInRelation(ms_context, node, Keynodes::nrel_basic_sequence);
        if (elemVec.empty())
            return;

//设置一个栈，在这里入栈


        for(auto const &currElem : elemVec)
            AnswerCheckProcess::traversalTemplate(ms_context, currElem);
//循环结束出栈;
    }

}
