/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "keynodes.generated.hpp"

namespace answerVerificationModule
{

    class Keynodes : public ScObject
    {
        SC_CLASS()
    SC_GENERATED_BODY()

    public:

        SC_PROPERTY(Keynode("factual_kn_similarity_calculation"), ForceCreate)
        static ScAddr factual_kn_similarity_calculation;

        SC_PROPERTY(Keynode("nrel_correct_answer"), ForceCreate)
        static ScAddr nrel_correct_answer;

        SC_PROPERTY(Keynode("nrel_user_answer"),ForceCreate)
        static ScAddr nrel_user_answer;

        SC_PROPERTY(Keynode("nrel_relation_decomposition"),ForceCreate)
        static ScAddr nrel_relation_decomposition;

        SC_PROPERTY(Keynode("nrel_system_identifier"),ForceCreate)
        static ScAddr nrel_system_identifier;

        SC_PROPERTY(Keynode("nrel_incorrect_answer"),ForceCreate)
        static ScAddr nrel_incorrect_answer;

        SC_PROPERTY(Keynode("nrel_coefficient"),ForceCreate)
        static ScAddr nrel_coefficient;

        SC_PROPERTY(Keynode("logical_kn_similarity_calculation"), ForceCreate)
        static ScAddr logical_kn_similarity_calculation;

        SC_PROPERTY(Keynode("negation"), ForceCreate)
        static ScAddr negation;

        SC_PROPERTY(Keynode("rrel_0"), ForceCreate)
        static ScAddr rrel_0;

        SC_PROPERTY(Keynode("rrel_1"), ForceCreate)
        static ScAddr rrel_1;

        SC_PROPERTY(Keynode("nrel_basic_sequence"), ForceCreate)
        static ScAddr nrel_basic_sequence;

        SC_PROPERTY(Keynode("rrel_if"), ForceCreate)
        static ScAddr rrel_if;

        SC_PROPERTY(Keynode("rrel_then"), ForceCreate)
        static ScAddr rrel_then;

        SC_PROPERTY(Keynode("nrel_equivalence"), ForceCreate)
        static ScAddr nrel_equivalence;

        SC_PROPERTY(Keynode("rrel_bind_variables"), ForceCreate)
        static ScAddr rrel_bind_variables;

        SC_PROPERTY(Keynode("nrel_possible_answer"), ForceCreate)
        static ScAddr nrel_possible_answer;

        SC_PROPERTY(Keynode("nrel_key_sc_element"), ForceCreate)
        static ScAddr nrel_key_sc_element;

        SC_PROPERTY(Keynode("nrel_definitional_domain"), ForceCreate)
        static ScAddr nrel_definitional_domain;

        SC_PROPERTY(Keynode("question_generation"), ForceCreate)
        static ScAddr question_generation;

        SC_PROPERTY(Keynode("rrel_main_key_sc_element"), ForceCreate)
        static ScAddr rrel_main_key_sc_element;
    };

} // namespace exampleModule
