//
// Created by root on 4/15/21.
//
#include "QuestionGenerationProcess.hpp"
#include <iostream>
#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "utils/IteratorUtilsLocal.hpp"
using namespace utils;
namespace answerVerificationModule
{
    void QuestionGenerationProcess::findAllElement(ScMemoryContext * ms_context, const  ScAddr &node, vector<ScAddr> &keyElemListCp, const ScAddr & relation)
    {
        vector<ScAddr> elementList = IteratorUtilsLocal::getAllByOutRelation(ms_context, node, relation);
        if (elementList.empty())
            return;
        for (auto element : elementList)
        {
            keyElemListCp.push_back(element);
            findAllElement(ms_context, element, keyElemListCp, relation);
        }
    }
}