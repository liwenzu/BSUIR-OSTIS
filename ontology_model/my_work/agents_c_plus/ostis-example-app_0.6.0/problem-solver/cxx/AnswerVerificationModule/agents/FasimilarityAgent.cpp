/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/
#include <iostream>
#include <sc-memory/sc_stream.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include "utils/Display.hpp"
#include "FasimilarityAgent.hpp"
#include "keynodes/keynodes.hpp"
#include "utils/Faprocrss.hpp"

using namespace std;
using namespace utils;

namespace answerVerificationModule {
    void FasimilarityAgent::SubstructureDecomposition(ScMemoryContext * ms_context,
                                                       ScAddr & elems_1, ScAddr & elem1,
                                                       ScAddr & elems_2, ScAddr & elem2)
    {
        Faprocess::decompositionCommon(ms_context, elems_1, elem1);
        Faprocess::decompositionCommon(ms_context, elems_2, elem2);
        Faprocess::decompositionAccwithrole(ms_context, elems_1, elem1);
        Faprocess::decompositionAccwithrole(ms_context, elems_2, elem2);
        Faprocess::decompositionTupleWithSub(ms_context, elems_1, elem1);
        Faprocess::decompositionTupleWithSub(ms_context, elems_2, elem2);
        Faprocess::decompositionTupleWithRelation(ms_context, elems_1, elem1);
        Faprocess::decompositionTupleWithRelation(ms_context, elems_2, elem2);
        Faprocess::decompositionEdge(ms_context, elems_1, elem1);
        Faprocess::decompositionEdge(ms_context, elems_2, elem2);
    }
    SC_AGENT_IMPLEMENTATION(FasimilarityAgent) {
    if (!edgeAddr.IsValid())
    return SC_RESULT_ERROR;
    ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
    ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(),questionNode);
    if (!param.IsValid())
    return SC_RESULT_ERROR_INVALID_PARAMS;
    ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
    ScAddr _elems_1, _elems_2, _elem1, _elem2;
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,param);
    Faprocess::generateTuple(ms_context.get(), answer, param, Keynodes::nrel_correct_answer, _elems_1, _elem1);
    Faprocess::generateTuple(ms_context.get(), answer, param, Keynodes::nrel_user_answer, _elems_2, _elem2);
    SubstructureDecomposition(ms_context.get(), _elems_1, _elem1, _elems_2,_elem2);
    int _summa = 0, _sumsta = 0, _sumcand = 0;
    vector<ScAddr> _allsst1 = IteratorUtils::getAllWithType(ms_context.get(), _elem1, ScType::NodeConstStruct);
    vector<ScAddr> _allsst2 = IteratorUtils::getAllWithType(ms_context.get(), _elem2, ScType::NodeConstStruct);
    _sumsta = _allsst1.size();
    _sumcand = _allsst2.size();
    vector<ScAddr> _classtup1, _classtup2, _classretup1, _classretup2, _classcomm1,
                   _classcomm2, _classedge1, _classedge2, _classpost51, _classpost52,
                   _classpost31, _classpost32, _mathstru, _mismathstru;
    Faprocess::SubstructureClassification(ms_context.get(),_allsst1,_classedge1,_classtup1,
                                         _classcomm1,_classretup1,_classpost51,_classpost31);
    Faprocess::SubstructureClassification(ms_context.get(),_allsst2,_classedge2,_classtup2,
                                       _classcomm2,_classretup2,_classpost52,_classpost32);
    Faprocess::CommonSimilarityCalculation(ms_context.get(), _classcomm1, _classcomm2, _summa, _mathstru);
    Faprocess::Post5SimilarityCalculation(ms_context.get(), _classpost51, _classpost52, _summa, _mathstru);
    Faprocess::Post3SimilarityCalculation(ms_context.get(), _classpost31, _classpost32, _summa, _mathstru);
    Faprocess::TupSimilarityCalculation(ms_context.get(), _classtup1, _classtup2, _summa, _mathstru);
    Faprocess::ReTupSimilarityCalculation(ms_context.get(), _classretup1, _classretup2, _summa, _mathstru);
    Faprocess::EdgeSimilarityCalculation(ms_context.get(), _classedge1, _classedge2, _summa, _mathstru);
    cout <<"Number of substructures of standard answers: "<<_sumsta << endl;
    cout <<"Number of substructures of user answers: "<<_sumcand << endl;
    cout <<"Number of substructures of matching answers: "<<_summa << endl;
    float _Fsc = Faprocess::SimilarityCalculation(_summa, _sumcand, _sumsta);
    Faprocess::MatchStructureStatistics(ms_context.get(), _mathstru, _allsst2, _mismathstru, answer, param);
    Faprocess::GenerateSimilarityLink(ms_context.get(), answer, param, _Fsc);
    AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
    return SC_RESULT_OK;
    }
}