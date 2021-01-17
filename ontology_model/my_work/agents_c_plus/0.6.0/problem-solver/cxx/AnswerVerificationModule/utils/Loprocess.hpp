//
// Created by root on 1/17/21.
//

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_iterator.hpp>
#include <sc-agents-common/utils/GenerationUtils.hpp>



namespace answerVerificationModule
{
    class Loprocess
    {
    public:
        static void AutomaticallyNumberSheaf(
                ScMemoryContext * ms_context,
                const ScAddr & mid_elems,
                std::vector<pair<ScAddr,int> >& elem_nbtups);
        static void AutomaticallyNumberStructure(
                ScMemoryContext * ms_context,
                std::vector<pair<ScAddr,int> >& elem_nbtups,
                std::vector<pair<ScAddr,int> >& elem_strus);
    };
}

