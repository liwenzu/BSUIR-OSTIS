//
// Created by root on 3/6/21.
//
/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include "utils/Display.hpp"
#include "QuestionGenerationAgent.hpp"
#include "keynodes/keynodes.hpp"

using namespace std;
using namespace utils;

namespace answerVerificationModule {

    SC_AGENT_IMPLEMENTATION(QuestionGenerationAgent) {
            if (!edgeAddr.IsValid())
                return SC_RESULT_ERROR;

            ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
            ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);
            if (!param.IsValid())
                return SC_RESULT_ERROR_INVALID_PARAMS;

            ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, param);


            ScIterator5Ptr it_5 = IteratorUtils::getIterator5(ms_context.get(), param, Keynodes::rrel_main_key_sc_element);
            ScAddr elem, initStruct, resultStruct;
            if (it_5->Next())
            {
                elem = it_5->Get(2);
                initStruct = ms_context->GetEdgeSource(elem);
                resultStruct = ms_context->GetEdgeTarget(elem);
            }

            Display::printEl(ms_context.get(), initStruct);

            cout << "Hello world" << endl;


            AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
            return SC_RESULT_OK;
    }
}

