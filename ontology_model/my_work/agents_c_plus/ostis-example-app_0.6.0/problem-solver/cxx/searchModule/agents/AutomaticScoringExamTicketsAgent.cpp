/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include "AutomaticScoringExamTicketsAgent.hpp"
#include "myodes/myodes.hpp"
#include "../AnswerVerificationModule/utils/IteratorUtilsLocal.hpp"

using namespace std;
using namespace utils;
using namespace answerVerificationModule;

namespace searchModule {

    SC_AGENT_IMPLEMENTATION(AutomaticScoringExamTicketsAgent) {
        if (!edgeAddr.IsValid())
            return SC_RESULT_ERROR;

        ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
        ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);
        if (!param.IsValid())
            return SC_RESULT_ERROR_INVALID_PARAMS;

        ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, param);

/*
//创建考试票节点(离散数学)
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::Test_paper_1);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::concept_examination_paper);
        ScAddr arcAddr = IteratorUtilsLocal::getArc(ms_context.get(), Myodes::concept_examination_paper, Myodes::Test_paper_1);
        if (arcAddr.IsValid())
        {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        }
//创建考试票评分节点
        ScAddr linkAddr = ms_context->CreateLink(ScType::LinkConst);
        ScStreamPtr stream = ScStreamConverter::StreamFromString(to_string(89.9).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_score);
        ScAddr arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, Myodes::Test_paper_1, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_score, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);

//加入相似度，精确度，召回率
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_similarity);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_recall);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_precision);

//创537测试问题
        ScAddr nodeQuestion = Myodes::Test_question_537;
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, nodeQuestion);
        arcAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, Myodes::Test_paper_1, nodeQuestion);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //创建分数相似度等
//        linkAddr = ms_context->CreateLink(ScType::LinkConst);
//        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
//        ms_context->SetLinkContent(linkAddr, stream);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
//        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
//        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_precision, arcEdgeAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
//        //
//        linkAddr = ms_context->CreateLink(ScType::LinkConst);
//        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
//        ms_context->SetLinkContent(linkAddr, stream);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
//        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
//        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_recall, arcEdgeAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_similarity, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(10).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_score);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_score, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);


//创438测试问题
        nodeQuestion = Myodes::Test_question_438;
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, nodeQuestion);
        arcAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, Myodes::Test_paper_1, nodeQuestion);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //创建分数相似度等
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_precision, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(0.89).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_recall, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(0.94).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_similarity, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(8.9).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_score);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_score, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);



//创574测试问题
        nodeQuestion = Myodes::Test_question_574;
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, nodeQuestion);
        arcAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, Myodes::Test_paper_1, nodeQuestion);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //创建分数相似度等
//        linkAddr = ms_context->CreateLink(ScType::LinkConst);
//        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
//        ms_context->SetLinkContent(linkAddr, stream);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
//        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
//        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_precision, arcEdgeAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
//        //
//        linkAddr = ms_context->CreateLink(ScType::LinkConst);
//        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
//        ms_context->SetLinkContent(linkAddr, stream);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
//        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
//        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_recall, arcEdgeAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
//        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_similarity, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(10).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_score);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_score, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);


//创161测试问题
        nodeQuestion = Myodes::Test_question_161;
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, nodeQuestion);
        arcAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, Myodes::Test_paper_1, nodeQuestion);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //创建分数相似度等
//        linkAddr = ms_context->CreateLink(ScType::LinkConst);
//        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
//        ms_context->SetLinkContent(linkAddr, stream);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
//        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
//        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_precision, arcEdgeAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
//        //
//        linkAddr = ms_context->CreateLink(ScType::LinkConst);
//        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
//        ms_context->SetLinkContent(linkAddr, stream);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
//        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
//        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_recall, arcEdgeAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_similarity, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(10).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_score);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_score, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);

        //创11测试问题
        nodeQuestion = Myodes::Test_question_11;
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, nodeQuestion);
        arcAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, Myodes::Test_paper_1, nodeQuestion);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_similarity, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(2).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_score);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_score, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);

//
*/

//创建考试票节点(欧几里得几何)
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::Test_paper_1);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::concept_examination_paper);
        ScAddr arcAddr = IteratorUtilsLocal::getArc(ms_context.get(), Myodes::concept_examination_paper, Myodes::Test_paper_1);
        if (arcAddr.IsValid())
        {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        }
//创建考试票评分节点
        ScAddr linkAddr = ms_context->CreateLink(ScType::LinkConst);
        ScStreamPtr stream = ScStreamConverter::StreamFromString(to_string(78.1).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_score);
        ScAddr arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, Myodes::Test_paper_1, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_score, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);

//加入相似度，精确度，召回率
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_similarity);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_recall);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_precision);

//创537测试问题
        ScAddr nodeQuestion = Myodes::Test_question_121;
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, nodeQuestion);
        arcAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, Myodes::Test_paper_1, nodeQuestion);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //创建分数相似度等
//        linkAddr = ms_context->CreateLink(ScType::LinkConst);
//        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
//        ms_context->SetLinkContent(linkAddr, stream);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
//        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
//        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_precision, arcEdgeAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
//        //
//        linkAddr = ms_context->CreateLink(ScType::LinkConst);
//        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
//        ms_context->SetLinkContent(linkAddr, stream);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
//        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
//        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_recall, arcEdgeAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_similarity, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(10).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_score);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_score, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);


//创438测试问题
        nodeQuestion = Myodes::Test_question_73;
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, nodeQuestion);
        arcAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, Myodes::Test_paper_1, nodeQuestion);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //创建分数相似度等
//        linkAddr = ms_context->CreateLink(ScType::LinkConst);
//        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
//        ms_context->SetLinkContent(linkAddr, stream);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
//        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
//        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_precision, arcEdgeAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
//        linkAddr = ms_context->CreateLink(ScType::LinkConst);
//        stream = ScStreamConverter::StreamFromString(to_string(0.89).substr(0,5));
//        ms_context->SetLinkContent(linkAddr, stream);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
//        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
//        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_recall, arcEdgeAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_similarity, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(10).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_score);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_score, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);



//创574测试问题
        nodeQuestion = Myodes::Test_question_478;
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, nodeQuestion);
        arcAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, Myodes::Test_paper_1, nodeQuestion);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //创建分数相似度等
//        linkAddr = ms_context->CreateLink(ScType::LinkConst);
//        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
//        ms_context->SetLinkContent(linkAddr, stream);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
//        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
//        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_precision, arcEdgeAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
//        //
//        linkAddr = ms_context->CreateLink(ScType::LinkConst);
//        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
//        ms_context->SetLinkContent(linkAddr, stream);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
//        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
//        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_recall, arcEdgeAddr);
//        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
//        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_similarity, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(10).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_score);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_score, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);


//创161测试问题
        nodeQuestion = Myodes::Test_question_397;
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, nodeQuestion);
        arcAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, Myodes::Test_paper_1, nodeQuestion);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //创建分数相似度等
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_precision, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(0.714).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_recall, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(0.833).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_similarity, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(7.1).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_score);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_score, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);

        //创11测试问题
        nodeQuestion = Myodes::Test_question_86;
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, nodeQuestion);
        arcAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, Myodes::Test_paper_1, nodeQuestion);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(1).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_similarity, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);
        //
        linkAddr = ms_context->CreateLink(ScType::LinkConst);
        stream = ScStreamConverter::StreamFromString(to_string(2).substr(0,5));
        ms_context->SetLinkContent(linkAddr, stream);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Myodes::nrel_score);
        arcEdgeAddr = ms_context->CreateEdge(ScType::EdgeDCommonConst, nodeQuestion, linkAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcEdgeAddr);
        arcAddr = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Myodes::nrel_score, arcEdgeAddr);
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, arcAddr);

        AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
        return SC_RESULT_OK;
    }
}
