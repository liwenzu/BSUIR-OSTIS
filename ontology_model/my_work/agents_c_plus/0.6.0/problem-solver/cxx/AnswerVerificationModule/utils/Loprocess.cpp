//
// Created by root on 1/17/21.
//
#include <iostream>
#include "Loprocess.hpp"
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "keynodes/keynodes.hpp"
#include "Faprocrss.hpp"
#include <sc-agents-common/utils/CommonUtils.hpp>
#include <algorithm>
#include <sc-agents-common/utils/display.hpp>

using namespace utils;

namespace answerVerificationModule {

    void Loprocess::AutomaticallyNumberSheaf(ScMemoryContext * ms_context, const ScAddr & mid_elems, std::vector<pair<ScAddr,int> >& elem_nbtups)
    {
//auto numbering
        ScAddr _elemmed;
        vector<ScAddr> edgenegation;
        vector<ScAddr> nodenegation;
        ScIterator3Ptr it_3 = ms_context->Iterator3(mid_elems, ScType::EdgeAccessConstPosPerm, Keynodes::negation);
        if (it_3->Next())
        {
            vector<ScAddr> _esaall = IteratorUtils::getAllWithType(ms_context, mid_elems, ScType::EdgeAccessConstPosPerm);
            for (auto elem : _esaall)
            {
                if (ms_context->GetElementType(ms_context->GetEdgeSource(elem)) == ScType::NodeConstTuple &&
                    ms_context->GetElementType(ms_context->GetEdgeTarget(elem)) == ScType::NodeConstTuple)
                {
                    if (ms_context->HelperCheckEdge(Keynodes::negation, ms_context->GetEdgeTarget(elem), ScType::EdgeAccessConstPosPerm))
                    {
                        vector<ScAddr> _esaall1 = IteratorUtils::getAllWithType(ms_context, ms_context->GetEdgeSource(elem), ScType::NodeConstTuple);
                        if (_esaall1.size() > 1)
                        {
                            edgenegation.push_back(elem);
                            nodenegation.push_back(ms_context->GetEdgeSource(elem));
                        }
                    }
                }
            }
        }
//find the first node
        vector<ScAddr> _esaall = IteratorUtils::getAllWithType(ms_context, mid_elems, ScType::NodeConstTuple);
        for (auto elem : _esaall)
        {
            ScIterator3Ptr it_3 = ms_context->Iterator3(ScType::NodeConstTuple, ScType::EdgeAccessConstPosPerm, elem);
            if (it_3->Next())
                continue;
            _elemmed = elem;
            break;
        }
        vector<vector<ScAddr>> _classpoint;
        int _elemmed1=0;
//tree
        while (true)
        {
            while (true)
            {
                elem_nbtups.push_back(make_pair(_elemmed, _elemmed1++));
                vector<ScAddr> _elstup = IteratorUtils::getAllWithType(ms_context, _elemmed, ScType::NodeConstTuple);
                if (_elstup.empty())
                    break;
                if (_elstup.size() > 1)
                {
                    ScIterator5Ptr it_5if = IteratorUtils::getIterator5(ms_context, _elemmed, Keynodes::rrel_if, true);
                    ScIterator5Ptr it_5then = IteratorUtils::getIterator5(ms_context, _elemmed, Keynodes::rrel_then, true);
                    if (it_5if->Next() && it_5then->Next())
                    {
                        _elemmed = it_5if->Get(2);
                        int i=0;
                        for (auto currelem : _elstup)
                        {
                            ++i;
                            if (currelem == _elemmed)
                            {
                                _elstup.erase(_elstup.begin()+i-1);
                                break;
                            }
                        }
                        _classpoint.push_back(_elstup);
                    } else
                    {
                        vector<ScAddr>::iterator it;
                        it = find(nodenegation.begin(), nodenegation.end(), _elemmed);
                        if (it != nodenegation.end())
                        {
                            int num = count(nodenegation.begin(), nodenegation.end(), _elemmed);
                            if (num >1 )
                            {
                                //TODO;
                                cout << " The numbers of negation are:" << num <<endl;
                                _elemmed = _elstup.back();
                                _elstup.pop_back();
                                _classpoint.push_back(_elstup);
                            } else
                            {
                                for (auto currelem : edgenegation)
                                {
                                    if (_elemmed == ms_context->GetEdgeSource(currelem))
                                    {
                                        _elemmed = ms_context->GetEdgeTarget(currelem);
                                        break;
                                    }
                                }
                                int i=0;
                                for (auto currelem : _elstup)
                                {
                                    ++i;
                                    if (currelem == _elemmed)
                                    {
                                        _elstup.erase(_elstup.begin()+i-1);
                                        break;
                                    }
                                }
                                _classpoint.push_back(_elstup);
                            }
                        } else
                        {
                            _elemmed = _elstup.back();
                            _elstup.pop_back();
                            _classpoint.push_back(_elstup);
                        }
                    }
                } else
                    _elemmed = _elstup.back();
            }
            if (_classpoint.empty())
                break;
            _elemmed = _classpoint.back().back();
            _classpoint.back().pop_back();
            if (_classpoint.back().empty())
                _classpoint.pop_back();
        }
    }

    void Loprocess::AutomaticallyNumberStructure(ScMemoryContext * ms_context, std::vector<pair<ScAddr,int> >& elem_nbtups, std::vector<pair<ScAddr,int> >& elem_strus)
    {
        int _elemmed1=0;
        for(auto _elemmed : elem_nbtups)
        {
            ScIterator3Ptr it_3 = ms_context->Iterator3(_elemmed.first, ScType::EdgeAccessConstPosPerm, ScType::NodeConstTuple);
            vector<ScAddr> _esaall = IteratorUtils::getAllWithType(ms_context, _elemmed.first, ScType::NodeConstStruct);
            if (!it_3->Next())
            {
                ScIterator5Ptr it_5if = IteratorUtils::getIterator5(ms_context, _elemmed.first, Keynodes::rrel_if, true);
                ScIterator5Ptr it_5then = IteratorUtils::getIterator5(ms_context, _elemmed.first, Keynodes::rrel_then, true);
                if (it_5if->Next() || it_5then->Next())
                {
                    ScAddr elem = it_5if->Get(2);
                    if (elem.IsValid())
                    {
                        elem_strus.push_back(make_pair(elem,++_elemmed1));
                        for (auto elemCp : _esaall)
                        {
                            if (elem == elemCp)
                                continue;
                            elem_strus.push_back(make_pair(elemCp,++_elemmed1));
                        }
                    } else
                    {
                        elem = it_5then->Get(2);
                        for (auto elemCp : _esaall)
                        {
                            if (elem == elemCp)
                                continue;
                            elem_strus.push_back(make_pair(elemCp,++_elemmed1));
                        }
                        elem_strus.push_back(make_pair(elem,++_elemmed1));
                    }
                } else
                {
                    if (ms_context->HelperCheckEdge(Keynodes::nrel_equivalence, _elemmed.first, ScType::EdgeAccessConstPosPerm))
                    {
                        ScAddr elemCp1 = _esaall[0];
                        ScAddr elemCp2 = _esaall[1];
                        vector<ScAddr> elemCp1Cp = IteratorUtils::getAllWithType(ms_context, elemCp1, ScType::EdgeAccessVarPosPerm);
                        vector<ScAddr> elemCp2Cp = IteratorUtils::getAllWithType(ms_context, elemCp2, ScType::EdgeAccessVarPosPerm);
                        vector<ScAddr> elemCp1Cp1 = IteratorUtils::getAllWithType(ms_context, elemCp1, ScType::NodeVar);
                        vector<ScAddr> elemCp2Cp2 = IteratorUtils::getAllWithType(ms_context, elemCp2, ScType::NodeVar);
                        if ((elemCp1Cp.size()+elemCp1Cp1.size()) > elemCp2Cp.size()+elemCp2Cp2.size())
                        {
                            elem_strus.push_back(make_pair(elemCp2,++_elemmed1));
                            elem_strus.push_back(make_pair(elemCp1,++_elemmed1));
                        } else
                        {
                            elem_strus.push_back(make_pair(elemCp1,++_elemmed1));
                            elem_strus.push_back(make_pair(elemCp2,++_elemmed1));
                        }
                    } else
                    {
                        ScIterator5Ptr it_5 = IteratorUtils::getIterator5(ms_context, _elemmed.first, Keynodes::rrel_bind_variables, true);
                        ScAddr elem;
                        if (it_5->Next())
                        {
                            elem = it_5->Get(2);
                            elem_strus.push_back(make_pair(elem,++_elemmed1));
                        }
                        for (auto elemCp : _esaall)
                        {
                            if (elem == elemCp)
                                continue;
                            elem_strus.push_back(make_pair(elemCp,++_elemmed1));
                        }
                    }
                }
            } else
            {
                if (_esaall.empty())
                    continue;
                for (auto elem : _esaall)
                {
                    elem_strus.push_back(make_pair(elem, ++_elemmed1));
                }
            }
        }
    }

    void Loprocess::JudgmentPossibleAnswer(ScMemoryContext * ms_context, const ScAddr & param)
    {
        vector<ScAddr> elems_1p = IteratorUtils::getAllByOutRelation(ms_context, param, Keynodes::nrel_possible_answer);
        ScAddr elemUser = IteratorUtils::getFirstByOutRelation(ms_context, param, Keynodes::nrel_user_answer);
        ScAddr elemStandard = IteratorUtils::getFirstByOutRelation(ms_context, param, Keynodes::nrel_correct_answer);
        ScAddr element = IteratorUtils::getFirstByOutRelation(ms_context, elemUser, Keynodes::nrel_key_sc_element);
        if (!elems_1p.empty() && !element.IsValid() && elemUser.IsValid() && elemStandard.IsValid())
        {
            elems_1p.insert(elems_1p.end(),elemStandard);
            int numberStandard = 0;
            ScAddr possibleNode2 = ms_context->CreateNode(ScType::NodeConst);
            GenerationUtils::generateRelationBetween(ms_context, elemUser, possibleNode2, Keynodes::nrel_key_sc_element);
            vector<ScAddr> elementAll2 = IteratorUtils::getAllWithType(ms_context, elemUser, ScType::NodeConstNoRole);
            vector<ScAddr> allStruct = IteratorUtils::getAllWithType(ms_context, elemUser, ScType::NodeConstStruct);
            for (auto elem : allStruct)
            {
                vector<ScAddr> elementClass = IteratorUtils::getAllWithType(ms_context, elem, ScType::NodeConstClass);
                if (elementClass.empty())
                    continue;
                elementAll2.insert(elementAll2.end(), elementClass.begin(), elementClass.end());
            }
            for (auto elem : elementAll2)
            {
                if (ms_context->HelperCheckEdge(possibleNode2, elem, ScType::EdgeAccessConstPosPerm))
                    continue;
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, possibleNode2, elem);
                ++numberStandard;
            }
            map<float, ScAddr> simMp;
            for (auto elem : elems_1p)
            {
                int numberCandidate = 0, numberMatched = 0;
                ScAddr possibleNode1 = IteratorUtils::getFirstByOutRelation(ms_context, elem, Keynodes::nrel_key_sc_element);
                vector<ScAddr> elementAll1 = IteratorUtils::getAllWithType(ms_context, possibleNode1, ScType::Unknown);
                for (auto elemCp : elementAll1)
                {
                    ++numberCandidate;
                    if (ms_context->HelperCheckEdge(possibleNode2, elemCp, ScType::EdgeAccessConstPosPerm))
                        ++numberMatched;
                }
                float sim = Faprocess::SimilarityCalculation(numberMatched, numberCandidate, numberStandard);
                simMp.insert(make_pair(sim, elem));
            }
            auto elemBig = simMp.end();
            --elemBig;
            ScIterator5Ptr it_5 = ms_context->Iterator5(param, ScType::EdgeDCommonConst, elemBig->second, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_correct_answer);
            if (!it_5->Next())
            {
                ScIterator5Ptr it_51 = ms_context->Iterator5(param, ScType::EdgeDCommonConst, ScType::Unknown, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_correct_answer);
                while (it_51->Next())
                {
                    ScAddr arc = it_51->Get(1);
                    ScAddr elemCp = it_51->Get(2);
                    ms_context->EraseElement(arc);
                    GenerationUtils::generateRelationBetween(ms_context, param, elemCp, Keynodes::nrel_possible_answer);
                }
                it_51 = ms_context->Iterator5(param, ScType::EdgeDCommonConst, elemBig->second, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_possible_answer);
                while (it_51->Next())
                {
                    ScAddr arc = it_51->Get(1);
                    ms_context->EraseElement(arc);
                    GenerationUtils::generateRelationBetween(ms_context, param, elemBig->second, Keynodes::nrel_correct_answer);
                }
            }
        }
    }

    void Loprocess::SubstructureDecomposition(ScMemoryContext * ms_context, const std::vector<pair<ScAddr,int> >& elem_nbtups, ScAddr & mid_elem)
    {
        for (auto elsTup : elem_nbtups)
        {
            ScAddr genStruct = ms_context->CreateNode(ScType::NodeConstStruct);
            ScIterator3Ptr it_3 = ms_context->Iterator3(ScType::NodeConstNoRole, ScType::EdgeAccessConstPosPerm, elsTup.first);
            while (it_3->Next())
            {
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, mid_elem, genStruct);
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct, it_3->Get(0));
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct, it_3->Get(1));
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct, elsTup.first);
            }
            vector<ScAddr> elsTupCp = IteratorUtils::getAllWithType(ms_context, elsTup.first, ScType::Unknown);
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
                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, mid_elem, genStruct1);
                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, elemStruct);
                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(0));
                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                    }
                    it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, ScType::EdgeAccessVarPosPerm);
                    if (it_31->Next())
                    {
                        // find the structure of common
                        vector<ScAddr> commArc = IteratorUtils::getAllWithType(ms_context, elemStruct, ScType::EdgeDCommonVar);
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
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, mid_elem, genStruct1);
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
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, mid_elem, genStruct1);
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
//find the structure of arc
                        commArc = IteratorUtils::getAllWithType(ms_context, elemStruct, ScType::EdgeAccessVarPosPerm);
                        for (auto elem : commArc)
                        {
                            ScAddr els1, els2;
                            els1 = ms_context->GetEdgeSource(elem);
                            els2 = ms_context->GetEdgeTarget(elem);
                            if (ms_context->GetElementType(els2) == ScType::EdgeDCommonVar || ms_context->GetElementType(els2) == ScType::EdgeUCommonVar ||
                                ms_context->GetElementType(els2) == ScType::EdgeAccessVarPosPerm || ms_context->GetElementType(els1) == ScType::NodeVarTuple)
                                continue;
                            ScAddr genStruct1 = ms_context->CreateNode(ScType::NodeConstStruct);
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, mid_elem, genStruct1);
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, elemStruct);
                            it_3 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els1);
                            while (it_3->Next())
                            {
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els1);
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_3->Get(1));
                            }
                            it_3 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els2);
                            while (it_3->Next())
                            {
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els2);
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_3->Get(1));
                            }
                            it_3 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, elem);
                            while (it_3->Next())
                            {
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, elem);
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_3->Get(1));
                            }
                            it_3 = ms_context->Iterator3(ScType::NodeRole, ScType::EdgeAccessVarPosPerm, elem);
                            if (it_3->Next())
                            {
                                it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, it_3->Get(0));
                                while (it_31->Next())
                                {
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_3->Get(0));
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                }
                                it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, it_3->Get(1));
                                while (it_31->Next())
                                {
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_3->Get(1));
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                }
                            }
                        }
//find the structure of edge
                        commArc = IteratorUtils::getAllWithType(ms_context, elemStruct, ScType::EdgeUCommonVar);
                        for (auto elem : commArc)
                        {
                            ScAddr els1, els2;
                            els1 = ms_context->GetEdgeSource(elem);
                            els2 = ms_context->GetEdgeTarget(elem);
                            ScAddr genStruct1 = ms_context->CreateNode(ScType::NodeConstStruct);
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, mid_elem, genStruct1);
                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, elemStruct);
                            it_3 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els1);
                            while (it_3->Next())
                            {
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els1);
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_3->Get(1));
                            }
                            it_3 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, elem);
                            while (it_3->Next())
                            {
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, elem);
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_3->Get(1));
                            }
                            it_3 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els2);
                            while (it_3->Next())
                            {
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els2);
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_3->Get(1));
                            }
                            it_3 = ms_context->Iterator3(ScType::NodeNoRole, ScType::EdgeAccessVarPosPerm, elem);
                            it_31 = ms_context->Iterator3(ScType::NodeVar, ScType::EdgeAccessVarPosPerm, elem);
                            ScIterator5Ptr it_5 = ms_context->Iterator5(elem, ScType::EdgeDCommonVar, ScType::NodeVar, ScType::EdgeAccessVarPosPerm, ScType::NodeNoRole);
                            if (it_3->Next() || it_31->Next())
                            {
                                ScAddr els3 = it_3->Get(0).IsValid()? it_3->Get(0):it_31->Get(0);
                                arc = it_3->Get(1).IsValid()? it_3->Get(1):it_31->Get(1);
                                it_3 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, els3);
                                while (it_3->Next())
                                {
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, els3);
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_3->Get(1));
                                }
                                it_3 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, arc);
                                while (it_3->Next())
                                {
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, arc);
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_3->Get(1));
                                }
                                it_3 = ms_context->Iterator3(ScType::NodeRole, ScType::EdgeAccessVarPosPerm, arc);
                                if (it_3->Next())
                                {
                                    it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, it_3->Get(0));
                                    while (it_31->Next())
                                    {
                                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_3->Get(0));
                                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                    }
                                    it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, it_3->Get(1));
                                    while (it_31->Next())
                                    {
                                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_3->Get(1));
                                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                    }
                                }
                            }
                            else if (it_5->Next())
                            {
                                it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, it_5->Get(1));
                                while (it_31->Next())
                                {
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_5->Get(1));
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                }
                                it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, it_5->Get(2));
                                while (it_31->Next())
                                {
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_5->Get(2));
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                }
                                it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, it_5->Get(3));
                                while (it_31->Next())
                                {
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_5->Get(3));
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                }
                                it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, it_5->Get(4));
                                while (it_31->Next())
                                {
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_5->Get(4));
                                    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                }
                            }
                        }
//find the structure with tuple
                        it_3 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, ScType::NodeVarTuple);
                        if (it_3->Next())
                        {
                            commArc = IteratorUtils::getAllWithType(ms_context, elemStruct, ScType::EdgeDCommonVar);
                            for (auto elem : commArc)
                            {
                                ScAddr els1, els2;
                                els1 = ms_context->GetEdgeSource(elem);
                                els2 = ms_context->GetEdgeTarget(elem);
                                if (ms_context->GetElementType(els1) != ScType::NodeVarTuple)
                                    continue;
                                ScAddr genStruct1 = ms_context->CreateNode(ScType::NodeConstStruct);
                                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, mid_elem, genStruct1);
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
                                it_3 = ms_context->Iterator3(ScType::NodeNoRole, ScType::EdgeAccessVarPosPerm, elem);
                                while (it_3->Next())
                                {
                                    ScAddr els3 = it_3->Get(0);
                                    arc = it_3->Get(1);
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
                                vector<ScAddr> elemStructCp = IteratorUtils::getAllWithTypeVar(ms_context, els1, ScType::Unknown);
                                for (auto elemCp : elemStructCp)
                                {
                                    it_3 = ms_context->Iterator3(els1, ScType::EdgeAccessVarPosPerm, elemCp);
                                    while (it_3->Next())
                                    {
                                        it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, elemCp);
                                        while (it_31->Next())
                                        {
                                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, elemCp);
                                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                        }
                                        it_31 = ms_context->Iterator3(elemStruct, ScType::EdgeAccessConstPosPerm, it_3->Get(1));
                                        while (it_31->Next())
                                        {
                                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_3->Get(1));
                                            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, genStruct1, it_31->Get(1));
                                        }
                                    }
                                }
                            }
                        }
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
    }

    void Loprocess::DetermineScopeQuantifier(ScMemoryContext * ms_context, const std::vector<pair<ScAddr,int> >& elem_strus, std::vector<ScAddr>& elemDeDoArc)
    {
        for (auto elem : elem_strus)
        {
            ScIterator5Ptr it_5 = ms_context->Iterator5(ScType::NodeConstTuple, ScType::EdgeAccessConstPosPerm, elem.first, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_bind_variables);
            if (it_5->Next())
            {
                ScAddr elemMed = it_5->Get(0);
                vector<vector<ScAddr>> classPoint;
                while (true)
                {
                    while (true)
                    {
                        vector<ScAddr> elemStructCp = IteratorUtils::getAllWithType(ms_context, elemMed, ScType::NodeConstStruct);
                        for (auto elemCp : elemStructCp)
                        {
                            ScIterator5Ptr it_51 = ms_context->Iterator5(elemMed, ScType::EdgeAccessConstPosPerm, elemCp, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_bind_variables);
                            if(!it_51->Next())
                            {
                                ScAddr arc = GenerationUtils::generateRelationBetweenReturnArc(ms_context, elemCp, elem.first, Keynodes::nrel_definitional_domain);
                                elemDeDoArc.push_back(arc);
                            }
                        }
                        vector<ScAddr> elsTup = IteratorUtils::getAllWithType(ms_context, elemMed, ScType::NodeConstTuple);
                        if (elsTup.empty())
                            break;
                        elemMed = elsTup.back();
                        elsTup.pop_back();
                        if (elsTup.empty())
                            continue;
                        classPoint.push_back(elsTup);
                    }
                    if (classPoint.empty())
                        break;
                    elemMed = classPoint.back().back();
                    classPoint.back().pop_back();
                    if (classPoint.back().empty())
                        classPoint.pop_back();
                }
            }
        }
    }

    void Loprocess::RemoveUnnecessaryElements(ScMemoryContext * ms_context, std::vector<ScAddr>& elemDeDoArc)
    {
        while (!elemDeDoArc.empty())
        {
            ScAddr elem = elemDeDoArc.back();
            elemDeDoArc.pop_back();
            ms_context->EraseElement(elem);
        }
    }

    void Loprocess::CreateMapping(ScMemoryContext * ms_context, const ScAddr & mid_elems1, const ScAddr & mid_elems2,
                                  const ScAddr & mid_elem1, const ScAddr & mid_elem2, std::vector<ScAddr>& elemMap,
                                  const std::vector<pair<ScAddr,int> >& elem_strus1,
                                  const std::vector<pair<ScAddr,int> >& elem_strus2)
    {
        for (auto elem : elem_strus1)
        {
            ScAddr elemCp1 = elem.first;
            int elemMed1 = elem.second;
            ScIterator5Ptr it_5 = ms_context->Iterator5(ScType::NodeConstTuple, ScType::EdgeAccessConstPosPerm, elemCp1, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_bind_variables);
            if (!it_5->Next())
            {
                if (elemMed1 > elem_strus2.size())
                    continue;
                ScAddr elemCp2 = elem_strus2[elemMed1-1].first;
                vector<ScAddr> elemDeDo1 = IteratorUtils::getAllByOutRelation(ms_context, elemCp1, Keynodes::nrel_definitional_domain);
                vector<ScAddr> elemDeDo2 = IteratorUtils::getAllByOutRelation(ms_context, elemCp2, Keynodes::nrel_definitional_domain);
                vector<ScAddr> elemDeVar1;
                vector<ScAddr> elemDeVar2;
                for (auto elemLo : elemDeDo1)
                {
                    vector<ScAddr> element = IteratorUtils::getAllWithType(ms_context, elemLo, ScType::NodeVar);
                    elemDeVar1.insert(elemDeVar1.end(), element.begin(), element.end());
                }
                for (auto elemLo : elemDeDo2)
                {
                    vector<ScAddr> element = IteratorUtils::getAllWithType(ms_context, elemLo, ScType::NodeVar);
                    elemDeVar2.insert(elemDeVar2.end(), element.begin(), element.end());
                }


//start the 3 step


//                for (auto i : elemDeVar1)
//                    display::printNl(ms_context, i);
//                cout << "" <<endl;
//                cout << "***************************" <<endl;
//                for (auto i : elemDeVar2)
//                    display::printNl(ms_context, i);
//                cout << "" <<endl;
//                cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXX" <<endl;




//                cout << " Create Mapping 1" << ":" << elemMed1 <<endl;
//                cout << " Create Mapping 2" << ":" << elem_strus2[elemMed1-1].second <<endl;
            }


        }
    }

}