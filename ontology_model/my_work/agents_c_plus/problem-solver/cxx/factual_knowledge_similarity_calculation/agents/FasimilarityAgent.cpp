/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/
#include <iostream>

#include <sc-memory/cpp/sc_stream.hpp>
#include <sc-kpm/sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-kpm/sc-agents-common/utils/AgentUtils.hpp>
#include <sc-kpm/sc-agents-common/utils/display.hpp>

#include "FasimilarityAgent.hpp"
#include "keynodes/keynodes.hpp"

using namespace std;
using namespace utils;

namespace facknowsimcalcu {

    SC_AGENT_IMPLEMENTATION(FasimilarityAgent) {
            if (!edgeAddr.IsValid())
            return SC_RESULT_ERROR;

            ScAddr _elems_1, _elems_2, _elem1, _elem2;

            ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
            ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(),questionNode);
            if (!param.IsValid())
            return SC_RESULT_ERROR_INVALID_PARAMS;
            ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
//为了在窗口显示正确答案部分
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,param);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,Keynodes::nrel_correct_answer);
            ScIterator5Ptr iterator5 = IteratorUtils::getIterator5(ms_context.get(),param,Keynodes::nrel_correct_answer,true);
            while(iterator5->Next())
            {
                _elems_1 = iterator5->Get(2);
                _elem1 = ms_context->CreateNode(ScType::NodeConstTuple);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,_elems_1);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,iterator5->Get(1));
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,iterator5->Get(3));
                ScAddr _arc1 = ms_context->CreateEdge(ScType::EdgeDCommonConst, _elem1,_elems_1);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,_arc1);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,_elem1);
                ScAddr _arc2 = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,Keynodes::nrel_relation_decomposition,_arc1);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,_arc2);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,Keynodes::nrel_relation_decomposition);
            }
//为了在窗口显示用户答案部分
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,Keynodes::nrel_user_answer);
            ScIterator5Ptr iterator5s = IteratorUtils::getIterator5(ms_context.get(),param,Keynodes::nrel_user_answer,true);
            while(iterator5s->Next())
            {
                _elems_2 = iterator5s->Get(2);
                _elem2 = ms_context->CreateNode(ScType::NodeConstTuple);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,_elems_2);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,iterator5s->Get(1));
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,iterator5s->Get(3));
                ScAddr _arc1 = ms_context->CreateEdge(ScType::EdgeDCommonConst,_elem2,_elems_2);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,_arc1);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,_elem2);
                ScAddr _arc2 = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,Keynodes::nrel_relation_decomposition,_arc1);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,_arc2);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,Keynodes::nrel_relation_decomposition);
            }
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&分解语义图
//首先找出结构中的所有通用弧
            vector<ScAddr> _class_comm1 = IteratorUtils::getAllWithType(ms_context.get(), _elems_1, ScType::EdgeDCommonConst);
            for (auto i : _class_comm1)
            {
                //判断通用弧的另一端是元组吗&&&&&&&&&&&&&&&
                ScAddr _els1 = ms_context->GetEdgeSource(i);
                if(ms_context->GetElementType(_els1) == ScType::NodeConstTuple)
                    continue;
                ScAddr _els2 = ms_context->GetEdgeTarget(i);
                ScAddr _elem = ms_context->CreateNode(ScType::NodeConstStruct);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem1, _elem);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, i);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els1);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els2);
                ScIterator3Ptr it_3 = ms_context->Iterator3(ScType::NodeConstNoRole, ScType::EdgeAccessConstPosPerm, i);
                while (it_3->Next())
                {
                    ScAddr _els3 = it_3->Get(0);
                    ScAddr _arc = it_3->Get(1);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els3);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arc);
                    break;
                }
            }
//首先找出结构中的所有通用弧(用户答案)
            vector<ScAddr> _class_comm2 = IteratorUtils::getAllWithType(ms_context.get(), _elems_2, ScType::EdgeDCommonConst);
            for (auto i : _class_comm2)
            {
                //判断通用弧的另一端是元组吗&&&&&&&&&&&&&&&
                ScAddr _els1 = ms_context->GetEdgeSource(i);
                if(ms_context->GetElementType(_els1) == ScType::NodeConstTuple)
                    continue;
                ScAddr _els2 = ms_context->GetEdgeTarget(i);
                ScAddr _elem = ms_context->CreateNode(ScType::NodeConstStruct);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem2, _elem);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, i);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els1);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els2);
                ScIterator3Ptr it_3 = ms_context->Iterator3(ScType::NodeConstNoRole, ScType::EdgeAccessConstPosPerm, i);
                while (it_3->Next())
                {
                    ScAddr _els3 = it_3->Get(0);
                    ScAddr _arc = it_3->Get(1);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els3);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arc);
                    break;
                }
                printEl(ms_context.get(), i);
                std::cout<<""<<std::endl;
            }












            AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);

            return SC_RESULT_OK;
    }
}
