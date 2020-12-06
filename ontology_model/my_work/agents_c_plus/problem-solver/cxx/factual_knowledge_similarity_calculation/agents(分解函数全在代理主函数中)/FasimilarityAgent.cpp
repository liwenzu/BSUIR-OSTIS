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
            }
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//找出结构中的所有归属弧(包括带角色关系)(标准答案)
            vector<ScAddr> _class_arc1 = IteratorUtils::getAllWithType(ms_context.get(), _elems_1, ScType::EdgeAccessConstPosPerm);
            for (auto i : _class_arc1)
            {
//查找三元构造另一端是通用弧吗？
               ScAddr _els2 = ms_context->GetEdgeTarget(i);
               ScAddr _els1 = ms_context->GetEdgeSource(i);
               if (ms_context->GetElementType(_els2) == ScType::EdgeDCommonConst
                || ms_context->GetElementType(_els2) == ScType::EdgeUCommonConst
                || ms_context->GetElementType(_els2) == ScType::EdgeAccessConstPosPerm
                || ms_context->GetElementType(_els2) == ScType::NodeConstTuple
                || ms_context->GetElementType(_els1) == ScType::NodeConstTuple)
                   continue;
                ScAddr _elem = ms_context->CreateNode(ScType::NodeConstStruct);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem1, _elem);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els1);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els2);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, i);
//判断弧的另一端有角色关系吗
                ScIterator3Ptr it_3 = ms_context->Iterator3(ScType::NodeConstRole, ScType::EdgeAccessConstPosPerm, i);
                while (it_3->Next())
                {
                    ScAddr _els3 = it_3->Get(0);
                    ScAddr _arc = it_3->Get(1);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els3);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arc);
                    break;
                }
            }
//找出结构中的所有归属弧(包括带角色关系)(用户答案)
        vector<ScAddr> _class_arc2 = IteratorUtils::getAllWithType(ms_context.get(), _elems_2, ScType::EdgeAccessConstPosPerm);
        for (auto i : _class_arc2)
        {
//查找三元构造另一端是通用弧吗？
            ScAddr _els2 = ms_context->GetEdgeTarget(i);
            ScAddr _els1 = ms_context->GetEdgeSource(i);
            if (ms_context->GetElementType(_els2) == ScType::EdgeDCommonConst
                || ms_context->GetElementType(_els2) == ScType::EdgeUCommonConst
                || ms_context->GetElementType(_els2) == ScType::EdgeAccessConstPosPerm
                || ms_context->GetElementType(_els2) == ScType::NodeConstTuple
                || ms_context->GetElementType(_els1) == ScType::NodeConstTuple)
                continue;
            ScAddr _elem = ms_context->CreateNode(ScType::NodeConstStruct);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem2, _elem);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els1);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els2);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, i);
//判断弧的另一端有角色关系吗
            ScIterator3Ptr it_3 = ms_context->Iterator3(ScType::NodeConstRole, ScType::EdgeAccessConstPosPerm, i);
            while (it_3->Next())
            {
                ScAddr _els3 = it_3->Get(0);
                ScAddr _arc = it_3->Get(1);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els3);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arc);
                break;
            }
        }
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//寻找和生成细分结构(subdividing)(标准答案)
        vector<ScAddr> _class_tups1 = IteratorUtils::getAllWithType(ms_context.get(), _elems_1, ScType::NodeConstTuple);
        for (auto i : _class_tups1)
        {
            ScAddr _arccom, _els2, _arc, _els3 ;
            ScIterator5Ptr it_5 = ms_context->Iterator5(i, ScType::EdgeDCommonConst,
                                                        ScType::Unknown,
                                                        ScType::EdgeAccessConstPosPerm,
                                                        ScType::NodeConstNoRole);
            if (!(it_5->Next()))
                continue;
            _arccom = it_5->Get(1);
            _els2 = it_5->Get(2);
            _arc = it_5->Get(3);
            _els3 = it_5->Get(4);
            vector<ScAddr> _elemtup = IteratorUtils::getAllWithType(ms_context.get(), i, ScType::Unknown);
            if (_elemtup.empty())
               continue;
//先将找到的元组上半部分加入到生成的子结构中
            ScAddr _elem = ms_context->CreateNode(ScType::NodeConstStruct);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem1, _elem);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, i);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arccom);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els2);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arc);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els3);
//元组下半部分子结构分解和子结构生成
            for (auto j : _elemtup)
            {
                ScAddr _arcsu;
                ScIterator3Ptr it_3su = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, j);
                while (it_3su->Next())
                {
                    _arcsu = it_3su->Get(1);
                    break;
                }
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, j);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arcsu);
                ScIterator3Ptr it_3su1 = ms_context->Iterator3(ScType::NodeConstRole, ScType::EdgeAccessConstPosPerm, _arcsu);
                while (it_3su1->Next())
                {
                    ScAddr _els1 = it_3su1->Get(0);
                    ScAddr _arcr = it_3su1->Get(1);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els1);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arcr);
                    break;
                }
            }
        }
//寻找和生成细分结构(subdividing)(用户答案)
        vector<ScAddr> _class_tups2 = IteratorUtils::getAllWithType(ms_context.get(), _elems_2, ScType::NodeConstTuple);
        for (auto i : _class_tups2)
        {
            ScAddr _arccom, _els2, _arc, _els3 ;
            ScIterator5Ptr it_5 = ms_context->Iterator5(i, ScType::EdgeDCommonConst,
                                                        ScType::Unknown,
                                                        ScType::EdgeAccessConstPosPerm,
                                                        ScType::NodeConstNoRole);
            if (!(it_5->Next()))
                continue;
            _arccom = it_5->Get(1);
            _els2 = it_5->Get(2);
            _arc = it_5->Get(3);
            _els3 = it_5->Get(4);
            vector<ScAddr> _elemtup = IteratorUtils::getAllWithType(ms_context.get(), i, ScType::Unknown);
            if (_elemtup.empty())
                continue;
//先将找到的元组上半部分加入到生成的子结构中
            ScAddr _elem = ms_context->CreateNode(ScType::NodeConstStruct);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem2, _elem);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, i);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arccom);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els2);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arc);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els3);
//元组下半部分子结构分解和子结构生成
            for (auto j : _elemtup)
            {
                ScAddr _arcsu;
                ScIterator3Ptr it_3su = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, j);
                while (it_3su->Next())
                {
                    _arcsu = it_3su->Get(1);
                    break;
                }
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, j);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arcsu);
                ScIterator3Ptr it_3su1 = ms_context->Iterator3(ScType::NodeConstRole, ScType::EdgeAccessConstPosPerm, _arcsu);
                while (it_3su1->Next())
                {
                    ScAddr _els1 = it_3su1->Get(0);
                    ScAddr _arcr = it_3su1->Get(1);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els1);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arcr);
                    break;
                }
            }
        }
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//关系的元组结构分解和子结构生成(标准答案)
        vector<ScAddr> _class_tupr1 = IteratorUtils::getAllWithType(ms_context.get(), _elems_1, ScType::NodeConstTuple);
        for (auto i : _class_tupr1)
        {
            ScAddr _arcpost, _els1;
            ScIterator3Ptr it_3 = ms_context->Iterator3(ScType::NodeConstNoRole, ScType::EdgeAccessConstPosPerm, i);
            if (!it_3->Next())
                continue;
            _els1 = it_3->Get(0);
            _arcpost = it_3->Get(1);
            vector<ScAddr> _elemtup = IteratorUtils::getAllWithType(ms_context.get(), i, ScType::Unknown);
            if (_elemtup.empty())
                continue;
            ScAddr _elem = ms_context->CreateNode(ScType::NodeConstStruct);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem1, _elem);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, i);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els1);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arcpost);
            for (auto j : _elemtup)
            {
                ScAddr _arc;
                ScIterator3Ptr it_3su = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, j);
                while (it_3su->Next())
                {
                    _arc = it_3su->Get(1);
                    break;
                }
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, j);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arc);
                ScIterator3Ptr it_3su1 = ms_context->Iterator3(ScType::NodeConstRole, ScType::EdgeAccessConstPosPerm, _arc);
                while (it_3su1->Next())
                {
                    ScAddr _els2 = it_3su1->Get(0);
                    ScAddr _arcr = it_3su1->Get(1);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els2);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arcr);
                    break;
                }
            }
        }
//关系的元组结构分解和子结构生成(用户答案)
        vector<ScAddr> _class_tupr2 = IteratorUtils::getAllWithType(ms_context.get(), _elems_2, ScType::NodeConstTuple);
        for (auto i : _class_tupr2)
        {
            ScAddr _arcpost, _els1;
            ScIterator3Ptr it_3 = ms_context->Iterator3(ScType::NodeConstNoRole, ScType::EdgeAccessConstPosPerm, i);
            if (!it_3->Next())
                continue;
            _els1 = it_3->Get(0);
            _arcpost = it_3->Get(1);
            vector<ScAddr> _elemtup = IteratorUtils::getAllWithType(ms_context.get(), i, ScType::Unknown);
            if (_elemtup.empty())
                continue;
            ScAddr _elem = ms_context->CreateNode(ScType::NodeConstStruct);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem2, _elem);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, i);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els1);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arcpost);
            for (auto j : _elemtup)
            {
                ScAddr _arc;
                ScIterator3Ptr it_3su = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, j);
                while (it_3su->Next())
                {
                    _arc = it_3su->Get(1);
                    break;
                }
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, j);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arc);
                ScIterator3Ptr it_3su1 = ms_context->Iterator3(ScType::NodeConstRole, ScType::EdgeAccessConstPosPerm, _arc);
                while (it_3su1->Next())
                {
                    ScAddr _els2 = it_3su1->Get(0);
                    ScAddr _arcr = it_3su1->Get(1);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els2);
                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arcr);
                    break;
                }
            }
        }
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//寻找通用边结构(标准答案)
        vector<ScAddr> _class_edge1 = IteratorUtils::getAllWithType(ms_context.get(), _elems_1, ScType::EdgeUCommonConst);
        for (auto i : _class_edge1)
        {
            ScAddr _els1 = ms_context->GetEdgeSource(i);
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
                ScAddr _arcr = it_3->Get(1);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els3);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arcr);
                break;
            }
        }
//寻找通用边结构(用户答案)
        vector<ScAddr> _class_edge2 = IteratorUtils::getAllWithType(ms_context.get(), _elems_2, ScType::EdgeUCommonConst);
        for (auto i : _class_edge2)
        {
            ScAddr _els1 = ms_context->GetEdgeSource(i);
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
                ScAddr _arcr = it_3->Get(1);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _els3);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, _elem, _arcr);
                break;
            }
        }


//        display::printEl(ms_context.get(), i);
//        std::cout<<""<<std::endl;


            AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);

            return SC_RESULT_OK;
    }
}
