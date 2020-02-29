/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include <sc-memory/cpp/sc_memory.hpp>

#include "decisionMakingKeynodes.hpp"

namespace decisionMaking
{

ScAddr DecisionMakingKeynodes::question_reverse_inference;
ScAddr DecisionMakingKeynodes::question_decision_making;
ScAddr DecisionMakingKeynodes::question_decision_making_preparation;
ScAddr DecisionMakingKeynodes::question_correct_marking;
ScAddr DecisionMakingKeynodes::question_use_logic_rule;
ScAddr DecisionMakingKeynodes::question_numbers_comparison;
ScAddr DecisionMakingKeynodes::concept_product;
ScAddr DecisionMakingKeynodes::concept_marked_product;
ScAddr DecisionMakingKeynodes::concept_correctly_marked_product;
ScAddr DecisionMakingKeynodes::concept_critical_bad_marking_product;
ScAddr DecisionMakingKeynodes::concept_test_yogurt;
ScAddr DecisionMakingKeynodes::concept_control_point;
ScAddr DecisionMakingKeynodes::nrel_product_for_check;
ScAddr DecisionMakingKeynodes::nrel_standard_marking;
ScAddr DecisionMakingKeynodes::nrel_marking;
ScAddr DecisionMakingKeynodes::nrel_consecutive_unmarked_products;
ScAddr DecisionMakingKeynodes::nrel_unmarked_products_limit;
ScAddr DecisionMakingKeynodes::nrel_decision_object;
ScAddr DecisionMakingKeynodes::nrel_decision_messages;
ScAddr DecisionMakingKeynodes::nrel_decision_programs;
ScAddr DecisionMakingKeynodes::concept_decision;
ScAddr DecisionMakingKeynodes::good_flow;
ScAddr DecisionMakingKeynodes::nrel_decision_tree;
ScAddr DecisionMakingKeynodes::nrel_checked_products;

} // namespace decisionMaking
