////
//// Created by root on 3/10/21.
////
#pragma once
#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "genKeynodes.generated.hpp"

namespace answerVerificationModule
{

    class GenKeynodes : public ScObject
    {
        SC_CLASS()
        SC_GENERATED_BODY()

    public:
        SC_PROPERTY(Keynode("question_generation"), ForceCreate)
        static ScAddr question_generation;

        SC_PROPERTY(Keynode("rrel_main_key_sc_element"), ForceCreate)
        static ScAddr rrel_main_key_sc_element;

        SC_PROPERTY(Keynode("subject_domain_of_actions_and_tasks"), ForceCreate)
        static ScAddr subject_domain_of_actions_and_tasks;

        SC_PROPERTY(Keynode("nrel_inclusion"), ForceCreate)
        static ScAddr nrel_inclusion;

        SC_PROPERTY(Keynode("rrel_not_maximum_studied_object_class"), ForceCreate)
        static ScAddr rrel_not_maximum_studied_object_class;

        SC_PROPERTY(Keynode("choice_the_incorrect_option"), ForceCreate)
        static ScAddr choice_the_incorrect_option;

        SC_PROPERTY(Keynode("rrel_key_sc_element"), ForceCreate)
        static ScAddr rrel_key_sc_element;

        SC_PROPERTY(Keynode("choice_the_correct_option"), ForceCreate)
        static ScAddr choice_the_correct_option;

        SC_PROPERTY(Keynode("objective_questions"), ForceCreate)
        static ScAddr objective_questions;

        SC_PROPERTY(Keynode("multiple_choice_questions_with_multiple_options"), ForceCreate)
        static ScAddr multiple_choice_questions_with_multiple_options;

        SC_PROPERTY(Keynode("multiple_choice_questions_with_single_option"), ForceCreate)
        static ScAddr multiple_choice_questions_with_single_option;
    };
}
