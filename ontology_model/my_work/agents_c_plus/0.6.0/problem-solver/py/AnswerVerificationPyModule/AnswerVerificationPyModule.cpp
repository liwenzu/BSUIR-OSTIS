//
// Created by root on 12/27/20.
//
#include "AnswerVerificationPyModule.hpp"

SC_IMPLEMENT_MODULE(AnswerVerificationPyModule)

sc_result AnswerVerificationPyModule::InitializeImpl()
{
    m_avService.reset(new FasimilarityPyAgentPythonService("AnswerVerificationPyModule/AnswerVerificationPyModule.py"));
    m_avService->Run();
    return SC_RESULT_OK;
}

sc_result AnswerVerificationPyModule::ShutdownImpl()
{
    m_avService->Stop();
    m_avService.reset();
    return SC_RESULT_OK;
}
