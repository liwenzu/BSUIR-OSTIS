/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include <sc-memory/cpp/sc_addr.hpp>

using namespace std;

namespace decisionMaking
{

class ProductUtils
{
public:
  static bool isProduct(ScMemoryContext * context, ScAddr & product);

  static bool isCorrectlyMarkedProduct(ScMemoryContext * context, ScAddr & product);
};
}
