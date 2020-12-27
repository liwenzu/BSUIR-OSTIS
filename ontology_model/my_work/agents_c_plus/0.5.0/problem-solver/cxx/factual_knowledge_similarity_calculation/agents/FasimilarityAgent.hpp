/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include <sc-memory/cpp/kpm/sc_agent.hpp>

#include "keynodes/keynodes.hpp"
#include "FasimilarityAgent.generated.hpp"

namespace facknowsimcalcu
{

    class FasimilarityAgent : public ScAgent
    {
        SC_CLASS(Agent, Event(Keynodes::factual_kn_similarity_calculation, ScEvent::Type::AddOutputEdge))
        SC_GENERATED_BODY()

    private:
        void SubstructureDecomposition(ScMemoryContext * ms_context,
                                       ScAddr & elems_1,
                                       ScAddr & elem1,
                                       ScAddr & elems_2,
                                       ScAddr & elem2
                                       );
    };

} // namespace exampleModule
