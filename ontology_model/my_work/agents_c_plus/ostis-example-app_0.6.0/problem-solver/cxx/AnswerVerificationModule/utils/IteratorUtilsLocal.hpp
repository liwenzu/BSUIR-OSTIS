//
// Created by root on 3/6/21.
//

#pragma once

#include <string>

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_type.hpp>
#include <sc-memory/sc_iterator.hpp>

using namespace std;

namespace answerVerificationModule
{
    class IteratorUtilsLocal
    {
    public:

        static vector<ScAddr> getAllWithTypeVar(ScMemoryContext * ms_context, const ScAddr & set, ScType scType);

        static vector<ScAddr> getAllWithTypeIn(ScMemoryContext * ms_context, const ScAddr & set, ScType scType);

        static vector<ScAddr> getAllByOutRelation(ScMemoryContext * ms_context, const ScAddr & node, const ScAddr & relation);

        static ScAddr getFirstWithType(ScMemoryContext * ms_context, const ScAddr & set, ScType scType);

        static vector<ScAddr> getAllByOutRelationWithType(ScMemoryContext * ms_context, const ScAddr & set, ScAddr const & relation, ScType scType);

        static ScAddr getFirstFromSetWithType(ScMemoryContext * ms_context, const ScAddr & set, ScType scType);

        static ScAddr getFirstFromSetByInReWithType(ScMemoryContext * ms_context, const ScAddr & set, ScType scType);
    };
}