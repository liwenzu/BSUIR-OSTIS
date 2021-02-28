//
// Created by root on 11/22/20.
//
#pragma once
#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_iterator.hpp>


namespace utils
{
    class display
    {
    public:
        static void printNl(ScMemoryContext * ms_context, ScAddr element);
        static void printEl(ScMemoryContext * ms_context, ScAddr element);
    };

}
