//
// Created by root on 11/22/20.
//
#include "display.hpp"
#include <iostream>
#include <sc-kpm/sc-agents-common/utils/CommonUtils.hpp>
namespace utils
{
    void display::printEl(ScMemoryContext *ms_context, ScAddr element)
    {
        ScType type;
        type = ms_context->GetElementType(element);
        if (type.IsNode() == ScType::Node)
        {
            std::string data;
            data = ms_context->HelperGetSystemIdtf(element);
            std::cout << data;
        }

        else if (type.IsLink())
        {
            std::string data = CommonUtils::readString(ms_context, element);
            std::cout << data;
        }
        else
        {
            ScAddr elem1, elem2;
            elem1 = ms_context->GetEdgeSource(element);
            elem2 = ms_context->GetEdgeTarget(element);
            std::cout << "(";
            printEl(ms_context, elem1);
            std::cout << "->";
            printEl(ms_context, elem2);
            std::cout << ")";
        }
    }
}