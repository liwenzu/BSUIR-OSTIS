from common import ScModule, ScKeynodes, ScPythonEventType
from keynodes import Keynodes
from FasimilarityPyAgent import FasimilarityPyAgent

from sc import *


class AnswerVerificationPyModule(ScModule):

    def __init__(self):
        ScModule.__init__(
            self,
            ctx=__ctx__,
            cpp_bridge=__cpp_bridge__,
            keynodes=[
            ],
        )

    def OnInitialize(self, params):
        agent = FasimilarityPyAgent(self)
        fa_addr = self.ctx.HelperResolveSystemIdtf("factual_kn_similarity_calculation_py", ScType.NodeConstClass)
        agent.Register(fa_addr, ScPythonEventType.AddOutputEdge)

    def OnShutdown(self):
        print('Shutting answer verification  module')


service = AnswerVerificationPyModule()
service.Run()
