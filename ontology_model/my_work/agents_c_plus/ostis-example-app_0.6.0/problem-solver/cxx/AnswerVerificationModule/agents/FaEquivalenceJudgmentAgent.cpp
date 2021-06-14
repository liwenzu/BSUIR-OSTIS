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

    SC_AGENT_IMPLEMENTATION(FaEquivalenceJudgmentAgent) {
        if (!edgeAddr.IsValid())
            return SC_RESULT_ERROR;
        ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
        ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(),questionNode);
        if (!param.IsValid())
            return SC_RESULT_ERROR_INVALID_PARAMS;
        ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);

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







        ScTemplate initStructTemplate, resultStructTemplate;
        if (ms_context->HelperBuildTemplate(initStructTemplate, initStruct) && ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct)) {
            ScTemplateSearchResult initSearchResult, resultSearchResult;
            if (ms_context->HelperSearchTemplate(initStructTemplate, initSearchResult) && ms_context->HelperSearchTemplate(resultStructTemplate, resultSearchResult)) {

                cout << "&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
                cout << initSearchResult.Size() << " and " << resultSearchResult.Size() << endl;


                vector<ScAddr> allElem1, allElem2;


                for (int i=0; i<initSearchResult.Size(); i++) {
                    ScTemplateSearchResultItem InitsearchResultItem = initSearchResult[i];
                    bool flag = true;
                    for(int j=0; j<InitsearchResultItem.Size();j++) {
                        ScAddr elem = InitsearchResultItem[j];
                        if (!ms_context->HelperCheckEdge(elems_1, elem, ScType::EdgeAccessConstPosPerm)) {
                            flag = false;
                            break;
                        }
                    }
                    if (flag) {
                        for(int j=0; j<InitsearchResultItem.Size();j++)
                            allElem1.push_back(InitsearchResultItem[j]);
                        break;
                    }
                }
                for (int i=0; i<resultSearchResult.Size(); i++) {
                    ScTemplateSearchResultItem ResultsearchResultItem = resultSearchResult[i];
                    bool flag = true;
                    for(int j=0; j<ResultsearchResultItem.Size();j++) {
                        ScAddr elem = ResultsearchResultItem[j];
                        if (!ms_context->HelperCheckEdge(elems_2, elem, ScType::EdgeAccessConstPosPerm)) {
                            flag = false;
                            break;
                        }
                    }
                    if (flag) {
                        for(int j=0; j<ResultsearchResultItem.Size();j++)
                            allElem2.push_back(ResultsearchResultItem[j]);
                        break;
                    }
                }
                if (allElem1.empty() || allElem2.empty()) {
                    AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
                    return SC_RESULT_ERROR_INVALID_PARAMS;;
                }











            }
        }




        cout << "hello world" << endl;








        AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
        return SC_RESULT_OK;
    }
}
