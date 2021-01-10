/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include <sc-memory/kpm/sc_agent.hpp>

#include "keynodes/keynodes.hpp"
#include "LosimilarityAgent.generated.hpp"

namespace answerVerificationModule
{

    class LosimilarityAgent : public ScAgent
    {
        SC_CLASS(Agent, Event(Keynodes::logical_kn_similarity_calculation, ScEvent::Type::AddOutputEdge))
        SC_GENERATED_BODY()
    };

} // namespace answerVerificationModule
