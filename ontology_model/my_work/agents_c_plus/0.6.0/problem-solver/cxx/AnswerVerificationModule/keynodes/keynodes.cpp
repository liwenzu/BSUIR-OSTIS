/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "keynodes.hpp"

#include <sc-memory/sc_memory.hpp>

namespace answerVerificationModule
{

    ScAddr Keynodes::factual_kn_similarity_calculation;
    ScAddr Keynodes::nrel_correct_answer;
    ScAddr Keynodes::nrel_user_answer;
    ScAddr Keynodes::nrel_relation_decomposition;
    ScAddr Keynodes::nrel_system_identifier;
    ScAddr Keynodes::nrel_incorrect_answer;
    ScAddr Keynodes::nrel_coefficient;
    ScAddr Keynodes::logical_kn_similarity_calculation;
    ScAddr Keynodes::negation;
    ScAddr Keynodes::rrel_0;
    ScAddr Keynodes::rrel_1;
    ScAddr Keynodes::nrel_basic_sequence;
}
