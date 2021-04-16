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

        SC_PROPERTY(Keynode("nrel_subdividing"), ForceCreate)
        static ScAddr nrel_subdividing;

        SC_PROPERTY(Keynode("nrel_strict_inclusion"), ForceCreate)
        static ScAddr nrel_strict_inclusion;

        SC_PROPERTY(Keynode("subject_domain_of_sets"), ForceCreate)
        static ScAddr subject_domain_of_sets;

        SC_PROPERTY(Keynode("nrel_mapping"), ForceCreate)
        static ScAddr nrel_mapping;

        SC_PROPERTY(Keynode("multiple_choice_questions_based_on_binary_relation"), ForceCreate)
        static ScAddr multiple_choice_questions_based_on_binary_relation;

        SC_PROPERTY(Keynode("nrel_author"), ForceCreate)
        static ScAddr nrel_author;

        SC_PROPERTY(Keynode("nrel_idtf"), ForceCreate)
        static ScAddr nrel_idtf;

        SC_PROPERTY(Keynode("nrel_main_idtf"), ForceCreate)
        static ScAddr nrel_main_idtf;

        SC_PROPERTY(Keynode("lang_ru"), ForceCreate)
        static ScAddr lang_ru;

        SC_PROPERTY(Keynode("multiple_choice_questions_based_on_set_identifier"), ForceCreate)
        static ScAddr multiple_choice_questions_based_on_set_identifier;

        SC_PROPERTY(Keynode("multiple_choice_questions_based_on_relation_identifier"), ForceCreate)
        static ScAddr multiple_choice_questions_based_on_relation_identifier;

        SC_PROPERTY(Keynode("multiple_choice_questions_based_on_axiomatics"), ForceCreate)
        static ScAddr multiple_choice_questions_based_on_axiomatics;

        SC_PROPERTY(Keynode("observation"), ForceCreate)
        static ScAddr observation;

        SC_PROPERTY(Keynode("multiple_choice_questions_based_on_image_examples"), ForceCreate)
        static ScAddr multiple_choice_questions_based_on_image_examples;

        SC_PROPERTY(Keynode("multiple_choice_questions_based_on_relation_attributes"), ForceCreate)
        static ScAddr multiple_choice_questions_based_on_relation_attributes;

        SC_PROPERTY(Keynode("binary_relation"), ForceCreate)
        static ScAddr binary_relation;

        SC_PROPERTY(Keynode("relation"), ForceCreate)
        static ScAddr relation;

        SC_PROPERTY(Keynode("multiple_choice_questions"), ForceCreate)
        static ScAddr multiple_choice_questions;

        SC_PROPERTY(Keynode("fill_in_the_blank_questions"), ForceCreate)
        static ScAddr fill_in_the_blank_questions;

        SC_PROPERTY(Keynode("fill_in_the_blank_questions_based_on_inclusion_relation"), ForceCreate)
        static ScAddr fill_in_the_blank_questions_based_on_inclusion_relation;

        SC_PROPERTY(Keynode("fill_in_the_blank_questions_with_a_blank"), ForceCreate)
        static ScAddr fill_in_the_blank_questions_with_a_blank;

        SC_PROPERTY(Keynode("fill_in_the_blank_questions_with_unique_answers"), ForceCreate)
        static ScAddr fill_in_the_blank_questions_with_unique_answers;

        SC_PROPERTY(Keynode("fill_in_the_blank_questions_with_several_possible_answers"), ForceCreate)
        static ScAddr fill_in_the_blank_questions_with_several_possible_answers;

        SC_PROPERTY(Keynode("fill_in_the_blank_questions_with_multiple_blanks"), ForceCreate)
        static ScAddr fill_in_the_blank_questions_with_multiple_blanks;

        SC_PROPERTY(Keynode("fill_in_the_blank_questions_based_on_strict_inclusion_relation"), ForceCreate)
        static ScAddr fill_in_the_blank_questions_based_on_strict_inclusion_relation;
    };
}
