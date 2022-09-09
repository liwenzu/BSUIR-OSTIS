#pragma once

#include <sc-memory/kpm/sc_agent.hpp>

#include "myodes/myodes.hpp"
#include "GenQuestionsAndExamTicketsAgent.generated.hpp"

namespace searchModule
{
    class GenQuestionsAndExamTicketsAgent : public ScAgent
    {
        SC_CLASS(Agent, Event(Myodes::question_generating_test_questions_and_exam_tickets, ScEvent::Type::AddOutputEdge))
        SC_GENERATED_BODY()
    };
}