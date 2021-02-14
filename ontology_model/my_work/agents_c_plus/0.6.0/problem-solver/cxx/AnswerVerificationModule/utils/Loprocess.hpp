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
        static void JudgmentPossibleAnswer(
                ScMemoryContext * ms_context,
                const ScAddr & param);
        static void SubstructureDecomposition(
                ScMemoryContext * ms_context,
                const std::vector<pair<ScAddr,int> >& elem_nbtups,
                ScAddr & mid_elem
                );
        static void DetermineScopeQuantifier(
                ScMemoryContext * ms_context,
                const std::vector<pair<ScAddr,int> >& elem_strus,
                std::vector<ScAddr>& elemDeDoArc
                );
        static void RemoveUnnecessaryElements(
                ScMemoryContext * ms_context,
                std::vector<ScAddr>& elemDeDoArc
                );
        static void CreateMapping(
                ScMemoryContext * ms_context,
                const ScAddr & mid_elems1,
                const ScAddr & mid_elems2,
                const ScAddr & mid_elem1,
                const ScAddr & mid_elem2,
                std::vector<ScAddr>& elemMap,
                const std::vector<pair<ScAddr,int> >& elem_strus1,
                const std::vector<pair<ScAddr,int> >& elem_strus2
                );
    };
}

