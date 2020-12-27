//
// Created by root on 12/6/20.
//
//
// Created by root on 11/22/20.
//
#include <iostream>
#include "Faprocrss.hpp"
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "keynodes/keynodes.hpp"
#include <sc-agents-common/utils/CommonUtils.hpp>
#include <algorithm>

using namespace utils;

namespace answerVerificationModule
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

     void Faprocess::SubstructureClassification(
            ScMemoryContext * ms_context,
            std::vector<ScAddr>& allsst,
            std::vector<ScAddr>& classedge,
            std::vector<ScAddr>& classtup,
            std::vector<ScAddr>& classcomm,
            std::vector<ScAddr>& classretup,
            std::vector<ScAddr>& classpost5,
            std::vector<ScAddr>& classpost3)
     {
         for (auto i : allsst)
         {
             ScIterator3Ptr it_edge = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, ScType::EdgeUCommonConst);
             if (it_edge->Next())
             {
                 classedge.push_back(i);
                 continue;
             }
             ScIterator3Ptr it_common = ms_context->Iterator3(i, ScType::EdgeAccessConstPosPerm, ScType::EdgeDCommonConst);
             if (it_common->Next())
             {
                 ScAddr arc = it_common->Get(2);
                 ScAddr els1 = ms_context->GetEdgeSource(arc);
                 if (ms_context->GetElementType(els1) == ScType::NodeConstTuple)
                 {
                     classtup.push_back(i);
                     continue;
                 }
                 classcomm.push_back(i);
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
                     classretup.push_back(i);
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
                     classpost5.push_back(i);
                     continue;
                 }
                 ScAddr els3 = ms_context->GetEdgeSource(els2);
                 if (els1 == els3)
                 {
                     classpost5.push_back(i);
                     continue;
                 }
             }
             classpost3.push_back(i);
         }
     }
    void Faprocess::CommonSimilarityCalculation(
            ScMemoryContext * ms_context,
            std::vector<ScAddr>& classcomm1,
            std::vector<ScAddr>& classcomm2,
            int & summa,
            std::vector<ScAddr>& mathstru)
    {
//通用弧相似度开始计算
        for (auto elem : classcomm1)
        {
            ScAddr _comarc, _els1, _els2, _els3;
            ScIterator3Ptr it_3 = ms_context->Iterator3(elem, ScType::EdgeAccessConstPosPerm, ScType::EdgeDCommonConst);
            while (it_3->Next())
            {
                _comarc = it_3->Get(2);
            }
            _els1 = ms_context->GetEdgeSource(_comarc);
            _els2 = ms_context->GetEdgeTarget(_comarc);
            ScIterator3Ptr it_31 = ms_context->Iterator3(elem, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
            while (it_31->Next())
            {
                _els3 = it_31->Get(2);
            }
            int il = 0;
            for (auto elemcp : classcomm2)
            {
                il++;
                ScAddr _els32;
                ScIterator3Ptr it_3 = ms_context->Iterator3(elemcp, ScType::EdgeAccessConstPosPerm, ScType::EdgeDCommonConst);
                while (it_3->Next())
                {
                    _comarc = it_3->Get(2);
                }
                ScAddr _els12 = ms_context->GetEdgeSource(_comarc);
                ScAddr _els22 = ms_context->GetEdgeTarget(_comarc);
                ScIterator3Ptr it_31 = ms_context->Iterator3(elemcp, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
                while (it_31->Next())
                {
                    _els32 = it_31->Get(2);
                }
                if (_els1 != _els12)
                {
                    if (ms_context->GetElementType(_els1) == ScType::NodeConst
                        && ms_context->GetElementType(_els12) == ScType::NodeConst)
                    {
                        ScAddr elemid1 = IteratorUtils::getFirstByOutRelation(ms_context, _els1, Keynodes::nrel_system_identifier);
                        ScAddr elemid2 = IteratorUtils::getFirstByOutRelation(ms_context, _els12, Keynodes::nrel_system_identifier);
                        if (elemid1.IsValid() || elemid2.IsValid())
                            continue;
                    }
                    else if (ms_context->GetElementType(_els1).IsLink()
                             && ms_context->GetElementType(_els12).IsLink())
                    {
                        std::string data1 = CommonUtils::readString(ms_context, _els1);
                        std::string data2 = CommonUtils::readString(ms_context, _els12);
                        if (data1 != data2)
                            continue;
                    }
                    else
                        continue;
                }
                if (_els2 != _els22)
                {
                    if (ms_context->GetElementType(_els2) == ScType::NodeConst
                        && ms_context->GetElementType(_els22) == ScType::NodeConst)
                    {
                        ScAddr elemid1 = IteratorUtils::getFirstByOutRelation(ms_context, _els2, Keynodes::nrel_system_identifier);
                        ScAddr elemid2 = IteratorUtils::getFirstByOutRelation(ms_context, _els22, Keynodes::nrel_system_identifier);
                        if (elemid1.IsValid() || elemid2.IsValid())
                            continue;
                    }
                    else if (ms_context->GetElementType(_els2).IsLink()
                             && ms_context->GetElementType(_els22).IsLink())
                    {
                        std::string data1 = CommonUtils::readString(ms_context, _els2);
                        std::string data2 = CommonUtils::readString(ms_context, _els22);
                        if (data1 != data2)
                            continue;
                    }
                    else
                        continue;
                }
                if (_els3 != _els32)
                    continue;
                summa++;
                mathstru.push_back(elemcp);
                classcomm2.erase(classcomm2.begin()+il-1);
                break;
            }
        }
    }
    void Faprocess::Post5SimilarityCalculation(
            ScMemoryContext * ms_context,
            std::vector<ScAddr>& classpost51,
            std::vector<ScAddr>& classpost52,
            int & summa,
            std::vector<ScAddr>& mathstru)
    {
        //归属弧五元结构相似度计算
        for (auto elem : classpost51)
        {
            ScAddr _comarc, _els1, _els2, _els3;
            ScIterator3Ptr it_3 = ms_context->Iterator3(elem, ScType::EdgeAccessConstPosPerm, ScType::NodeConstRole);
            while (it_3->Next())
            {
                _els3 = it_3->Get(2);
            }
            ScIterator3Ptr it_31 = ms_context->Iterator3(elem, ScType::EdgeAccessConstPosPerm, ScType::EdgeAccessConstPosPerm);
            while (it_31->Next())
            {
                _comarc = it_31->Get(2);
                _els1 = ms_context->GetEdgeSource(_comarc);
                if (_els1 == _els3)
                    continue;
                _els2 = ms_context->GetEdgeTarget(_comarc);
                break;
            }
            int il =0 ;
            for (auto elemcp : classpost52)
            {
                il++;
                ScAddr _els12, _els22, _els32;
                ScIterator3Ptr it_3 = ms_context->Iterator3(elemcp, ScType::EdgeAccessConstPosPerm, ScType::NodeConstRole);
                while (it_3->Next())
                {
                    _els32 = it_3->Get(2);
                }
                ScIterator3Ptr it_31 = ms_context->Iterator3(elemcp, ScType::EdgeAccessConstPosPerm, ScType::EdgeAccessConstPosPerm);
                while (it_31->Next())
                {
                    _comarc = it_31->Get(2);
                    _els12 = ms_context->GetEdgeSource(_comarc);
                    if (_els12 == _els32)
                        continue;
                    _els22 = ms_context->GetEdgeTarget(_comarc);
                    break;
                }
                if (_els1 != _els12)
                {
                    if (ms_context->GetElementType(_els1) == ScType::NodeConst
                        && ms_context->GetElementType(_els12) == ScType::NodeConst)
                    {
                        ScAddr elemid1 = IteratorUtils::getFirstByOutRelation(ms_context, _els1, Keynodes::nrel_system_identifier);
                        ScAddr elemid2 = IteratorUtils::getFirstByOutRelation(ms_context, _els12, Keynodes::nrel_system_identifier);
                        if (elemid1.IsValid() || elemid2.IsValid())
                            continue;
                    }
                    else if (ms_context->GetElementType(_els1).IsLink()
                             && ms_context->GetElementType(_els12).IsLink())
                    {
                        std::string data1 = CommonUtils::readString(ms_context, _els1);
                        std::string data2 = CommonUtils::readString(ms_context, _els12);
                        if (data1 != data2)
                            continue;
                    }
                    else
                        continue;
                }
                if (_els2 != _els22)
                {
                    if (ms_context->GetElementType(_els2) == ScType::NodeConst
                        && ms_context->GetElementType(_els22) == ScType::NodeConst)
                    {
                        ScAddr elemid1 = IteratorUtils::getFirstByOutRelation(ms_context, _els2, Keynodes::nrel_system_identifier);
                        ScAddr elemid2 = IteratorUtils::getFirstByOutRelation(ms_context, _els22, Keynodes::nrel_system_identifier);
                        if (elemid1.IsValid() || elemid2.IsValid())
                            continue;
                    }
                    else if (ms_context->GetElementType(_els2).IsLink()
                             && ms_context->GetElementType(_els22).IsLink())
                    {
                        std::string data1 = CommonUtils::readString(ms_context, _els2);
                        std::string data2 = CommonUtils::readString(ms_context, _els22);
                        if (data1 != data2)
                            continue;
                    }
                    else
                        continue;
                }
                if (_els3 != _els32)
                    continue;
                summa++;
                mathstru.push_back(elemcp);
                classpost52.erase(classpost52.begin()+il-1);
                break;
            }
        }
    }
     void Faprocess::Post3SimilarityCalculation(
            ScMemoryContext * ms_context,
            std::vector<ScAddr>& classpost31,
            std::vector<ScAddr>& classpost32,
            int & summa,
            std::vector<ScAddr>& mathstru)
     {
         //归属弧三元结构相似度计算
         for (auto elem : classpost31)
         {
             ScAddr _comarc, _els1, _els2;
             ScIterator3Ptr it_3 = ms_context->Iterator3(elem, ScType::EdgeAccessConstPosPerm, ScType::EdgeAccessConstPosPerm);
             while (it_3->Next())
             {
                 _comarc = it_3->Get(2);
             }
             _els1 = ms_context->GetEdgeSource(_comarc);
             _els2 = ms_context->GetEdgeTarget(_comarc);
             int il =0;
             for (auto elemcp : classpost32)
             {
                 il++;
                 ScAddr _els12, _els22;
                 ScIterator3Ptr it_3 = ms_context->Iterator3(elemcp, ScType::EdgeAccessConstPosPerm, ScType::EdgeAccessConstPosPerm);
                 while (it_3->Next())
                 {
                     _comarc = it_3->Get(2);
                 }
                 _els12 = ms_context->GetEdgeSource(_comarc);
                 _els22 = ms_context->GetEdgeTarget(_comarc);
                 if (_els1 != _els12)
                 {
                     if (ms_context->GetElementType(_els1) == ScType::NodeConst
                         && ms_context->GetElementType(_els12) == ScType::NodeConst)
                     {
                         ScAddr elemid1 = IteratorUtils::getFirstByOutRelation(ms_context, _els1, Keynodes::nrel_system_identifier);
                         ScAddr elemid2 = IteratorUtils::getFirstByOutRelation(ms_context, _els12, Keynodes::nrel_system_identifier);
                         if (elemid1.IsValid() || elemid2.IsValid())
                             continue;
                     }
                     else if (ms_context->GetElementType(_els1).IsLink()
                              && ms_context->GetElementType(_els12).IsLink())
                     {
                         std::string data1 = CommonUtils::readString(ms_context, _els1);
                         std::string data2 = CommonUtils::readString(ms_context, _els12);
                         if (data1 != data2)
                             continue;
                     }
                     else
                         continue;
                 }
                 if (_els2 != _els22)
                 {
                     if (ms_context->GetElementType(_els2) == ScType::NodeConst
                         && ms_context->GetElementType(_els22) == ScType::NodeConst)
                     {
                         ScAddr elemid1 = IteratorUtils::getFirstByOutRelation(ms_context, _els2, Keynodes::nrel_system_identifier);
                         ScAddr elemid2 = IteratorUtils::getFirstByOutRelation(ms_context, _els22, Keynodes::nrel_system_identifier);
                         if (elemid1.IsValid() || elemid2.IsValid())
                             continue;
                     }
                     else if (ms_context->GetElementType(_els2).IsLink()
                              && ms_context->GetElementType(_els22).IsLink())
                     {
                         std::string data1 = CommonUtils::readString(ms_context, _els2);
                         std::string data2 = CommonUtils::readString(ms_context, _els22);
                         if (data1 != data2)
                             continue;
                     }
                     else
                         continue;
                 }
                 summa++;
                 mathstru.push_back(elemcp);
                 classpost32.erase(classpost32.begin()+il-1);
                 break;
             }
         }
     }
     void Faprocess::TupSimilarityCalculation(
            ScMemoryContext * ms_context,
            std::vector<ScAddr>& classtup1,
            std::vector<ScAddr>& classtup2,
            int & summa,
            std::vector<ScAddr>& mathstru)
     {
         //细分结构相似度计算，元组结构相似度计算
         for (auto elem : classtup1)
         {
             ScAddr _comarc, _els1, _els3, _elstup1;
             ScIterator3Ptr it_3 = ms_context->Iterator3(elem, ScType::EdgeAccessConstPosPerm, ScType::EdgeDCommonConst);
             while (it_3->Next())
             {
                 _comarc = it_3->Get(2);
             }
             _elstup1 = ms_context->GetEdgeSource(_comarc);
             _els1= ms_context->GetEdgeTarget(_comarc);
             it_3 = ms_context->Iterator3(elem, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
             while (it_3->Next())
             {
                 _els3 = it_3->Get(2);
             }
             int il=0;
             for (auto elemcp : classtup2)
             {
                 il++;
                 ScAddr _els12, _els32, _elstup12;
                 ScIterator3Ptr it_3 = ms_context->Iterator3(elemcp, ScType::EdgeAccessConstPosPerm, ScType::EdgeDCommonConst);
                 while (it_3->Next())
                 {
                     _comarc = it_3->Get(2);
                 }
                 _elstup12 = ms_context->GetEdgeSource(_comarc);
                 _els12= ms_context->GetEdgeTarget(_comarc);
                 it_3 = ms_context->Iterator3(elemcp, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
                 while (it_3->Next())
                 {
                     _els32 = it_3->Get(2);
                 }
                 if (_els1 != _els12)
                 {
                     if (ms_context->GetElementType(_els1) == ScType::NodeConst
                         && ms_context->GetElementType(_els12) == ScType::NodeConst)
                     {
                         ScAddr elemid1 = IteratorUtils::getFirstByOutRelation(ms_context, _els1, Keynodes::nrel_system_identifier);
                         ScAddr elemid2 = IteratorUtils::getFirstByOutRelation(ms_context, _els12, Keynodes::nrel_system_identifier);
                         if (elemid1.IsValid() || elemid2.IsValid())
                             continue;
                     }
                     else if (ms_context->GetElementType(_els1).IsLink()
                              && ms_context->GetElementType(_els12).IsLink())
                     {
                         std::string data1 = CommonUtils::readString(ms_context, _els1);
                         std::string data2 = CommonUtils::readString(ms_context, _els12);
                         if (data1 != data2)
                             continue;
                     }
                     else
                         continue;
                 }
                 if (_els3 != _els32)
                 {
                     continue;
                 }
                 vector<ScAddr> _elstup1cp, _elstup12cp;
                 _elstup1cp = IteratorUtils::getAllWithType(ms_context, _elstup1, ScType::Unknown);
                 _elstup12cp = IteratorUtils::getAllWithType(ms_context, _elstup12, ScType::Unknown);
                 int s1 = _elstup1cp.size(), s2 = _elstup12cp.size();
                 if (s1 != s2)
                     continue;
                 for (auto currelem : _elstup1cp)
                 {
                     ScIterator5Ptr it_5 = ms_context->Iterator5(_elstup1, ScType::EdgeAccessConstPosPerm, currelem,
                                                                 ScType::EdgeAccessConstPosPerm, ScType::NodeConstRole);
                     if(it_5->Next())
                     {
                         ScAddr _els4 = it_5->Get(4);
                         vector<ScAddr>::iterator existence;
                         existence = find(_elstup12cp.begin(), _elstup12cp.end(), currelem);
                         if (existence == _elstup12cp.end())
                         {
                             if (ms_context->GetElementType(currelem).IsLink())
                             {
                                 vector<ScAddr> _elstup12cplink;
                                 _elstup12cplink = IteratorUtils::getAllWithType(ms_context, _elstup12, ScType::Link);
                                 for (auto elemlink : _elstup12cplink)
                                 {
                                     std::string data1 = CommonUtils::readString(ms_context, currelem);
                                     std::string data2 = CommonUtils::readString(ms_context, elemlink);
                                     if (data1 != data2)
                                         continue;
                                     ScIterator5Ptr it_51 = ms_context->Iterator5(_elstup12, ScType::EdgeAccessConstPosPerm, elemlink,
                                                                                  ScType::EdgeAccessConstPosPerm, _els4);
                                     if (!(it_51->Next()))
                                         continue;
                                     goto Step1;
                                 }
                                 goto Step;
                             }
                             else
                                 goto Step;
                         }
                         ScIterator5Ptr it_51 = ms_context->Iterator5(_elstup12, ScType::EdgeAccessConstPosPerm, currelem,
                                                                      ScType::EdgeAccessConstPosPerm, _els4);
                         if (!(it_51->Next()))
                             goto Step;
                     }
                     else
                     {
                         vector<ScAddr>::iterator existence;
                         existence = find(_elstup12cp.begin(), _elstup12cp.end(), currelem);
                         if (existence == _elstup12cp.end())
                         {
                             if (ms_context->GetElementType(currelem).IsLink())
                             {
                                 vector<ScAddr> _elstup12cplink;
                                 _elstup12cplink = IteratorUtils::getAllWithType(ms_context, _elstup12, ScType::Link);
                                 for (auto elemlink : _elstup12cplink)
                                 {
                                     std::string data1 = CommonUtils::readString(ms_context, currelem);
                                     std::string data2 = CommonUtils::readString(ms_context, elemlink);
                                     if (data1 != data2)
                                         continue;
                                     ScIterator5Ptr it_51 = ms_context->Iterator5(_elstup12, ScType::EdgeAccessConstPosPerm, elemlink,
                                                                                  ScType::EdgeAccessConstPosPerm, ScType::NodeConstRole);
                                     if (it_51->Next())
                                         continue;
                                     goto Step1;
                                 }
                                 goto Step;
                             }
                             else
                                 goto Step;
                         }
                         ScIterator5Ptr it_51 = ms_context->Iterator5(_elstup12, ScType::EdgeAccessConstPosPerm, currelem,
                                                                      ScType::EdgeAccessConstPosPerm, ScType::NodeConstRole);
                         if (it_51->Next())
                             goto Step;
                     }
                     Step1: ;
                 }
                 summa++;
                 mathstru.push_back(elemcp);
                 classtup2.erase(classtup2.begin()+il-1);
                 break;
                 Step: ;
             }
         }
     }
     void Faprocess::ReTupSimilarityCalculation(
            ScMemoryContext * ms_context,
            std::vector<ScAddr>& classretup1,
            std::vector<ScAddr>& classretup2,
            int & summa,
            std::vector<ScAddr>& mathstru)
     {
         //细分结构相似度计算，元组结构相似度计算
         for (auto elem : classretup1)
         {
             ScAddr _els1, _elstup1;
             vector<ScAddr> res = IteratorUtils::getAllWithType(ms_context, elem, ScType::EdgeAccessConstPosPerm);
             for (auto _comarc : res)
             {
                 _elstup1 = ms_context->GetEdgeTarget(_comarc);
                 if (!(ms_context->GetElementType(_elstup1) == ScType::NodeConstTuple))
                     continue;
                 _els1 = ms_context->GetEdgeSource(_comarc);
             }
             int il=0;
             for (auto elemcp : classretup2)
             {
                 il++;
                 ScAddr _els12, _elstup12;
                 vector<ScAddr> res = IteratorUtils::getAllWithType(ms_context, elemcp, ScType::EdgeAccessConstPosPerm);
                 for (auto _comarc : res)
                 {
                     _elstup12 = ms_context->GetEdgeTarget(_comarc);
                     if (!(ms_context->GetElementType(_elstup12) == ScType::NodeConstTuple))
                         continue;
                     _els12 = ms_context->GetEdgeSource(_comarc);
                 }
                 if (_els1 != _els12)
                     continue;
                 vector<ScAddr> _elstup1cp, _elstup12cp;
                 _elstup1cp = IteratorUtils::getAllWithType(ms_context, _elstup1, ScType::Unknown);
                 _elstup12cp = IteratorUtils::getAllWithType(ms_context, _elstup12, ScType::Unknown);
                 int s1 = _elstup1cp.size(), s2 = _elstup12cp.size();
                 if (s1 != s2)
                     continue;
                 for (auto currelem : _elstup1cp)
                 {
                     ScIterator5Ptr it_5 = ms_context->Iterator5(_elstup1, ScType::EdgeAccessConstPosPerm, currelem,
                                                                 ScType::EdgeAccessConstPosPerm, ScType::NodeConstRole);
                     if(it_5->Next())
                     {
                         ScAddr _els4 = it_5->Get(4);
                         vector<ScAddr>::iterator existence;
                         existence = find(_elstup12cp.begin(), _elstup12cp.end(), currelem);
                         if (existence == _elstup12cp.end())
                         {
                             if (ms_context->GetElementType(currelem).IsLink())
                             {
                                 vector<ScAddr> _elstup12cplink;
                                 _elstup12cplink = IteratorUtils::getAllWithType(ms_context, _elstup12, ScType::Link);
                                 for (auto elemlink : _elstup12cplink)
                                 {
                                     std::string data1 = CommonUtils::readString(ms_context, currelem);
                                     std::string data2 = CommonUtils::readString(ms_context, elemlink);
                                     if (data1 != data2)
                                         continue;
                                     ScIterator5Ptr it_51 = ms_context->Iterator5(_elstup12, ScType::EdgeAccessConstPosPerm, elemlink,
                                                                                  ScType::EdgeAccessConstPosPerm, _els4);
                                     if (!(it_51->Next()))
                                         continue;
                                     goto Step1cp;
                                 }
                                 goto Stepcp;
                             }
                             else
                                 goto Stepcp;
                         }
                         ScIterator5Ptr it_51 = ms_context->Iterator5(_elstup12, ScType::EdgeAccessConstPosPerm, currelem,
                                                                      ScType::EdgeAccessConstPosPerm, _els4);
                         if (!(it_51->Next()))
                             goto Stepcp;
                     }
                     else
                     {
                         vector<ScAddr>::iterator existence;
                         existence = find(_elstup12cp.begin(), _elstup12cp.end(), currelem);
                         if (existence == _elstup12cp.end())
                         {
                             if (ms_context->GetElementType(currelem).IsLink())
                             {
                                 vector<ScAddr> _elstup12cplink;
                                 _elstup12cplink = IteratorUtils::getAllWithType(ms_context, _elstup12, ScType::Link);
                                 for (auto elemlink : _elstup12cplink)
                                 {
                                     std::string data1 = CommonUtils::readString(ms_context, currelem);
                                     std::string data2 = CommonUtils::readString(ms_context, elemlink);
                                     if (data1 != data2)
                                         continue;
                                     ScIterator5Ptr it_51 = ms_context->Iterator5(_elstup12, ScType::EdgeAccessConstPosPerm, elemlink,
                                                                                  ScType::EdgeAccessConstPosPerm, ScType::NodeConstRole);
                                     if (it_51->Next())
                                         continue;
                                     goto Step1cp;
                                 }
                                 goto Stepcp;
                             }
                             else
                                 goto Stepcp;
                         }
                         ScIterator5Ptr it_51 = ms_context->Iterator5(_elstup12, ScType::EdgeAccessConstPosPerm, currelem,
                                                                      ScType::EdgeAccessConstPosPerm, ScType::NodeConstRole);
                         if (it_51->Next())
                             goto Stepcp;
                     }
                     Step1cp: ;
                 }
                 summa++;
                 mathstru.push_back(elemcp);
                 classretup2.erase(classretup2.begin()+il-1);
                 break;
                 Stepcp: ;
             }
         }
     }
    void Faprocess::EdgeSimilarityCalculation(
            ScMemoryContext * ms_context,
            std::vector<ScAddr>& classedge1,
            std::vector<ScAddr>& classedge2,
            int & summa,
            std::vector<ScAddr>& mathstru)
    {
        //边edge相似度开始计算
        for (auto elem : classedge1)
        {
            ScAddr _comarc, _els1, _els2, _els3;
            ScIterator3Ptr it_3 = ms_context->Iterator3(elem, ScType::EdgeAccessConstPosPerm, ScType::EdgeUCommonConst);
            while (it_3->Next())
            {
                _comarc = it_3->Get(2);
            }
            _els1 = ms_context->GetEdgeSource(_comarc);
            _els2 = ms_context->GetEdgeTarget(_comarc);
            it_3 = ms_context->Iterator3(elem, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
            while (it_3->Next())
            {
                _els3 = it_3->Get(2);
            }
            int il=0;
            for (auto elemcp : classedge2)
            {
                il++;
                ScAddr _els12, _els22, _els32;
                ScIterator3Ptr it_3 = ms_context->Iterator3(elemcp, ScType::EdgeAccessConstPosPerm, ScType::EdgeUCommonConst);
                while (it_3->Next())
                {
                    _comarc = it_3->Get(2);
                }
                _els12 = ms_context->GetEdgeSource(_comarc);
                _els22 = ms_context->GetEdgeTarget(_comarc);
                it_3 = ms_context->Iterator3(elemcp, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
                while (it_3->Next())
                {
                    _els32 = it_3->Get(2);
                }
                if (_els3 != _els32)
                    continue;
                if (_els1 != _els12)
                {
                    if (_els1 != _els22)
                    {
                        if (!(ms_context->GetElementType(_els1).IsLink()))
                            continue;
                        if (ms_context->GetElementType(_els12).IsLink())
                        {
                            std::string data1 = CommonUtils::readString(ms_context, _els1);
                            std::string data2 = CommonUtils::readString(ms_context, _els12);
                            if (data1 == data2)
                                goto Edge1;
                        }
                        if (!(ms_context->GetElementType(_els22).IsLink()))
                            continue;
                        std::string data1 = CommonUtils::readString(ms_context, _els1);
                        std::string data2 = CommonUtils::readString(ms_context, _els22);
                        if (data1 != data2)
                            continue;
                    }
                    if (_els2 != _els12)
                    {
                        if (!(ms_context->GetElementType(_els2).IsLink()))
                            continue;
                        if (!(ms_context->GetElementType(_els12).IsLink()))
                            continue;
                        std::string data1 = CommonUtils::readString(ms_context, _els2);
                        std::string data2 = CommonUtils::readString(ms_context, _els12);
                        if (data1 != data2)
                            continue;
                    }
                    goto Edge;
                }
                Edge1: if (_els2 != _els22)
            {
                if (!(ms_context->GetElementType(_els2).IsLink()))
                    continue;
                if (!(ms_context->GetElementType(_els22).IsLink()))
                    continue;
                std::string data1 = CommonUtils::readString(ms_context, _els2);
                std::string data2 = CommonUtils::readString(ms_context, _els22);
                if (data1 != data2)
                    continue;
            }
                Edge :
                summa++;
                mathstru.push_back(elemcp);
                classedge2.erase(classedge2.begin()+il-1);
                break;
            }
        }
    }
    float Faprocess::SimilarityCalculation(
            int & summa,
            int & sumcand,
            int & sumsta)
    {
        float Psc = 0, Rsc = 0, Fsc = 0;
        Psc = summa*1.0/sumcand;
        Rsc = summa*1.0/sumsta;
        Fsc = (2*Psc*Rsc*1.0)/(Psc+Rsc);
        cout <<"精确度: "<<Psc << endl;
        cout <<"召回率: "<<Rsc << endl;
        cout <<"相似度: "<<Fsc << endl;
        return Fsc;
    }
    void Faprocess::MatchStructureStatistics(
            ScMemoryContext * ms_context,
            std::vector<ScAddr>& mathstru,
            std::vector<ScAddr>& allsst2,
            std::vector<ScAddr>& mismathstru,
            const ScAddr & answer,
            const ScAddr & param)
    {
        //用户答案不匹配结构和匹配结构信息统计
        for (auto i : mathstru)
        {
            int il=0;
            for (auto j : allsst2)
            {
                il++;
                if (i==j)
                {
                    allsst2.erase(allsst2.begin()+il-1);
                    break;
                }
            }
        }
        mismathstru = allsst2;
        if (mismathstru.size() == 0)
            cout <<"用户答案全部匹配:" << endl;
        else if (mathstru.size() == 0)
            cout <<"用户答案全部不匹配:" << endl;
        else
        {
            cout <<"用户答案有匹配部分也有不匹配部分:" << endl;
            ScAddr node = ms_context->CreateNode(ScType::NodeConst);
            for (auto i : mismathstru)
            {
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, node, i);
            }
            ScAddr arc = ms_context->CreateEdge(ScType::EdgeDCommonConst, param, node);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arc);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, node);
            ScAddr arc1 = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::nrel_incorrect_answer, arc);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arc1);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_incorrect_answer);
        }
    }
    void  Faprocess::GenerateSimilarityLink(
            ScMemoryContext * ms_context,
            const ScAddr & answer,
            const ScAddr & param,
            float Fsc)
    {
        ScAddr link = ms_context->CreateLink();
        ScAddr arc = ms_context->CreateEdge(ScType::EdgeDCommonConst, param, link);
        ScAddr arc1 = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::nrel_coefficient, arc);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arc);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, link);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arc1);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_coefficient);
        string str = to_string(Fsc);
        ScStreamPtr stream;
        stream.reset(new ScStream((sc_char*)(&str), sizeof(str), SC_STREAM_FLAG_READ | SC_STREAM_FLAG_SEEK));
        ms_context->SetLinkContent(link, stream);
    }
}







