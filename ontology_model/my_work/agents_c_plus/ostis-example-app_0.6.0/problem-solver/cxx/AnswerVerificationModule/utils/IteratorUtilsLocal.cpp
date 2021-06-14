//
// Created by root on 3/6/21.
//
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>

#include <sc-memory/sc_iterator.hpp>

#include "IteratorUtilsLocal.hpp"

using namespace std;
using namespace utils;


namespace answerVerificationModule
{
    vector<ScAddr> IteratorUtilsLocal::getAllWithTypeVar(ScMemoryContext * ms_context, const ScAddr & set, ScType scType)
    {
        vector<ScAddr> elementList;
        ScIterator3Ptr iterator3 = ms_context->Iterator3(set, ScType::EdgeAccessVarPosPerm, scType);
        while (iterator3->Next())
        {
            elementList.push_back(iterator3->Get(2));
        }
        return elementList;
    }

    vector<ScAddr> IteratorUtilsLocal::getAllWithTypeIn(ScMemoryContext * ms_context, const ScAddr & set, ScType scType)
    {
        vector<ScAddr> elementList;
        ScIterator3Ptr iterator3 = ms_context->Iterator3(scType, ScType::EdgeAccessConstPosPerm, set);
        while (iterator3->Next())
        {
            elementList.push_back(iterator3->Get(0));
        }
        return elementList;
    }

    vector<ScAddr> IteratorUtilsLocal::getAllByOutRelation(
            ScMemoryContext * ms_context,
            const ScAddr & node,
            const ScAddr & relation)
    {
        vector<ScAddr> elementList;
        ScIterator5Ptr iterator5 = IteratorUtils::getIterator5(ms_context, node, relation);
        while (iterator5->Next())
        {
            elementList.push_back(iterator5->Get(2));
        }
        return elementList;
    }

    ScAddr IteratorUtilsLocal::getFirstWithType(ScMemoryContext *ms_context, const ScAddr & set, ScType scType)
    {
        ScAddr element;
        ScIterator3Ptr iterator3 = ms_context->Iterator3(set, ScType::EdgeAccessConstPosPerm, scType);
        if (iterator3->Next())
        {
            element = iterator3->Get(2);
        }
        return element;
    }

    vector<ScAddr> IteratorUtilsLocal::getAllByOutRelationWithType(ScMemoryContext *ms_context,
                const ScAddr &set,
                ScAddr const & relation,
                ScType scType) {
        vector<ScAddr> elementList;
        bool isRole = CommonUtils::checkType(ms_context, relation, ScType::NodeConstRole);
        ScType arcType = isRole ? ScType::EdgeAccessConstPosPerm : ScType::EdgeDCommonConst;
        ScIterator5Ptr iterator5 = ms_context->Iterator5(set, arcType, scType, ScType::EdgeAccessConstPosPerm, relation);
        while (iterator5->Next())
            elementList.push_back(iterator5->Get(2));
        return elementList;
    }

    ScAddr IteratorUtilsLocal::getFirstFromSetWithType(ScMemoryContext * ms_context, const ScAddr & set, ScType scType)
    {
        ScAddr element;
        ScIterator3Ptr iterator3 = ms_context->Iterator3(set, ScType::EdgeAccessConstPosPerm, scType);
        if (iterator3->Next())
        {
            element = iterator3->Get(2);
        }
        return element;
    }

    ScAddr IteratorUtilsLocal::getFirstFromSetByInReWithType(ScMemoryContext *ms_context, const ScAddr &set, ScType scType)
    {
        ScAddr element;
        ScIterator3Ptr iterator3 = ms_context->Iterator3(scType, ScType::EdgeAccessConstPosPerm, set);
        if (iterator3->Next())
        {
            element = iterator3->Get(0);
        }
        return element;
    }

    ScAddr IteratorUtilsLocal::getFirstByInRelationWithType(ScMemoryContext * ms_context, const ScAddr & node, const ScAddr & relation, ScType scType)
    {
        ScAddr element;
        bool isRole = CommonUtils::checkType(ms_context, relation, ScType::NodeConstRole);
        ScType arcType = isRole ? ScType::EdgeAccessConstPosPerm : ScType::EdgeDCommonConst;
        ScIterator5Ptr iterator5 = ms_context->Iterator5(scType, arcType, node, ScType::EdgeAccessConstPosPerm, relation);
        if (iterator5->Next())
        {
            element = iterator5->Get(0);
        }
        return element;
    }
}