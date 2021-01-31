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
    void Loprocess::AutomaticallyNumberSheaf(ScMemoryContext * ms_context,
                                             const ScAddr & mid_elems,
                                             std::vector<pair<ScAddr,int> >& elem_nbtups)
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
    void Loprocess::AutomaticallyNumberStructure(ScMemoryContext * ms_context,
                                                 std::vector<pair<ScAddr,int> >& elem_nbtups,
                                                 std::vector<pair<ScAddr,int> >& elem_strus)
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










}