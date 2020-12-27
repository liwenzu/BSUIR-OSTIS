/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "factual_knowledge_similarity_calculation.hpp"
#include "keynodes/keynodes.hpp"
#include "agents/FasimilarityAgent.hpp"

using namespace facknowsimcalcu;

SC_IMPLEMENT_MODULE(Facknowsimcalcu)

sc_result Facknowsimcalcu::InitializeImpl()
{
    if (!facknowsimcalcu::Keynodes::InitGlobal())
        return SC_RESULT_ERROR;

    SC_AGENT_REGISTER(FasimilarityAgent)

    return SC_RESULT_OK;
}

sc_result Facknowsimcalcu::ShutdownImpl()
{
    SC_AGENT_UNREGISTER(FasimilarityAgent)

    return SC_RESULT_OK;
}
