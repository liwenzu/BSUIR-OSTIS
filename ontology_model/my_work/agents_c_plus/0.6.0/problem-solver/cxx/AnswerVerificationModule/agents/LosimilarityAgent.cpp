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
    ScAddr _elems_1, _elems_2, _elem1, _elem2;
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, param);
    Faprocess::generateTuple(ms_context.get(), answer, param, Keynodes::nrel_correct_answer, _elems_1, _elem1);
    Faprocess::generateTuple(ms_context.get(), answer, param, Keynodes::nrel_user_answer, _elems_2, _elem2);


//auto numbering
    ScAddr _elemmed;
    ScIterator3Ptr it_3 = ms_context->Iterator3(_elems_1, ScType::EdgeAccessConstPosPerm, Keynodes::negation);
    if (it_3->Next())
    {
        cout <<"The structure includes negation node" << endl;
    }
//find the first node
    vector<ScAddr> _esaall = IteratorUtils::getAllWithType(ms_context.get(), _elems_1, ScType::NodeConstTuple);
    for (auto elem : _esaall)
    {
        ScIterator3Ptr it_3 = ms_context->Iterator3(ScType::NodeConstTuple, ScType::EdgeAccessConstPosPerm, elem);
        if (it_3->Next())
            continue;
        _elemmed = elem;
        break;
    }
    ScAddr _elem_nbtups = ms_context->CreateNode(ScType::NodeConst);
    ScAddr _elemmed1 = Keynodes::rrel_0;
    vector<vector<ScAddr>> _classpoint;
//tree
     while (1)
     {
         while (1)
         {
             GenerationUtils::generateRelationBetween(ms_context.get(), _elem_nbtups, _elemmed, _elemmed1);
             ScIterator5Ptr it_5 = ms_context->Iterator5(_elemmed1, ScType::EdgeDCommonConst, ScType::NodeConstRole,
                                                         ScType::EdgeAccessConstPosPerm, Keynodes::nrel_basic_sequence);
             while (it_5->Next())
             {
                 _elemmed1 = it_5->Get(2);
             }

             vector<ScAddr> _elstup = IteratorUtils::getAllWithType(ms_context.get(), _elemmed, ScType::NodeConstTuple);
             if (_elstup.empty())
                 break;
             _elemmed = _elstup.back();
             _elstup.pop_back();
             if (_elstup.empty())
                 continue;
             _classpoint.push_back(_elstup);
         }
         if (_classpoint.empty())
             break;
         _elemmed = _classpoint.back().back();
         _classpoint.back().pop_back();
         if (_classpoint.back().empty())
         {
             _classpoint.pop_back();
         }
     }
















    cout <<"The length:"<<_esaall.size()<< endl;




    AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
    return SC_RESULT_OK;
}
}