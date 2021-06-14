//
// Created by root on 6/11/21.
//
#pragma once
#include <sc-memory/kpm/sc_agent.hpp>
#include "keynodes/keynodes.hpp"
#include "FaEquivalenceJudgmentAgent.generated.hpp"

namespace answerVerificationModule
{

    class FaEquivalenceJudgmentAgent: public ScAgent
    {
        SC_CLASS(Agent, Event(Keynodes::concept_log_eq_judgment_fa, ScEvent::Type::AddOutputEdge))
        SC_GENERATED_BODY()

    private:
        static void searchStructure(ScMemoryContext * ctx,
                             ScAddr &struct1,
                             ScAddr &struct2,
                             const ScAddr & elems1,
                             const ScAddr & elems2);

    };

} // namespace answerVerificationModule