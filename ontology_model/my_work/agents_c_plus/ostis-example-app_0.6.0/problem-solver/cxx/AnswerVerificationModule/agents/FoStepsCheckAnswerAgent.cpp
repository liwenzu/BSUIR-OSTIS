//
// Created by root on 8/4/21.
//
#include "FoStepsCheckAnswerAgent.hpp"
#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include "utils/Display.hpp"
#include "utils/Foprocess.hpp"
#include <stack>

using namespace std;
using namespace utils;

namespace answerVerificationModule
{

    SC_AGENT_IMPLEMENTATION(FoStepsCheckAnswerAgent)
        {
            if (!edgeAddr.IsValid())
                return SC_RESULT_ERROR;

            ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
            ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);
            if (!param.IsValid())
                return SC_RESULT_ERROR_INVALID_PARAMS;

            ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, param);

            ScAddr tupleSet = IteratorUtils::getFirstByInRelation(ms_context.get(), param, Keynodes::nrel_decomposition_of_action);
            ScAddr firstStep = IteratorUtils::getFirstByOutRelation(ms_context.get(), tupleSet, Keynodes::rrel_1);
            if (!firstStep.IsValid() || !tupleSet.IsValid())
                return SC_RESULT_ERROR_INVALID_PARAMS;
            ScAddr templateSet = IteratorUtils::getFirstByOutRelation(ms_context.get(), param, Keynodes::nrel_template);
            if (!templateSet.IsValid())
                return SC_RESULT_ERROR_INVALID_PARAMS;
            ScAddr firstTemplate = IteratorUtils::getFirstByOutRelation(ms_context.get(), templateSet, Keynodes::rrel_1);
            if (!firstTemplate.IsValid())
                return SC_RESULT_ERROR_INVALID_PARAMS;
//用户答案编号并且记录编号和有关属性的节点
            unordered_map<ScAddr, pair<ScAddr,int>, ScAddrHashFunc< uint32_t >> answerMap;
            AnswerCheckProcess::answerNumbering(ms_context.get(), firstStep, answerMap);
//答案中可能有多个相同的模板，因此设置一个标志位哈希表
            unordered_map<ScAddr, bool, ScAddrHashFunc< uint32_t >> flagMap;
            for(const auto &currElem : answerMap)
                flagMap[currElem.first] = false;
//用于验证解题步骤正确性的栈;
            stack<int> numberStruct;
//用于记录答案匹配情况的向量
//            vector<ScAddr> mathStru, incoStepStru, incoTheoremStru, incoConditionStru, incoResultStru;
//用一个数组加链表将几种答案类型存储起来,0,      1,           2,               3,                     4
            vector<list<ScAddr>> answerMatchStatus(5);
//记录模板和答案的匹配关系
            unordered_map<ScAddr, int, ScAddrHashFunc< uint32_t >> tempMatchStatus;
//遍历每个模板，检查答案匹配情况
            AnswerCheckProcess::traversalTemplate(ms_context.get(), firstTemplate, answerMap, numberStruct, answerMatchStatus, flagMap, tempMatchStatus);
//计算模板数，和所有答案数
            vector<ScAddr> templateList = IteratorUtils::getAllWithType(ms_context.get(), templateSet, ScType::Unknown);
            int templateNumber = templateList.size();
            int answerNumber = answerMap.size();



            cout << templateNumber  << " : " << answerNumber << endl;









            AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
            return SC_RESULT_OK;
        }
}
