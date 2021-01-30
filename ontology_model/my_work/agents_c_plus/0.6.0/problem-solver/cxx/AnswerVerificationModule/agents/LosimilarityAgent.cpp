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
    for (auto elsTup : _elem_nbtups1)
    {
        ScAddr genStruct = ms_context->CreateNode(ScType::NodeConstStruct);
        ScIterator3Ptr it_3 = ms_context->Iterator3(ScType::NodeConstNoRole, ScType::EdgeAccessConstPosPerm, elsTup.first);
        while (it_3->Next())
        {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem1, genStruct);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct, it_3->Get(0));
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct, it_3->Get(1));
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct, elsTup.first);
        }
        vector<ScAddr> elsTupCp = IteratorUtils::getAllWithType(ms_context.get(), elsTup.first, ScType::Unknown);
        //TODO record the structure
        for (auto elemStruct : elsTupCp)
        {
            ScAddr arc;
            it_3 = ms_context->Iterator3(elsTup.first, ScType::EdgeAccessConstPosPerm, elemStruct);
            while (it_3->Next())
            {
                arc = it_3->Get(1);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct, arc);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct, elemStruct);
            }
            ScIterator3Ptr it_31 = ms_context->Iterator3(ScType::NodeConstRole, ScType::EdgeAccessConstPosPerm, arc);
            if (it_31->Next())
            {
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct, it_31->Get(0));
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct, it_31->Get(1));
            }
            if (ms_context->GetElementType(elemStruct) == ScType::NodeConstTuple)
                continue;
            else
            {
                it_31 = ms_context->Iterator3(ScType::NodeClass, ScType::EdgeAccessConstPosPerm, elemStruct);
                if (it_31->Next())
                {
                    ScAddr genStruct1 = ms_context->CreateNode(ScType::NodeConstStruct);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem1, genStruct1);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, elemStruct);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(0));
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                    //TODO record the structure
                }
                it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, ScType::EdgeAccessVarPosPerm);
                if (it_31->Next())
                {
                // find the structure of common
                    vector<ScAddr> commArc = IteratorUtils::getAllWithType(ms_context.get(), elemStruct, ScType::EdgeDCommonVar);
                    for (auto elem : commArc)
                    {
                        ScAddr els1, els2;
                        els1 = ms_context->GetEdgeSource(elem);
                        els2 = ms_context->GetEdgeTarget(elem);
                        if (ms_context->GetElementType(els1) == ScType::NodeVarTuple ||
                            ms_context->GetElementType(els1) == ScType::EdgeUCommonVar ||
                            ms_context->GetElementType(els1) == ScType::EdgeDCommonVar)
                            continue;
                        ScAddr genStruct1 = ms_context->CreateNode(ScType::NodeConstStruct);
                        //TODO record the structure
                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem1, genStruct1);
                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, elemStruct);
                        ScIterator3Ptr it_32 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els1);
                        while (it_32->Next())
                        {
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els1);
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_32->Get(1));
                        }
                        it_32 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, elem);
                        while (it_32->Next())
                        {
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, elem);
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_32->Get(1));
                        }
                        it_32 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els2);
                        while (it_32->Next())
                        {
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els2);
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_32->Get(1));
                        }
                        it_32 = ms_context->Iterator3(ScType::NodeNoRole, ScType::EdgeAccessVarPosPerm, elem);
                        ScIterator3Ptr it_33 = ms_context->Iterator3(ScType::NodeVar, ScType::EdgeAccessVarPosPerm, elem);
                        if (it_32->Next() || it_33->Next())
                        {
                            ScAddr els3;
                            els3 = it_32->Get(0).IsValid()? it_32->Get(0):it_33->Get(0);
                            arc = it_32->Get(1).IsValid()? it_32->Get(1):it_33->Get(1);
                            it_3 = ms_context->Iterator3(ScType::NodeConstRole, ScType::EdgeAccessVarPosPerm, arc);
                            if (it_3->Next())
                            {
                                els1 = it_3->Get(0);
                                els2 = it_3->Get(1);
                                it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els1);
                                while (it_31->Next())
                                {
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els1);
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                }
                                it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els2);
                                while (it_31->Next())
                                {
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els2);
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                }
                            }
                            it_3 = ms_context->Iterator3(elem, ScType::EdgeDCommonVar, ScType::NodeVar);
                            if (!it_3->Next())
                            {
                                it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els3);
                                while (it_31->Next())
                                {
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els3);
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                }
                                it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, arc);
                                while (it_31->Next())
                                {
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, arc);
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                }
                            }
                        }
                        ScIterator5Ptr  it_5 = ms_context->Iterator5(elem, ScType::EdgeDCommonVar, ScType::Unknown, ScType::EdgeAccessVarPosPerm, ScType::NodeConstNoRole);
                         if (it_5->Next())
                         {
                             ScAddr els3;
                             els1 = it_5->Get(1);
                             els2 = it_5->Get(2);
                             arc = it_5->Get(3);
                             els3 = it_5->Get(4);
                             it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els1);
                             while (it_31->Next())
                             {
                                 ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els1);
                                 ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                             }
                             it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els2);
                             while (it_31->Next())
                             {
                                 ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els2);
                                 ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                             }
                             it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, arc);
                             while (it_31->Next())
                             {
                                 ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, arc);
                                 ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                             }
                             it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els3);
                             while (it_31->Next())
                             {
                                 ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els3);
                                 ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                             }
                             it_31 = ms_context->Iterator3(ScType::NodeNoRole, ScType::EdgeAccessVarPosPerm, elem);
                             if (it_31->Next())
                             {
                                 els3 = it_31->Get(0);
                                 arc = it_31->Get(1);
                                 els1 = ms_context->GetEdgeSource(elem);
                                 els2 = ms_context->GetEdgeTarget(elem);
                                 genStruct1 = ms_context->CreateNode(ScType::NodeConstStruct);
                                 ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem1, genStruct1);
                                 ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, elemStruct);
                                 it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els1);
                                 while (it_31->Next())
                                 {
                                     ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els1);
                                     ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                 }
                                 it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, elem);
                                 while (it_31->Next())
                                 {
                                     ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, elem);
                                     ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                 }
                                 it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els2);
                                 while (it_31->Next())
                                 {
                                     ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els2);
                                     ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                 }
                                 it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els3);
                                 while (it_31->Next())
                                 {
                                     ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els3);
                                     ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                 }
                                 it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, arc);
                                 while (it_31->Next())
                                 {
                                     ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, arc);
                                     ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                 }
                             }
                         }
                    }






                    //TODO
                } else{
                    it_3 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, ScType::Unknown);
                    while (it_3->Next())
                    {
                        ScIterator3Ptr it_32 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, it_3->Get(2));
                        while (it_32->Next())
                        {
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct, it_32->Get(1));
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct, it_3->Get(2));
                        }
                    }
                }







            }


        }
    }





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