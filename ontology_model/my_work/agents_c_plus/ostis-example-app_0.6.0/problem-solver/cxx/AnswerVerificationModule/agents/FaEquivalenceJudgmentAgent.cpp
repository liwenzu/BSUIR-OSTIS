//
// Created by root on 6/11/21.
//
#include <iostream>
#include <sc-memory/sc_stream.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <algorithm>
#include "utils/IteratorUtilsLocal.hpp"
#include "utils/Display.hpp"
#include "keynodes/genKeynodes.hpp"
#include "FaEquivalenceJudgmentAgent.hpp"
#include "utils/Faprocrss.hpp"


using namespace std;
using namespace utils;

namespace answerVerificationModule {

    void FaEquivalenceJudgmentAgent::searchStructure(ScMemoryContext *ctx, ScAddr &struct1, ScAddr &struct2, const ScAddr & elems1, const ScAddr & elems2) {
        vector<ScAddr> eqConcept = IteratorUtils::getAllWithType(ctx, Keynodes::equivalent_concept_template, ScType::Unknown);
        vector<ScAddr> stEqConcept = IteratorUtils::getAllWithType(ctx, elems1, ScType::NodeConstNoRole);
        vector<ScAddr> usEqConcept = IteratorUtils::getAllWithType(ctx, elems2, ScType::NodeConstNoRole);
        vector<ScAddr> resultIntersection1;
        vector<ScAddr> resultIntersection2;
        sort(eqConcept.begin(), eqConcept.end(), ScAddLessFunc());
        sort(stEqConcept.begin(), stEqConcept.end(), ScAddLessFunc());
        sort(usEqConcept.begin(), usEqConcept.end(), ScAddLessFunc());
        set_intersection(eqConcept.begin(), eqConcept.end(), stEqConcept.begin(), stEqConcept.end(), back_inserter(resultIntersection1), ScAddLessFunc());
        set_intersection(eqConcept.begin(), eqConcept.end(), usEqConcept.begin(), usEqConcept.end(), back_inserter(resultIntersection2), ScAddLessFunc());
        if (resultIntersection1.size() != 1 && resultIntersection2.size() != 1)
            return;
        ScAddr eqElement1 = resultIntersection1.front();
        ScAddr eqElement2 = resultIntersection2.front();
        struct1 = IteratorUtilsLocal::getFirstByInRelationWithType(ctx, eqElement1, GenKeynodes::rrel_key_sc_element, ScType::NodeConstStruct);
        struct2 = IteratorUtilsLocal::getFirstByInRelationWithType(ctx, eqElement2, GenKeynodes::rrel_key_sc_element, ScType::NodeConstStruct);
    }

    void FaEquivalenceJudgmentAgent::findAllElements(ScMemoryContext *ctx, const ScAddr &elems1, const ScAddr &elems2, const ScAddr &struct1,
                                                     const ScAddr &struct2, std::vector<ScAddr> & allElemCp1, std::vector<ScAddr> & allElemCp2) {
        ScTemplate initStructTemplate, resultStructTemplate;
        if (ms_context->HelperBuildTemplate(initStructTemplate, struct1) && ms_context->HelperBuildTemplate(resultStructTemplate, struct2)) {
            ScTemplateSearchResult initSearchResult, resultSearchResult;
            if (ms_context->HelperSearchTemplate(initStructTemplate, initSearchResult) && ms_context->HelperSearchTemplate(resultStructTemplate, resultSearchResult)) {
                for (int i=0; i<initSearchResult.Size(); i++) {
                    ScTemplateSearchResultItem InitsearchResultItem = initSearchResult[i];
                    bool flag = true;
                    for(int j=0; j<InitsearchResultItem.Size();j++) {
                        ScAddr elem = InitsearchResultItem[j];
                        if (!ms_context->HelperCheckEdge(elems1, elem, ScType::EdgeAccessConstPosPerm)) {
                            flag = false;
                            break;
                        }
                    }
                    if (flag) {
                        for(int j=0; j<InitsearchResultItem.Size();j++)
                            allElemCp1.push_back(InitsearchResultItem[j]);
                        break;
                    }
                }
                for (int i=0; i<resultSearchResult.Size(); i++) {
                    ScTemplateSearchResultItem ResultsearchResultItem = resultSearchResult[i];
                    bool flag = true;
                    for(int j=0; j<ResultsearchResultItem.Size();j++) {
                        ScAddr elem = ResultsearchResultItem[j];
                        if (!ms_context->HelperCheckEdge(elems2, elem, ScType::EdgeAccessConstPosPerm)) {
                            flag = false;
                            break;
                        }
                    }
                    if (flag) {
                        for(int j=0; j<ResultsearchResultItem.Size();j++)
                            allElemCp2.push_back(ResultsearchResultItem[j]);
                        break;
                    }
                }
            }
        }
    }

    void FaEquivalenceJudgmentAgent::delArc(ScMemoryContext *ctx, const ScAddr &elem,
            std::unordered_set<ScAddr, ScAddrHashFunc< uint32_t >> & setAllElem, std::vector<ScAddr> &delSubstruct) {
        vector<ScAddr> allSubStruct = IteratorUtils::getAllWithType(ms_context.get(), elem, ScType::NodeConstStruct);
        for (auto elemStruct : allSubStruct) {
            vector<ScAddr> allElement = IteratorUtils::getAllWithType(ms_context.get(), elemStruct, ScType::Unknown);
            bool flag = true;
            for (auto element : allElement) {
                if (!setAllElem.count(element)) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                delSubstruct.push_back(elemStruct);
                ScAddr arc = IteratorUtilsLocal::getArc(ms_context.get(), elem, elemStruct);
                ms_context->EraseElement(arc);
            }
        }
    }

    SC_AGENT_IMPLEMENTATION(FaEquivalenceJudgmentAgent) {
        if (!edgeAddr.IsValid())
            return SC_RESULT_ERROR;
        ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
        ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(),questionNode);
        if (!param.IsValid())
            return SC_RESULT_ERROR_INVALID_PARAMS;
        ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, param);
        ScAddr elems_1 = IteratorUtils::getFirstByOutRelation(ms_context.get(), param, Keynodes::nrel_correct_answer);
        ScAddr elems_2 = IteratorUtils::getFirstByOutRelation(ms_context.get(), param, Keynodes::nrel_user_answer);
        ScAddr elem1 = IteratorUtils::getFirstByInRelation(ms_context.get(), elems_1, Keynodes::nrel_relation_decomposition);
        ScAddr elem2 = IteratorUtils::getFirstByInRelation(ms_context.get(), elems_2, Keynodes::nrel_relation_decomposition);
        if (!elems_1.IsValid() || !elems_2.IsValid() || !elem1.IsValid() || !elem2.IsValid()){
            AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
            return SC_RESULT_ERROR_INVALID_PARAMS;
        }
        ScAddr initStruct, resultStruct;
        searchStructure(ms_context.get(), initStruct, resultStruct, elems_1, elems_2);
        if(!initStruct.IsValid() || !resultStruct.IsValid() || !ms_context->HelperCheckEdge(initStruct, resultStruct, ScType::EdgeUCommonConst)) {
            AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
            return SC_RESULT_ERROR_INVALID_PARAMS;
        }
        vector<ScAddr> allElem1, allElem2;
        findAllElements(ms_context.get(), elems_1, elems_2, initStruct, resultStruct, allElem1, allElem2);
        unordered_set<ScAddr, ScAddrHashFunc< uint32_t >> setAllElem1(allElem1.begin(), allElem1.end());
        unordered_set<ScAddr, ScAddrHashFunc< uint32_t >> setAllElem2(allElem2.begin(), allElem2.end());
        if (setAllElem1.empty() || setAllElem2.empty()) {
            AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
            return SC_RESULT_ERROR_INVALID_PARAMS;;
        }
        vector<ScAddr> delSubstruct1, delSubstruct2;
        delArc(ms_context.get(), elem1, setAllElem1, delSubstruct1);
        delArc(ms_context.get(), elem2, setAllElem2, delSubstruct2);
        if (delSubstruct1.empty() || delSubstruct2.empty()) {
            AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
            return SC_RESULT_ERROR_INVALID_PARAMS;;
        }
        int sumMa = 0;
        vector<ScAddr> allSubStruct1 = IteratorUtils::getAllWithType(ms_context.get(), elem1, ScType::NodeConstStruct);
        vector<ScAddr> allSubStruct2 = IteratorUtils::getAllWithType(ms_context.get(), elem2, ScType::NodeConstStruct);
        int sumSta = allSubStruct1.size();
        int sumCand = allSubStruct2.size();
        vector<ScAddr> classtup1, classtup2, classretup1, classretup2, classcomm1,
                classcomm2, classedge1, classedge2, classpost51, classpost52,
                classpost31, classpost32, mathstru, mismathstru, classcomm71,
                classcomm72, classedge71, classedge72;
        Faprocess::SubstructureClassification(ms_context.get(), allSubStruct1, classedge1, classtup1,
                                               classcomm1, classretup1, classpost51, classpost31, classcomm71, classedge71);
        Faprocess::SubstructureClassification(ms_context.get(), allSubStruct2, classedge2, classtup2,
                                               classcomm2, classretup2, classpost52, classpost32,  classcomm72,  classedge72);
        Faprocess::CommonSimilarityCalculation(ms_context.get(), classcomm1, classcomm2, sumMa, mathstru);
        Faprocess::Post5SimilarityCalculation(ms_context.get(), classpost51, classpost52, sumMa, mathstru);
        Faprocess::Post3SimilarityCalculation(ms_context.get(), classpost31, classpost32, sumMa, mathstru);
        Faprocess::TupSimilarityCalculation(ms_context.get(), classtup1, classtup2, sumMa, mathstru);
        Faprocess::ReTupSimilarityCalculation(ms_context.get(), classretup1, classretup2, sumMa, mathstru);
        Faprocess::EdgeSimilarityCalculation(ms_context.get(), classedge1, classedge2, sumMa, mathstru);
        Faprocess::Common7SimilarityCalculation(ms_context.get(), classcomm71, classcomm72, sumMa, mathstru);
        Faprocess::Edge7SimilarityCalculation(ms_context.get(), classedge71, classedge72, sumMa, mathstru);
        cout <<"Number of substructures of standard answers: "<< sumSta << endl;
        cout <<"Number of substructures of user answers: "<< sumCand << endl;
        cout <<"Number of substructures of matching answers: "<< sumMa << endl;
        ScAddr delSim = IteratorUtils::getFirstByOutRelation(ms_context.get(), param, Keynodes::nrel_coefficient);
        if (delSim.IsValid())
            ms_context->EraseElement(delSim);
        ScAddr incoAnswer = IteratorUtils::getFirstByOutRelation(ms_context.get(), param, Keynodes::nrel_incorrect_answer);
        if (incoAnswer.IsValid())
            ms_context->EraseElement(incoAnswer);
        for (auto elem : delSubstruct1)
            ms_context->EraseElement(elem);
        for (auto elem : delSubstruct2)
            ms_context->EraseElement(elem);
        float Fsc = Faprocess::SimilarityCalculation(sumMa, sumCand, sumSta);
        Faprocess::MatchStructureStatistics(ms_context.get(), mathstru, allSubStruct2, mismathstru, answer, param);
        Faprocess::GenerateSimilarityLink(ms_context.get(), answer, param, Fsc);
        AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
        return SC_RESULT_OK;
    }
}
