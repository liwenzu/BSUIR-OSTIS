//
// Created by root on 12/6/20.
//
#pragma once
#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_iterator.hpp>

namespace answerVerificationModule
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
        static void SubstructureClassification(
                ScMemoryContext * ms_context,
                std::vector<ScAddr>& allsst,
                std::vector<ScAddr>& classedge,
                std::vector<ScAddr>& classtup,
                std::vector<ScAddr>& classcomm,
                std::vector<ScAddr>& classretup,
                std::vector<ScAddr>& classpost5,
                std::vector<ScAddr>& classpost3);
        static void CommonSimilarityCalculation(
                ScMemoryContext * ms_context,
                std::vector<ScAddr>& classcomm1,
                std::vector<ScAddr>& classcomm2,
                int & summa,
                std::vector<ScAddr>& mathstru);
        static void Post5SimilarityCalculation(
                ScMemoryContext * ms_context,
                std::vector<ScAddr>& classpost51,
                std::vector<ScAddr>& classpost52,
                int & summa,
                std::vector<ScAddr>& mathstru);
        static void Post3SimilarityCalculation(
                ScMemoryContext * ms_context,
                std::vector<ScAddr>& classpost31,
                std::vector<ScAddr>& classpost32,
                int & summa,
                std::vector<ScAddr>& mathstru);
        static void TupSimilarityCalculation(
                ScMemoryContext * ms_context,
                std::vector<ScAddr>& classtup1,
                std::vector<ScAddr>& classtup2,
                int & summa,
                std::vector<ScAddr>& mathstru);
        static void ReTupSimilarityCalculation(
                ScMemoryContext * ms_context,
                std::vector<ScAddr>& classretup1,
                std::vector<ScAddr>& classretup2,
                int & summa,
                std::vector<ScAddr>& mathstru);
        static void EdgeSimilarityCalculation(
                ScMemoryContext * ms_context,
                std::vector<ScAddr>& classedge1,
                std::vector<ScAddr>& classedge2,
                int & summa,
                std::vector<ScAddr>& mathstru);
        static float SimilarityCalculation(
                int & summa,
                int & sumcand,
                int & sumsta);
        static void MatchStructureStatistics(
                ScMemoryContext * ms_context,
                std::vector<ScAddr>& mathstru,
                std::vector<ScAddr>& allsst2,
                std::vector<ScAddr>& mismathstru,
                const ScAddr & answer,
                const ScAddr & param);
        static void  GenerateSimilarityLink(
                ScMemoryContext * ms_context,
                const ScAddr & answer,
                const ScAddr & param,
                float Fsc);
    };
}
