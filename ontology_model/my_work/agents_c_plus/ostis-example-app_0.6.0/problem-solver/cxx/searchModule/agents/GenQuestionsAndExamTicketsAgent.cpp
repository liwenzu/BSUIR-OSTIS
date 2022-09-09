/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include <random>
#include <chrono>
#include <algorithm>


#include "myodes/myodes.hpp"
#include "GenQuestionsAndExamTicketsAgent.hpp"

using namespace std;
using namespace utils;
using namespace searchModule;

namespace searchModule {

    SC_AGENT_IMPLEMENTATION(GenQuestionsAndExamTicketsAgent) {
        if (!edgeAddr.IsValid())
            return SC_RESULT_ERROR;

        ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
        ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);
        if (!param.IsValid())
            return SC_RESULT_ERROR_INVALID_PARAMS;

        ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, param);

        //加入题型
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::objective_questions);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::subjective_questions);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::concept_examination_paper);
        //设置数量
        ScAddr arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::set_power_of_386, Myodes::objective_questions);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::set_power_of_386);

        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::set_power_of_194, Myodes::subjective_questions);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::set_power_of_194);

        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::set_power_of_1, Myodes::concept_examination_paper);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::set_power_of_1);

        //生成一个考试票

        ScAddr paperNodeAddr = ms_context->CreateNode(ScType::NodeConst);
        ms_context->HelperSetSystemIdtf("Test_paper_1", paperNodeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, paperNodeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::concept_examination_paper, paperNodeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);

        vector<int> num;
        for (int i = 1; i <= 580; i++)
        {
            num.push_back(i);
        }

        unsigned seed = std::chrono::system_clock::now()
                .time_since_epoch()
                .count();
        shuffle(num.begin(), num.end(), std::default_random_engine(seed));

        //生成客观问题
        for (int i = 0; i < 386; i++)
        {
            string str = "Test_question_";
            paperNodeAddr = ms_context->CreateNode(ScType::NodeConst);
            str+=to_string(num[i]);
            ms_context->HelperSetSystemIdtf(str, paperNodeAddr);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, paperNodeAddr);
            arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::objective_questions, paperNodeAddr);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        }

        //生成主观问题
        for (int i = 386; i < num.size(); i++)
        {
            string str = "Test_question_";
            paperNodeAddr = ms_context->CreateNode(ScType::NodeConst);
            str+=to_string(num[i]);
            ms_context->HelperSetSystemIdtf(str, paperNodeAddr);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, paperNodeAddr);
            arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::subjective_questions, paperNodeAddr);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        }


//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_inclusion);
//
//        ScIterator5Ptr iterator5 = IteratorUtils::getIterator5(ms_context.get(), param, Myodes::nrel_inclusion, true);
//        while (iterator5->Next()) {
//        ScAddr sheaf = iterator5->Get(2);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, iterator5->Get(1));
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, sheaf);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, iterator5->Get(3));
//    }



    AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
    return SC_RESULT_OK;
}
}