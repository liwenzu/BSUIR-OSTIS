/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include "AutomaticScoringExamTicketsAgent.hpp"
#include "myodes/myodes.hpp"

using namespace std;
using namespace utils;

namespace searchModule {

    SC_AGENT_IMPLEMENTATION(AutomaticScoringExamTicketsAgent) {
        if (!edgeAddr.IsValid())
            return SC_RESULT_ERROR;

        ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
        ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);
        if (!param.IsValid())
            return SC_RESULT_ERROR_INVALID_PARAMS;

        ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, param);





        AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
        return SC_RESULT_OK;
    }
}
