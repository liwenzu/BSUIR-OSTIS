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
                                      unordered_map<ScAddr, pair<ScAddr,int>, ScAddrHashFunc< uint32_t >> &answerMapS,
                                      stack<int> &numberStructS, vector<list<ScAddr>> &answerMatchStatusS,
                                      unordered_map<ScAddr, bool, ScAddrHashFunc< uint32_t >> &flagMapS,
                                      unordered_map<ScAddr, int, ScAddrHashFunc< uint32_t >> &tempMatchStatusS);

        static void answerNumbering(ScMemoryContext *ms_context, const ScAddr &node,
                                      unordered_map<ScAddr, pair<ScAddr,int>, ScAddrHashFunc< uint32_t >> &answerMapS);

        static bool allEleInStru(ScMemoryContext *ms_context, ScTemplateSearchResultItem currSearchResultItemS, const ScAddr &node);

        static bool theoremChecking(ScMemoryContext *ms_context, const ScAddr &nodeS, const ScAddr &actionNode);

        static bool conditionChecking(ScMemoryContext *ms_context, const ScAddr &nodeS, const ScAddr &actionNode);

        static bool resultChecking(ScMemoryContext *ms_context, const ScAddr &nodeS, const ScTemplateSearchResultItem &currSearchResultItemS);

        static int traversalSubtree(ScMemoryContext *ms_context, const ScAddr &nodeS, unordered_map<ScAddr, int, ScAddrHashFunc< uint32_t >> &tempMatchStatusSS);

    };
}

