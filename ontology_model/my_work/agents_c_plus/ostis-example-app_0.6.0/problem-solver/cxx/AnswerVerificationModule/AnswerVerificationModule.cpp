/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "AnswerVerificationModule.hpp"
#include "keynodes/keynodes.hpp"
#include "agents/FasimilarityAgent.hpp"
#include "agents/LosimilarityAgent.hpp"
#include "agents/QuestionGenerationAgent.hpp"

using namespace answerVerificationModule;

SC_IMPLEMENT_MODULE(AnswerVerificationModule)

sc_result AnswerVerificationModule::InitializeImpl()
{
    if (!answerVerificationModule::Keynodes::InitGlobal())
        return SC_RESULT_ERROR;

    SC_AGENT_REGISTER(FasimilarityAgent)
    SC_AGENT_REGISTER(LosimilarityAgent)
    SC_AGENT_REGISTER(QuestionGenerationAgent)

    return SC_RESULT_OK;
}

sc_result AnswerVerificationModule::ShutdownImpl()
{
    SC_AGENT_UNREGISTER(FasimilarityAgent)
    SC_AGENT_UNREGISTER(LosimilarityAgent)
    SC_AGENT_UNREGISTER(QuestionGenerationAgent)

    return SC_RESULT_OK;
}
