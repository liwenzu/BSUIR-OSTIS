//
// Created by root on 12/6/20.
//
//
// Created by root on 11/22/20.
//
#include <iostream>
#include "Faprocrss.hpp"
#include <sc-kpm/sc-agents-common/utils/IteratorUtils.hpp>
#include "keynodes/keynodes.hpp"

using namespace utils;

namespace facknowsimcalcu
{
    void Faprocess::generateTuple(ScMemoryContext * ms_context,
                                    const ScAddr & answer,
                                    const ScAddr & param,
                                    const ScAddr & key_node,
                                    ScAddr & mid_elems,
                                    ScAddr & mid_elem
                                    )
    {
        //为了在窗口显示
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, key_node);
        ScIterator5Ptr iterator5 = IteratorUtils::getIterator5(ms_context, param, key_node,true);
        while(iterator5->Next())
        {
            mid_elems = iterator5->Get(2);
            mid_elem = ms_context->CreateNode(ScType::NodeConstTuple);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,mid_elems);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,iterator5->Get(1));
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,iterator5->Get(3));
            ScAddr _arc1 = ms_context->CreateEdge(ScType::EdgeDCommonConst, mid_elem,mid_elems);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,_arc1);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,mid_elem);
            ScAddr _arc2 = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,Keynodes::nrel_relation_decomposition,_arc1);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,_arc2);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm,answer,Keynodes::nrel_relation_decomposition);
        }

    }
//首先找出结构中的所有通用弧
     void Faprocess::decompositionCommon(ScMemoryContext * ms_context,
                                            ScAddr & mid_elems,
                                            ScAddr & mid_elem)
     {
         vector<ScAddr> _class_comm1 = IteratorUtils::getAllWithType(ms_context, mid_elems, ScType::EdgeDCommonConst);
         for (auto i : _class_comm1)
         {
             //判断通用弧的另一端是元组吗&&&&&&&&&&&&&&&
             ScAddr _els1 = ms_context->GetEdgeSource(i);
             if(ms_context->GetElementType(_els1) == ScType::NodeConstTuple)
                 continue;
             ScAddr _els2 = ms_context->GetEdgeTarget(i);
             ScAddr _elem = ms_context->CreateNode(ScType::NodeConstStruct);
             ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, mid_elem, _elem);
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
     }
//找出结构中的所有归属弧(包括带角色关系)(标准答案)
    void Faprocess::decompositionAccwithrole(ScMemoryContext * ms_context,
                                        ScAddr & mid_elems,
                                        ScAddr & mid_elem)
    {
        vector<ScAddr> _class_arc1 = IteratorUtils::getAllWithType(ms_context, mid_elems, ScType::EdgeAccessConstPosPerm);
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
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, mid_elem, _elem);
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
    }

    void Faprocess::decompositionTupleWithSub(ScMemoryContext * ms_context,
                                             ScAddr & mid_elems,
                                             ScAddr & mid_elem)
    {
        vector<ScAddr> _class_tups1 = IteratorUtils::getAllWithType(ms_context, mid_elems, ScType::NodeConstTuple);
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
            vector<ScAddr> _elemtup = IteratorUtils::getAllWithType(ms_context, i, ScType::Unknown);
            if (_elemtup.empty())
                continue;
            //先将找到的元组上半部分加入到生成的子结构中
            ScAddr _elem = ms_context->CreateNode(ScType::NodeConstStruct);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, mid_elem, _elem);
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
    }

    void Faprocess::decompositionTupleWithRelation(ScMemoryContext * ms_context,
                                              ScAddr & mid_elems,
                                              ScAddr & mid_elem)
    {
        vector<ScAddr> _class_tupr1 = IteratorUtils::getAllWithType(ms_context, mid_elems, ScType::NodeConstTuple);
        for (auto i : _class_tupr1)
        {
            ScAddr _arcpost, _els1;
            ScIterator3Ptr it_3 = ms_context->Iterator3(ScType::NodeConstNoRole, ScType::EdgeAccessConstPosPerm, i);
            if (!it_3->Next())
                continue;
            _els1 = it_3->Get(0);
            _arcpost = it_3->Get(1);
            vector<ScAddr> _elemtup = IteratorUtils::getAllWithType(ms_context, i, ScType::Unknown);
            if (_elemtup.empty())
                continue;
            ScAddr _elem = ms_context->CreateNode(ScType::NodeConstStruct);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, mid_elem, _elem);
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
    }

    void Faprocess::decompositionEdge(ScMemoryContext * ms_context,
                                                   ScAddr & mid_elems,
                                                   ScAddr & mid_elem)
    {
        vector<ScAddr> _class_edge1 = IteratorUtils::getAllWithType(ms_context, mid_elems, ScType::EdgeUCommonConst);
        for (auto i : _class_edge1)
        {
            ScAddr _els1 = ms_context->GetEdgeSource(i);
            ScAddr _els2 = ms_context->GetEdgeTarget(i);
            ScAddr _elem = ms_context->CreateNode(ScType::NodeConstStruct);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, mid_elem, _elem);
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
    }



}







