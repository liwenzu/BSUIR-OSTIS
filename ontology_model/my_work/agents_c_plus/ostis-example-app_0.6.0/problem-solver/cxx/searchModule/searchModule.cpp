/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "searchModule.hpp"
#include "myodes/myodes.hpp"
#include "agents/InclusionSearchAgent.hpp"
#include "agents/GenQuestionsAndExamTicketsAgent.hpp"
#include "agents/WhatIsThatAgent.hpp"

using namespace searchModule;

SC_IMPLEMENT_MODULE(SearchModule)

sc_result SearchModule::InitializeImpl()
{
  if (!searchModule::Myodes::InitGlobal())
     return SC_RESULT_ERROR;

  SC_AGENT_REGISTER(InclusionSearchAgent)
  SC_AGENT_REGISTER(GenQuestionsAndExamTicketsAgent)
  SC_AGENT_REGISTER(WhatIsThatAgent)

  return SC_RESULT_OK;
}

sc_result SearchModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(InclusionSearchAgent)
  SC_AGENT_UNREGISTER(GenQuestionsAndExamTicketsAgent)
  SC_AGENT_UNREGISTER(WhatIsThatAgent)

  return SC_RESULT_OK;
}
