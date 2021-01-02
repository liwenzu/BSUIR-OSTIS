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
# Similarity calculation part
        _summa, _sumsta, _sumcand = 0, 0, 0
        _allsst1 = Utils.getAllWithType(self.module.ctx, _elem1, ScType.NodeConstStruct)
        _allsst2 = Utils.getAllWithType(self.module.ctx, _elem2, ScType.NodeConstStruct)
        _sumsta = len(_allsst1)
        _sumcand = len(_allsst2)
        _classtup1, _classtup2, _classretup1, _classretup2 = [], [], [], []
        _classcomm1, _classcomm2, _classedge1, _classedge2 = [], [], [], []
        _classpost51, _classpost52, _classpost31, _classpost32 = [], [], [], []
        _mathstru, _mismathstru = [], []
        Faprocess.SubstructureClassification(self.module.ctx, _allsst1, _classedge1, _classtup1,
                                             _classcomm1, _classretup1, _classpost51, _classpost31)
        Faprocess.SubstructureClassification(self.module.ctx, _allsst2, _classedge2, _classtup2,
                                             _classcomm2, _classretup2, _classpost52, _classpost32)
# CommonSimilarityCalculation
        _summa= Faprocess.CommonSimilarityCalculation(self.module.ctx, _classcomm1, _classcomm2, _summa, _mathstru)





        print("Standard answer:%d" % _sumsta)
        print("Uswer answer:%d" % _sumsta)
        print("The matched answer:%d" % _summa)

        Utils.finishAgentWork(self.module.ctx, questionNode, answer)
        return ScResult.Ok

#"""

"""
        print(len(_classcomm1))
        print(len(_classedge1))
        print(len(_classtup1))
        print(len(_classretup1))
        print(len(_classpost51))
        print(len(_classpost31))
        print("*****************")
        print(len(_classcomm2))
        print(len(_classedge2))
        print(len(_classtup2))
        print(len(_classretup2))
        print(len(_classpost52))
        print(len(_classpost32))
        print("*****************")
"""
