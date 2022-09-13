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

        SC_PROPERTY(Keynode("set_power_of_236"), ForceCreate)
        static ScAddr set_power_of_236;

        SC_PROPERTY(Keynode("set_power_of_377"), ForceCreate)
        static ScAddr set_power_of_377;

        SC_PROPERTY(Keynode("Test_paper_1"), ForceCreate)
        static ScAddr Test_paper_1;

        SC_PROPERTY(Keynode("rrel_1"), ForceCreate)
        static ScAddr rrel_1;

        SC_PROPERTY(Keynode("nrel_basic_sequence"), ForceCreate)
        static ScAddr nrel_basic_sequence;

        SC_PROPERTY(Keynode("Test_question_86"), ForceCreate)
        static ScAddr Test_question_86;

        SC_PROPERTY(Keynode("rrel_key_sc_element"), ForceCreate)
        static ScAddr rrel_key_sc_element;

        SC_PROPERTY(Keynode("nrel_standard_answer"), ForceCreate)
        static ScAddr nrel_standard_answer;

        SC_PROPERTY(Keynode("nrel_user_answer"), ForceCreate)
        static ScAddr nrel_user_answer;

        SC_PROPERTY(Keynode("nrel_correct_option"), ForceCreate)
        static ScAddr nrel_correct_option;

        SC_PROPERTY(Keynode("lang_ru"), ForceCreate)
        static ScAddr lang_ru;

        SC_PROPERTY(Keynode("semantic_model_exam_tickets"), ForceCreate)
        static ScAddr semantic_model_exam_tickets;

        SC_PROPERTY(Keynode("automatic_scoring_exam_tickets"), ForceCreate)
        static ScAddr automatic_scoring_exam_tickets;
    };

} // namespace exampleModule
