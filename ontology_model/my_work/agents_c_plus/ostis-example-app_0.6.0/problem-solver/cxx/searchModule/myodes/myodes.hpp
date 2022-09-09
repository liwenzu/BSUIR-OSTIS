/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"
#include "myodes.generated.hpp"

namespace searchModule
{

    class Myodes : public ScObject
    {
        SC_CLASS()
        SC_GENERATED_BODY()

    public:

        SC_PROPERTY(Keynode("question_find_inclusion"), ForceCreate)
        static ScAddr question_find_inclusion;

        SC_PROPERTY(Keynode("nrel_inclusion"), ForceCreate)
        static ScAddr nrel_inclusion;

        SC_PROPERTY(Keynode("question_generating_test_questions_and_exam_tickets"), ForceCreate)
        static ScAddr question_generating_test_questions_and_exam_tickets;

        SC_PROPERTY(Keynode("question_what_is_that"), ForceCreate)
        static ScAddr question_what_is_that;

        SC_PROPERTY(Keynode("logic_rules_for_generating_test_questions"), ForceCreate)
        static ScAddr logic_rules_for_generating_test_questions;

        SC_PROPERTY(Keynode("scope_of_test_questions"), ForceCreate)
        static ScAddr scope_of_test_questions;

        SC_PROPERTY(Keynode("nrel_measure"), ForceCreate)
        static ScAddr nrel_measure;

        SC_PROPERTY(Keynode("number"), ForceCreate)
        static ScAddr number;

        SC_PROPERTY(Keynode("subjective_questions"), ForceCreate)
        static ScAddr subjective_questions;

        SC_PROPERTY(Keynode("objective_questions"), ForceCreate)
        static ScAddr objective_questions;

        SC_PROPERTY(Keynode("concept_examination_paper"), ForceCreate)
        static ScAddr concept_examination_paper;

        SC_PROPERTY(Keynode("set_power_of_386"), ForceCreate)
        static ScAddr set_power_of_386;

        SC_PROPERTY(Keynode("set_power_of_194"), ForceCreate)
        static ScAddr set_power_of_194;


        SC_PROPERTY(Keynode("set_power_of_1"), ForceCreate)
        static ScAddr set_power_of_1;
    };

} // namespace exampleModule
