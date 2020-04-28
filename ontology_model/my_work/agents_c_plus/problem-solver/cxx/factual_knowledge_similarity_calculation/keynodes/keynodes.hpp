/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/cpp/sc_addr.hpp"
#include "sc-memory/cpp/sc_object.hpp"

#include "keynodes.generated.hpp"

namespace facknowsimcalcu
{

    class Keynodes : public ScObject
    {
        SC_CLASS()
    SC_GENERATED_BODY()

    public:

        SC_PROPERTY(Keynode("factual_kn_similarity_calculation"), ForceCreate)
        static ScAddr factual_kn_similarity_calculation;

//        SC_PROPERTY(Keynode("nrel_inclusion"), ForceCreate)
//        static ScAddr nrel_inclusion;
    };

} // namespace exampleModule
