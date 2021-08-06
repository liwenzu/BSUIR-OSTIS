//
// Created by root on 8/5/21.
//

#pragma once
#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_iterator.hpp>
#include "utils/Display.hpp"
#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <stack>

namespace answerVerificationModule
{
    class AnswerCheckProcess
    {
    public:
        static void traversalTemplate(ScMemoryContext *ms_context, const ScAddr &node,
            unordered_map<ScAddr, pair<ScAddr,int>, ScAddrHashFunc< uint32_t >> &answerMapS, stack<int> &numberStructS);

        static void answerNumbering(ScMemoryContext *ms_context, const ScAddr &node,
            unordered_map<ScAddr, pair<ScAddr,int>, ScAddrHashFunc< uint32_t >> &answerMapS);

        static bool allEleInStru(ScMemoryContext *ms_context, ScTemplateSearchResultItem currSearchResultItemS, const ScAddr &node);



    };
}

