//
// Created by root on 4/15/21.
//
#include <iostream>
#include "QuestionGenerationProcess.hpp"
#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "utils/IteratorUtilsLocal.hpp"
#include "keynodes/genKeynodes.hpp"

using namespace utils;
namespace answerVerificationModule
{
    void QuestionGenerationProcess::findAllElement(ScMemoryContext *ms_context, const ScAddr &node,
                                                   vector<ScAddr> &keyElemListCp, const ScAddr &relation) {
        vector<ScAddr> elementList = IteratorUtilsLocal::getAllByOutRelation(ms_context, node, relation);
        if (elementList.empty())
            return;
        for (auto element : elementList) {
            keyElemListCp.push_back(element);
            findAllElement(ms_context, element, keyElemListCp, relation);
        }
    }

    void QuestionGenerationProcess::templateGeneration(ScMemoryContext *ms_context, const ScTemplate &resultStructTemplate,
                                                  const ScTemplateParams &templateParams,
                                                  std::vector<ScAddr> &elemDuplicate, const ScAddr &answer,
                                                  const ScAddr &keyElem) {
        ScTemplateGenResult genResult;
        if (ms_context->HelperGenTemplate(resultStructTemplate, genResult, templateParams))
        {
            cout << "Hello genResult" << endl;
            cout << genResult.Size() << endl;
            for (int k=0; k<genResult.Size(); k++)
                ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, genResult[k]);
            ScAddr elem = genResult["_question_number"];
            vector<ScAddr> objectQuestion = IteratorUtils::getAllWithType(ms_context, GenKeynodes::objective_questions, ScType::NodeConst);
            int num = objectQuestion.size();
            string strQuestion = "Generated_Question";
            string strNum = to_string(num);
            ms_context->HelperSetSystemIdtf(strQuestion + strNum, elem);
            elemDuplicate.push_back(keyElem);
        }
    }



}