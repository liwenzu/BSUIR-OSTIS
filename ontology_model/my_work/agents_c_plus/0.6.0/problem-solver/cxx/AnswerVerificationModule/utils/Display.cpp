//
// Created by root on 3/6/21.
//
#include "Display.hpp"
#include <iostream>
#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "utils/IteratorUtilsLocal.hpp"
using namespace utils;
namespace answerVerificationModule
{
    void Display::printNl(ScMemoryContext *ms_context, ScAddr element)
    {
        ScType type;
        type = ms_context->GetElementType(element);
        if (type.IsNode() == ScType::Node)
        {
            std::string data;
            data = ms_context->HelperGetSystemIdtf(element);
            if (data.empty())
                std::cout << element.Hash();
            else
                std::cout << data;
        }
        else if (type.IsLink())
        {
            std::string data = CommonUtils::readString(ms_context, element);
            if (data.empty())
                std::cout << element.Hash();
            else
                std::cout << data;
        }
        else
        {
            ScAddr elem1, elem2;
            elem1 = ms_context->GetEdgeSource(element);
            elem2 = ms_context->GetEdgeTarget(element);
            std::cout << "(";
            printNl(ms_context, elem1);
            std::cout << "->";
            printNl(ms_context, elem2);
            std::cout << ")";
        }
    }
    void Display::printEl(ScMemoryContext *ms_context, ScAddr element)
    {
        vector<ScAddr> elementListIn = IteratorUtilsLocal::getAllWithTypeIn(ms_context, element, ScType::Unknown);
        vector<ScAddr> elementListOut = IteratorUtils::getAllWithType(ms_context, element, ScType::Unknown);
        printNl(ms_context, element);
        std::cout << "" << endl;
        std::cout << "Input arc:" << elementListIn.size() << endl;
        for (auto elem : elementListIn)
        {
            std::cout << "<-";
            printNl(ms_context, elem);
            std::cout << "" <<endl;
        }
        std::cout << "Output arc:" << elementListOut.size() << endl;
        for (auto elem : elementListOut)
        {
            std::cout << "->";
            printNl(ms_context, elem);
            std::cout << "" <<endl;
        }
    }
}