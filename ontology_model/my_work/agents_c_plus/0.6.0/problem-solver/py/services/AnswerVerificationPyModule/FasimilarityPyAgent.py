from common import ScModule, ScAgent, ScEventParams
from sc import *
from Utils import Utils
from Faprocess import Faprocess

class FasimilarityPyAgent(ScAgent):
    def CheckImpl(self, evt: ScEventParams) -> bool:
        src, questionNode = self.module.ctx.GetEdgeInfo(evt.edge_addr)
        if questionNode.IsValid() == False:
            return  False
        return  True
    def RunImpl(self, evt: ScEventParams) -> ScResult:
        param = ""
        src, questionNode = self.module.ctx.GetEdgeInfo(evt.edge_addr)
        answer = self.module.ctx.CreateNode(ScType.NodeConstStruct)
        it_3 = self.module.ctx.Iterator3(questionNode, ScType.EdgeAccessConstPosPerm, ScType.Unknown)
        while it_3.Next():
            param = it_3.Get(2)
        self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, param)
        _elems_1, _elems_2 , _elem1, _elem2 = "", "", "", ""
# generate in windows
        keynode = self.module.ctx.HelperResolveSystemIdtf("nrel_correct_answer", ScType.NodeConstNoRole)
        keynode1 = self.module.ctx.HelperResolveSystemIdtf("nrel_relation_decomposition", ScType.NodeConstNoRole)
        _elems_1, _elem1 = Faprocess.generateTuple(self.module.ctx, answer, param, keynode, keynode1)
        keynode = self.module.ctx.HelperResolveSystemIdtf("nrel_user_answer", ScType.NodeConstNoRole)
        _elems_2, _elem2 = Faprocess.generateTuple(self.module.ctx, answer, param, keynode, keynode1)
#find common arc
        Faprocess.decompositionCommon(self.module.ctx, _elems_1, _elem1)
        Faprocess.decompositionCommon(self.module.ctx, _elems_2, _elem2)
# find posarc 3 or 5
        Faprocess.decompositionArcwithrole(self.module.ctx, _elems_1, _elem1)
        Faprocess.decompositionArcwithrole(self.module.ctx, _elems_2, _elem2)
# find the structure of subdividing
        Faprocess.decompositionTupleWithSub(self.module.ctx, _elems_1, _elem1)
        Faprocess.decompositionTupleWithSub(self.module.ctx, _elems_2, _elem2)
# find the structure of tuples with relation
        Faprocess.decompositionTupleWithRelation(self.module.ctx, _elems_1, _elem1)
        Faprocess.decompositionTupleWithRelation(self.module.ctx, _elems_2, _elem2)
# find the structure of edge
        Faprocess.decompositionEdge(self.module.ctx, _elems_1, _elem1)
        Faprocess.decompositionEdge(self.module.ctx, _elems_2, _elem2)




        Utils.finishAgentWork(self.module.ctx, questionNode, answer)
        return ScResult.Ok

#"""



