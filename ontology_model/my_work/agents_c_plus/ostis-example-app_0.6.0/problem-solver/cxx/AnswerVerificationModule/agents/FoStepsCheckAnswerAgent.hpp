//
// Created by root on 8/4/21.
//
#pragma once

#include <sc-memory/kpm/sc_agent.hpp>

#include "keynodes/keynodes.hpp"
#include "FoStepsCheckAnswerAgent.generated.hpp"

namespace answerVerificationModule
{

    class FoStepsCheckAnswerAgent : public ScAgent
    {
        SC_CLASS(Agent, Event(Keynodes::following_st_check_answer, ScEvent::Type::AddOutputEdge))
        SC_GENERATED_BODY()
    };

} //answerVerificationModule
