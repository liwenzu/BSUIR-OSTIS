//
// Created by root on 8/4/21.
//
#include "FoStepsCheckAnswerAgent.hpp"
#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>

using namespace std;
using namespace utils;

namespace answerVerificationModule
{

    SC_AGENT_IMPLEMENTATION(FoStepsCheckAnswerAgent)
        {
            if (!edgeAddr.IsValid())
                return SC_RESULT_ERROR;

            ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
            ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);
            if (!param.IsValid())
                return SC_RESULT_ERROR_INVALID_PARAMS;

            ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, param);



            cout << "Hello World!" << endl;






            AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
            return SC_RESULT_OK;
        }
}
