/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/
#include <iostream>
#include <algorithm>

#include <sc-memory/cpp/sc_stream.hpp>
#include <sc-kpm/sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-kpm/sc-agents-common/utils/AgentUtils.hpp>
#include <sc-kpm/sc-agents-common/utils/display.hpp>
#include <sc-kpm/sc-agents-common/utils/CommonUtils.hpp>

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
            int _summa = 0, _sumsta = 0, _sumcand = 0;
            vector<ScAddr> _allsst1 = IteratorUtils::getAllWithType(ms_context.get(), _elem1, ScType::NodeConstStruct);
            _sumsta = _allsst1.size();
            vector<ScAddr> _allsst2 = IteratorUtils::getAllWithType(ms_context.get(), _elem2, ScType::NodeConstStruct);
            _sumcand = _allsst2.size();

            vector<ScAddr> _classtup1, _classtup2, _classretup1, _classretup2,
                           _classcomm1, _classcomm2, _classedge1, _classedge2,
                           _classpost51, _classpost52, _classpost31, _classpost32,
                           _mathstru, _mismathstru;


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
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//通用弧相似度开始计算
        for (auto elem : _classcomm1)
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
            for (auto elemcp : _classcomm2)
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
                        ScAddr elemid1 = IteratorUtils::getFirstByOutRelation(ms_context.get(), _els1, Keynodes::nrel_system_identifier);
                        ScAddr elemid2 = IteratorUtils::getFirstByOutRelation(ms_context.get(), _els12, Keynodes::nrel_system_identifier);
                        if (elemid1.IsValid() || elemid2.IsValid())
                            continue;
                    }
                    else if (ms_context->GetElementType(_els1).IsLink()
                        && ms_context->GetElementType(_els12).IsLink())
                    {
                        std::string data1 = CommonUtils::readString(ms_context.get(), _els1);
                        std::string data2 = CommonUtils::readString(ms_context.get(), _els12);
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
                        ScAddr elemid1 = IteratorUtils::getFirstByOutRelation(ms_context.get(), _els2, Keynodes::nrel_system_identifier);
                        ScAddr elemid2 = IteratorUtils::getFirstByOutRelation(ms_context.get(), _els22, Keynodes::nrel_system_identifier);
                        if (elemid1.IsValid() || elemid2.IsValid())
                            continue;
                    }
                    else if (ms_context->GetElementType(_els2).IsLink()
                             && ms_context->GetElementType(_els22).IsLink())
                    {
                        std::string data1 = CommonUtils::readString(ms_context.get(), _els2);
                        std::string data2 = CommonUtils::readString(ms_context.get(), _els22);
                        if (data1 != data2)
                            continue;
                    }
                    else
                        continue;
                }
                if (_els3 != _els32)
                    continue;
                _summa++;
                _mathstru.push_back(elemcp);
                _classcomm2.erase(_classcomm2.begin()+il-1);
                break;
            }
        }
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//归属弧五元结构相似度计算
        for (auto elem : _classpost51)
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
            for (auto elemcp : _classpost52)
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
                        ScAddr elemid1 = IteratorUtils::getFirstByOutRelation(ms_context.get(), _els1, Keynodes::nrel_system_identifier);
                        ScAddr elemid2 = IteratorUtils::getFirstByOutRelation(ms_context.get(), _els12, Keynodes::nrel_system_identifier);
                        if (elemid1.IsValid() || elemid2.IsValid())
                            continue;
                    }
                    else if (ms_context->GetElementType(_els1).IsLink()
                             && ms_context->GetElementType(_els12).IsLink())
                    {
                        std::string data1 = CommonUtils::readString(ms_context.get(), _els1);
                        std::string data2 = CommonUtils::readString(ms_context.get(), _els12);
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
                        ScAddr elemid1 = IteratorUtils::getFirstByOutRelation(ms_context.get(), _els2, Keynodes::nrel_system_identifier);
                        ScAddr elemid2 = IteratorUtils::getFirstByOutRelation(ms_context.get(), _els22, Keynodes::nrel_system_identifier);
                        if (elemid1.IsValid() || elemid2.IsValid())
                            continue;
                    }
                    else if (ms_context->GetElementType(_els2).IsLink()
                             && ms_context->GetElementType(_els22).IsLink())
                    {
                        std::string data1 = CommonUtils::readString(ms_context.get(), _els2);
                        std::string data2 = CommonUtils::readString(ms_context.get(), _els22);
                        if (data1 != data2)
                            continue;
                    }
                    else
                        continue;
                }
                if (_els3 != _els32)
                    continue;
                _summa++;
                _mathstru.push_back(elemcp);
                _classpost52.erase(_classpost52.begin()+il-1);
                break;
            }
        }
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//归属弧三元结构相似度计算
        for (auto elem : _classpost31)
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
            for (auto elemcp : _classpost32)
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
                        ScAddr elemid1 = IteratorUtils::getFirstByOutRelation(ms_context.get(), _els1, Keynodes::nrel_system_identifier);
                        ScAddr elemid2 = IteratorUtils::getFirstByOutRelation(ms_context.get(), _els12, Keynodes::nrel_system_identifier);
                        if (elemid1.IsValid() || elemid2.IsValid())
                            continue;
                    }
                    else if (ms_context->GetElementType(_els1).IsLink()
                             && ms_context->GetElementType(_els12).IsLink())
                    {
                        std::string data1 = CommonUtils::readString(ms_context.get(), _els1);
                        std::string data2 = CommonUtils::readString(ms_context.get(), _els12);
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
                        ScAddr elemid1 = IteratorUtils::getFirstByOutRelation(ms_context.get(), _els2, Keynodes::nrel_system_identifier);
                        ScAddr elemid2 = IteratorUtils::getFirstByOutRelation(ms_context.get(), _els22, Keynodes::nrel_system_identifier);
                        if (elemid1.IsValid() || elemid2.IsValid())
                            continue;
                    }
                    else if (ms_context->GetElementType(_els2).IsLink()
                             && ms_context->GetElementType(_els22).IsLink())
                    {
                        std::string data1 = CommonUtils::readString(ms_context.get(), _els2);
                        std::string data2 = CommonUtils::readString(ms_context.get(), _els22);
                        if (data1 != data2)
                            continue;
                    }
                    else
                        continue;
                }
                _summa++;
                _mathstru.push_back(elemcp);
                _classpost32.erase(_classpost32.begin()+il-1);
                break;
            }
        }
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//细分结构相似度计算，元组结构相似度计算
        for (auto elem : _classtup1)
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
            for (auto elemcp : _classtup2)
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
                        ScAddr elemid1 = IteratorUtils::getFirstByOutRelation(ms_context.get(), _els1, Keynodes::nrel_system_identifier);
                        ScAddr elemid2 = IteratorUtils::getFirstByOutRelation(ms_context.get(), _els12, Keynodes::nrel_system_identifier);
                        if (elemid1.IsValid() || elemid2.IsValid())
                            continue;
                    }
                    else if (ms_context->GetElementType(_els1).IsLink()
                             && ms_context->GetElementType(_els12).IsLink())
                    {
                        std::string data1 = CommonUtils::readString(ms_context.get(), _els1);
                        std::string data2 = CommonUtils::readString(ms_context.get(), _els12);
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
                _elstup1cp = IteratorUtils::getAllWithType(ms_context.get(), _elstup1, ScType::Unknown);
                _elstup12cp = IteratorUtils::getAllWithType(ms_context.get(), _elstup12, ScType::Unknown);
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
                                _elstup12cplink = IteratorUtils::getAllWithType(ms_context.get(), _elstup12, ScType::Link);
                                for (auto elemlink : _elstup12cplink)
                                {
                                    std::string data1 = CommonUtils::readString(ms_context.get(), currelem);
                                    std::string data2 = CommonUtils::readString(ms_context.get(), elemlink);
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
                                _elstup12cplink = IteratorUtils::getAllWithType(ms_context.get(), _elstup12, ScType::Link);
                                for (auto elemlink : _elstup12cplink)
                                {
                                    std::string data1 = CommonUtils::readString(ms_context.get(), currelem);
                                    std::string data2 = CommonUtils::readString(ms_context.get(), elemlink);
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
                _summa++;
                _mathstru.push_back(elemcp);
                _classtup2.erase(_classtup2.begin()+il-1);
                break;
                Step: ;
            }
        }
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//细分结构相似度计算，元组结构相似度计算
        for (auto elem : _classretup1)
        {
            ScAddr _els1, _elstup1;
            vector<ScAddr> res = IteratorUtils::getAllWithType(ms_context.get(), elem, ScType::EdgeAccessConstPosPerm);
            for (auto _comarc : res)
            {
                _elstup1 = ms_context->GetEdgeTarget(_comarc);
                if (!(ms_context->GetElementType(_elstup1) == ScType::NodeConstTuple))
                    continue;
                _els1 = ms_context->GetEdgeSource(_comarc);
            }
            int il=0;
            for (auto elemcp : _classretup2)
            {
                il++;
                ScAddr _els12, _elstup12;
                vector<ScAddr> res = IteratorUtils::getAllWithType(ms_context.get(), elemcp, ScType::EdgeAccessConstPosPerm);
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
                _elstup1cp = IteratorUtils::getAllWithType(ms_context.get(), _elstup1, ScType::Unknown);
                _elstup12cp = IteratorUtils::getAllWithType(ms_context.get(), _elstup12, ScType::Unknown);
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
                                _elstup12cplink = IteratorUtils::getAllWithType(ms_context.get(), _elstup12, ScType::Link);
                                for (auto elemlink : _elstup12cplink)
                                {
                                    std::string data1 = CommonUtils::readString(ms_context.get(), currelem);
                                    std::string data2 = CommonUtils::readString(ms_context.get(), elemlink);
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
                                _elstup12cplink = IteratorUtils::getAllWithType(ms_context.get(), _elstup12, ScType::Link);
                                for (auto elemlink : _elstup12cplink)
                                {
                                    std::string data1 = CommonUtils::readString(ms_context.get(), currelem);
                                    std::string data2 = CommonUtils::readString(ms_context.get(), elemlink);
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
                _summa++;
                _mathstru.push_back(elemcp);
                _classretup2.erase(_classretup2.begin()+il-1);
                break;
                Stepcp: ;
            }
        }
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//边edge相似度开始计算
        for (auto elem : _classedge1)
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
            for (auto elemcp : _classedge2)
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
                            std::string data1 = CommonUtils::readString(ms_context.get(), _els1);
                            std::string data2 = CommonUtils::readString(ms_context.get(), _els12);
                            if (data1 == data2)
                                goto Edge1;
                        }
                        if (!(ms_context->GetElementType(_els22).IsLink()))
                            continue;
                        std::string data1 = CommonUtils::readString(ms_context.get(), _els1);
                        std::string data2 = CommonUtils::readString(ms_context.get(), _els22);
                        if (data1 != data2)
                            continue;
//                        goto Edge2;
 /*
                        if (_els2 != _els12)
                        {
                            if (!(ms_context->GetElementType(_els2).IsLink()))
                                continue;
                            if (!(ms_context->GetElementType(_els12).IsLink()))
                                continue;
                            std::string data1 = CommonUtils::readString(ms_context.get(), _els2);
                            std::string data2 = CommonUtils::readString(ms_context.get(), _els12);
                            if (data1 != data2)
                                continue;
                            goto Edge;
                        }
 */
  //                      goto Edge;
                    }
 //           Edge2: if (_els2 != _els12)
                    if (_els2 != _els12)
                    {
                        if (!(ms_context->GetElementType(_els2).IsLink()))
                            continue;
                        if (!(ms_context->GetElementType(_els12).IsLink()))
                            continue;
                        std::string data1 = CommonUtils::readString(ms_context.get(), _els2);
                        std::string data2 = CommonUtils::readString(ms_context.get(), _els12);
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
                    std::string data1 = CommonUtils::readString(ms_context.get(), _els2);
                    std::string data2 = CommonUtils::readString(ms_context.get(), _els22);
                    if (data1 != data2)
                        continue;
                }
                Edge :
                _summa++;
                _mathstru.push_back(elemcp);
                _classedge2.erase(_classedge2.begin()+il-1);
                break;
            }
        }








/*
            cout <<_classcomm1.size() << endl;
            cout <<_classedge1.size() << endl;
            cout <<_classpost31.size() << endl;
            cout <<_classpost51.size() << endl;
            cout <<_classretup1.size() << endl;
            cout <<_classtup1.size() << endl;

            cout <<"the same arc:";
            display::printEl(ms_context.get(), _comarc);
            cout <<""<< endl;


*/

        cout <<_sumsta << endl;


/*
        cout <<_classcomm2.size() << endl;
        cout <<_classedge2.size() << endl;
        cout <<_classpost32.size() << endl;
        cout <<_classpost52.size() << endl;
        cout <<_classretup2.size() << endl;
        cout <<_classtup2.size() << endl;



            cout <<"user answer:";
            display::printEl(ms_context.get(), _comarc);
            cout <<""<< endl;
*/

        cout <<_sumcand << endl;
        cout <<_summa << endl;

//        cout <<_mathstru.size() << endl;





            AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);

            return SC_RESULT_OK;
    }
}
