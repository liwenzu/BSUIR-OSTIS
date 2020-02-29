/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include <sc-memory/cpp/sc_addr.hpp>
#include <sc-memory/cpp/sc_object.hpp>

#include "decisionMakingKeynodes.generated.hpp"

namespace decisionMaking
{

class DecisionMakingKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:

  SC_PROPERTY(Keynode("question_reverse_inference"), ForceCreate)
  static ScAddr question_reverse_inference;

  SC_PROPERTY(Keynode("question_decision_making"), ForceCreate)
  static ScAddr question_decision_making;

  SC_PROPERTY(Keynode("question_decision_making_preparation"), ForceCreate)
  static ScAddr question_decision_making_preparation;

  SC_PROPERTY(Keynode("question_correct_marking"), ForceCreate)
  static ScAddr question_correct_marking;

  SC_PROPERTY(Keynode("question_use_logic_rule"), ForceCreate)
  static ScAddr question_use_logic_rule;

  SC_PROPERTY(Keynode("question_numbers_comparison"), ForceCreate)
  static ScAddr question_numbers_comparison;

  SC_PROPERTY(Keynode("concept_product"), ForceCreate)
  static ScAddr concept_product;

  SC_PROPERTY(Keynode("concept_marked_product"), ForceCreate)
  static ScAddr concept_marked_product;

  SC_PROPERTY(Keynode("concept_correctly_marked_product"), ForceCreate)
  static ScAddr concept_correctly_marked_product;

  SC_PROPERTY(Keynode("concept_critical_bad_marking_product"), ForceCreate)
  static ScAddr concept_critical_bad_marking_product;

  SC_PROPERTY(Keynode("concept_test_yogurt"), ForceCreate)
  static ScAddr concept_test_yogurt;

  SC_PROPERTY(Keynode("concept_control_point"), ForceCreate)
  static ScAddr concept_control_point;

  SC_PROPERTY(Keynode("nrel_product_for_check"), ForceCreate)
  static ScAddr nrel_product_for_check;

  SC_PROPERTY(Keynode("nrel_standard_marking"), ForceCreate)
  static ScAddr nrel_standard_marking;

  SC_PROPERTY(Keynode("nrel_marking"), ForceCreate)
  static ScAddr nrel_marking;

  SC_PROPERTY(Keynode("nrel_consecutive_unmarked_products"), ForceCreate)
  static ScAddr nrel_consecutive_unmarked_products;

  SC_PROPERTY(Keynode("nrel_unmarked_products_limit"), ForceCreate)
  static ScAddr nrel_unmarked_products_limit;

  SC_PROPERTY(Keynode("nrel_decision_object"), ForceCreate)
  static ScAddr nrel_decision_object;

  SC_PROPERTY(Keynode("nrel_decision_messages"), ForceCreate)
  static ScAddr nrel_decision_messages;

  SC_PROPERTY(Keynode("nrel_decision_programs"), ForceCreate)
  static ScAddr nrel_decision_programs;

  SC_PROPERTY(Keynode("concept_decision"), ForceCreate)
  static ScAddr concept_decision;

  SC_PROPERTY(Keynode("good_flow"), ForceCreate)
  static ScAddr good_flow;

  SC_PROPERTY(Keynode("nrel_decision_tree"), ForceCreate)
  static ScAddr nrel_decision_tree;

  SC_PROPERTY(Keynode("nrel_checked_products"), ForceCreate)
  static ScAddr nrel_checked_products;

};

} // namespace decisionMaking
