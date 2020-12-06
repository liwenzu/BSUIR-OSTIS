//
// Created by root on 12/6/20.
//

#include <sc-memory/cpp/sc_memory.hpp>
#include <sc-memory/cpp/sc_iterator.hpp>


namespace facknowsimcalcu
{
    class Faprocess
    {
    public:
        static void generateTuple(
                ScMemoryContext * ms_context,
                const ScAddr & answer,
                const ScAddr & param,
                const ScAddr & key_node,
                ScAddr & mid_elems,
                ScAddr & mid_elem);
        static void decompositionCommon(
                ScMemoryContext * ms_context,
                ScAddr & mid_elems,
                ScAddr & mid_elem);
        static void decompositionAccwithrole(
                ScMemoryContext * ms_context,
                ScAddr & mid_elems,
                ScAddr & mid_elem);
        static void decompositionTupleWithSub(
                ScMemoryContext * ms_context,
                ScAddr & mid_elems,
                ScAddr & mid_elem);
        static void decompositionTupleWithRelation(
                ScMemoryContext * ms_context,
                ScAddr & mid_elems,
                ScAddr & mid_elem);
        static void decompositionEdge(
                ScMemoryContext * ms_context,
                ScAddr & mid_elems,
                ScAddr & mid_elem);
    };

}
