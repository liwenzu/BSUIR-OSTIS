//
// Created by root on 4/15/21.
//
#pragma once
#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_iterator.hpp>

namespace answerVerificationModule
{
    class QuestionGenerationProcess
    {
    public:
        static void findAllElement(ScMemoryContext * ms_context, const  ScAddr &node, std::vector<ScAddr> &keyElemListCp, const ScAddr & relation);
    };
}

