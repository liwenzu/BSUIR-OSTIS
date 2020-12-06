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
#include "utils/Faprocrss.hpp"

using namespace std;
using namespace utils;

namespace facknowsimcalcu {

    SC_AGENT_IMPLEMENTATION(FasimilarityAgent) {
            if (!edgeAddr.IsValid())
            return SC_RESULT_ERROR;
            ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
            ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(),questionNode);
            if (!param.IsValid())
            return SC_RESULT_ERROR_INVALID_PARAMS;
            ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);

            ScAddr _elems_1, _elems_2, _elem1, _elem2;

//为了在窗口显示正确答案部分
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,param);
            Faprocess::generateTuple(ms_context.get(), answer, param, Keynodes::nrel_correct_answer, _elems_1, _elem1);
//为了在窗口显示用户答案部分
            Faprocess::generateTuple(ms_context.get(), answer, param, Keynodes::nrel_user_answer, _elems_2, _elem2);
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&分解语义图
//首先找出结构中的所有通用弧
            Faprocess::decompositionCommon(ms_context.get(), _elems_1, _elem1);
//首先找出结构中的所有通用弧(用户答案)
            Faprocess::decompositionCommon(ms_context.get(), _elems_2, _elem2);
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//找出结构中的所有归属弧(包括带角色关系)(标准答案)
            Faprocess::decompositionAccwithrole(ms_context.get(), _elems_1, _elem1);
//(用户答案)
            Faprocess::decompositionAccwithrole(ms_context.get(), _elems_2, _elem2);
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//寻找和生成细分结构(subdividing)(标准答案)
            Faprocess::decompositionTupleWithSub(ms_context.get(), _elems_1, _elem1);
            Faprocess::decompositionTupleWithSub(ms_context.get(), _elems_2, _elem2);
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//关系的元组结构分解和子结构生成(标准答案)
            Faprocess::decompositionTupleWithRelation(ms_context.get(), _elems_1, _elem1);
            Faprocess::decompositionTupleWithRelation(ms_context.get(), _elems_2, _elem2);
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//寻找通用边结构
            Faprocess::decompositionEdge(ms_context.get(), _elems_1, _elem1);
            Faprocess::decompositionEdge(ms_context.get(), _elems_2, _elem2);
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//相似度计算部分
            vector<ScAddr> _allsst1 = IteratorUtils::getAllWithType(ms_context.get(), _elem1, ScType::NodeConstStruct);
            int _sumsta = _allsst1.size();
            vector<ScAddr> _allsst2 = IteratorUtils::getAllWithType(ms_context.get(), _elem2, ScType::NodeConstStruct);
            int _sumcand = _allsst2.size();

            vector<ScAddr> _classtup1, _classtup2, _classretup1, _classretup2,
                           _classcomm1, _classcomm2, _classedge1, _classedge2,
                           _classpost51, _classpost52, _classpost31, _classpost32;


            for (auto i : _allsst1)
            {
                ScIterator3Ptr it_edge = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, ScType::EdgeUCommonConst);
                if (it_edge->Next())
                {
                    _classedge1.push_back(i);
                    continue;
                }
                ScIterator3Ptr it_common = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, ScType::EdgeDCommonConst);
                if (it_common->Next())
                {
                    ScAddr arc = it_common->Get(2);
                    ScAddr els1 = ms_context->GetEdgeSource(arc);
                    if (ms_context->GetElementType(els1) == ScType::NodeConstTuple)
                    {
                        _classtup1.push_back(i);
                        continue;
                    }
                    _classcomm1.push_back(i);
                    continue;
                }
                ScIterator3Ptr it_retup = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
                ScIterator3Ptr it_retup1 = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, ScType::NodeConstTuple);
                if (it_retup->Next() && it_retup1->Next())
                {
                    ScAddr els1 = it_retup->Get(2);
                    ScAddr els2 = it_retup1->Get(2);
                    ScIterator3Ptr it_3 = ms_context->Iterator3(els1, ScType::EdgeAccessConstPosPerm, els2);
                    if (it_3->Next())
                    {
                        _classretup1.push_back(i);
                        continue;
                    }
                }
                ScIterator3Ptr it_post5 = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, ScType::NodeConstRole);
                ScIterator3Ptr it_post51 = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, ScType::EdgeAccessConstPosPerm);
                if (it_post5->Next() && it_post51->Next())
                {
                    ScAddr els1 = it_post5->Get(2);
                    ScAddr els2 = it_post51->Get(2);
                    ScIterator3Ptr it_3 = ms_context->Iterator3(els1, ScType::EdgeAccessConstPosPerm, els2);
                    if (it_3->Next())
                    {
                        _classpost51.push_back(i);
                        continue;
                    }
                    ScAddr els3 = ms_context->GetEdgeSource(els2);
                    if (els1 == els3)
                    {
                        _classpost51.push_back(i);
                        continue;
                    }
                }
                _classpost31.push_back(i);
            }



        for (auto i : _allsst2)
        {
            ScIterator3Ptr it_edge = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, ScType::EdgeUCommonConst);
            if (it_edge->Next())
            {
                _classedge2.push_back(i);
                continue;
            }
            ScIterator3Ptr it_common = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, ScType::EdgeDCommonConst);
            if (it_common->Next())
            {
                ScAddr arc = it_common->Get(2);
                ScAddr els1 = ms_context->GetEdgeSource(arc);
                if (ms_context->GetElementType(els1) == ScType::NodeConstTuple)
                {
                    _classtup2.push_back(i);
                    continue;
                }
                _classcomm2.push_back(i);
                continue;
            }
            ScIterator3Ptr it_retup = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
            ScIterator3Ptr it_retup1 = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, ScType::NodeConstTuple);
            if (it_retup->Next() && it_retup1->Next())
            {
                ScAddr els1 = it_retup->Get(2);
                ScAddr els2 = it_retup1->Get(2);
                ScIterator3Ptr it_3 = ms_context->Iterator3(els1, ScType::EdgeAccessConstPosPerm, els2);
                if (it_3->Next())
                {
                    _classretup2.push_back(i);
                    continue;
                }
            }
            ScIterator3Ptr it_post5 = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, ScType::NodeConstRole);
            ScIterator3Ptr it_post51 = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, ScType::EdgeAccessConstPosPerm);
            if (it_post5->Next() && it_post51->Next())
            {
                ScAddr els1 = it_post5->Get(2);
                ScAddr els2 = it_post51->Get(2);
                ScIterator3Ptr it_3 = ms_context->Iterator3(els1, ScType::EdgeAccessConstPosPerm, els2);
                if (it_3->Next())
                {
                    _classpost52.push_back(i);
                    continue;
                }
                ScAddr els3 = ms_context->GetEdgeSource(els2);
                if (els1 == els3)
                {
                    _classpost52.push_back(i);
                    continue;
                }
            }
            _classpost32.push_back(i);
        }







//display::printEl(ms_context.get(), arc);

            cout <<_classcomm1.size() << endl;
            cout <<_classedge1.size() << endl;
            cout <<_classpost31.size() << endl;
            cout <<_classpost51.size() << endl;
            cout <<_classretup1.size() << endl;
            cout <<_classtup1.size() << endl;


        cout <<_sumsta << endl;



        cout <<_classcomm2.size() << endl;
        cout <<_classedge2.size() << endl;
        cout <<_classpost32.size() << endl;
        cout <<_classpost52.size() << endl;
        cout <<_classretup2.size() << endl;
        cout <<_classtup2.size() << endl;


            cout <<_sumcand << endl;









            AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);

            return SC_RESULT_OK;
    }
}
