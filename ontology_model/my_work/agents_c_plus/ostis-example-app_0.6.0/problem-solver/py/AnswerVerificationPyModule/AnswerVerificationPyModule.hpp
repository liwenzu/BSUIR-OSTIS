//
// Created by root on 12/27/20.
//
#pragma once

#include "sc-memory/sc_module.hpp"
#include "AnswerVerificationService.hpp"
#include "AnswerVerificationPyModule.generated.hpp"


class AnswerVerificationPyModule : public ScModule
{
    SC_CLASS(LoadOrder(1000))
    SC_GENERATED_BODY()

    virtual sc_result InitializeImpl() override;
    virtual sc_result ShutdownImpl() override;

private:
    std::unique_ptr<FasimilarityPyAgentPythonService> m_avService;
};
