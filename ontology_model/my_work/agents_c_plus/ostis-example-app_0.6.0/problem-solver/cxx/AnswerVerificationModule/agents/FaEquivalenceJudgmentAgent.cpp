//
// Created by root on 6/11/21.
//
#include <iostream>
#include <sc-memory/sc_stream.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include <sc-agents-common/utils/GenerationUtils.hpp>
#include "utils/IteratorUtilsLocal.hpp"
#include "utils/Display.hpp"
#include "FaEquivalenceJudgmentAgent.hpp"


using namespace std;
using namespace utils;

namespace answerVerificationModule {

    SC_AGENT_IMPLEMENTATION(FaEquivalenceJudgmentAgent) {
        if (!edgeAddr.IsValid())
            return SC_RESULT_ERROR;
        ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
        ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(),questionNode);
        if (!param.IsValid())
            return SC_RESULT_ERROR_INVALID_PARAMS;
        ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);


        ScAddr edge = IteratorUtilsLocal::getFirstWithType(ms_context.get(), Keynodes::equivalent_concept_template, ScType::EdgeUCommonConst);
        ScAddr initStruct = ms_context->GetEdgeSource(edge);
        ScAddr resultStruct = ms_context->GetEdgeTarget(edge);
        if(!initStruct.IsValid() || !resultStruct.IsValid())
            return SC_RESULT_ERROR_INVALID_PARAMS;
        ScTemplate initStructTemplate, resultStructTemplate;
        if (ms_context->HelperBuildTemplate(initStructTemplate, initStruct) && ms_context->HelperBuildTemplate(resultStructTemplate, resultStruct)) {
            ScTemplateSearchResult initSearchResult, resultSearchResult;
            if (ms_context->HelperSearchTemplate(initStructTemplate, initSearchResult) && ms_context->HelperSearchTemplate(resultStructTemplate, resultSearchResult)) {

                cout << "&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
                cout << initSearchResult.Size() << " and " << resultSearchResult.Size() << endl;





                for (int i=0; i<initSearchResult.Size(); i++) {
                    ScTemplateSearchResultItem InitsearchResultItem = initSearchResult[i];
                    for(int j=0; j<InitsearchResultItem.Size();j++) {
                        ScAddr elem = InitsearchResultItem[j];
                    if (!ms_context->HelperCheckEdge(answer, elem, ScType::EdgeAccessConstPosPerm))
                        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, elem);
                    }
                }








            }
        }




        cout << "hello world" << endl;








        AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
        return SC_RESULT_OK;
    }
}
