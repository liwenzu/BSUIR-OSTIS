/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/
#include <iostream>
#include <sc-memory/sc_stream.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include <sc-agents-common/utils/display.hpp>
#include <sc-agents-common/utils/GenerationUtils.hpp>
#include "LosimilarityAgent.hpp"
#include "keynodes/keynodes.hpp"
#include "utils/Faprocrss.hpp"
#include "utils/Loprocess.hpp"

using namespace std;
using namespace utils;

namespace answerVerificationModule {

    SC_AGENT_IMPLEMENTATION(LosimilarityAgent) {
    if (!edgeAddr.IsValid())
        return SC_RESULT_ERROR;
    ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
    ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(),questionNode);
    if (!param.IsValid())
        return SC_RESULT_ERROR_INVALID_PARAMS;
    ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
//Judgement possible answer
    Loprocess::JudgmentPossibleAnswer(ms_context.get(), param);
//generate the basic structures
    ScAddr _elems_1, _elems_2, _elem1, _elem2;
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, param);
    Faprocess::generateTuple(ms_context.get(), answer, param, Keynodes::nrel_correct_answer, _elems_1, _elem1);
    Faprocess::generateTuple(ms_context.get(), answer, param, Keynodes::nrel_user_answer, _elems_2, _elem2);
//auto numbering sheaf
    vector<pair<ScAddr,int>> _elem_nbtups1;
    vector<pair<ScAddr,int>> _elem_nbtups2;
    Loprocess::AutomaticallyNumberSheaf(ms_context.get(), _elems_1, _elem_nbtups1);
    Loprocess::AutomaticallyNumberSheaf(ms_context.get(), _elems_2, _elem_nbtups2);
//auto numbering structure
    vector<pair<ScAddr,int>> _elem_strus1;
    vector<pair<ScAddr,int>> _elem_strus2;
    Loprocess::AutomaticallyNumberStructure(ms_context.get(), _elem_nbtups1, _elem_strus1);
    Loprocess::AutomaticallyNumberStructure(ms_context.get(), _elem_nbtups2, _elem_strus2);
//generate the substructures
    Loprocess::SubstructureDecomposition(ms_context.get(), _elem_nbtups1, _elem1);
    Loprocess::SubstructureDecomposition(ms_context.get(), _elem_nbtups2, _elem2);
// determine the domain of definition




/*
    for (auto i : _elem_strus1)
    {
        display::printEl(ms_context.get(), i.first);
        cout <<"The sequence:"<<i.second <<endl;
    }
    for (auto i : _elem_strus2)
    {
        display::printEl(ms_context.get(), i.first);
        cout <<"The sequence:"<<i.second <<endl;
    }

*/









    AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
    return SC_RESULT_OK;
}
}