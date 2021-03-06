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
        keynode = self.module.ctx.HelperResolveSystemIdtf("nrel_correct_answer", ScType.NodeConstNoRole)
        keynode1 = self.module.ctx.HelperResolveSystemIdtf("nrel_relation_decomposition", ScType.NodeConstNoRole)
        _elems_1, _elem1 = Faprocess.generateTuple(self.module.ctx, answer, param, keynode, keynode1)
        keynode = self.module.ctx.HelperResolveSystemIdtf("nrel_user_answer", ScType.NodeConstNoRole)
        _elems_2, _elem2 = Faprocess.generateTuple(self.module.ctx, answer, param, keynode, keynode1)
        Faprocess.decompositionCommon(self.module.ctx, _elems_1, _elem1)
        Faprocess.decompositionCommon(self.module.ctx, _elems_2, _elem2)
        Faprocess.decompositionArcwithrole(self.module.ctx, _elems_1, _elem1)
        Faprocess.decompositionArcwithrole(self.module.ctx, _elems_2, _elem2)
        Faprocess.decompositionTupleWithSub(self.module.ctx, _elems_1, _elem1)
        Faprocess.decompositionTupleWithSub(self.module.ctx, _elems_2, _elem2)
        Faprocess.decompositionTupleWithRelation(self.module.ctx, _elems_1, _elem1)
        Faprocess.decompositionTupleWithRelation(self.module.ctx, _elems_2, _elem2)
        Faprocess.decompositionEdge(self.module.ctx, _elems_1, _elem1)
        Faprocess.decompositionEdge(self.module.ctx, _elems_2, _elem2)
        _summa, _sumsta, _sumcand = 0, 0, 0
        _allsst1 = Utils.getAllWithType(self.module.ctx, _elem1, ScType.NodeConstStruct)
        _allsst2 = Utils.getAllWithType(self.module.ctx, _elem2, ScType.NodeConstStruct)
        _sumsta = len(_allsst1)
        _sumcand = len(_allsst2)
        _classtup1, _classtup2, _classretup1, _classretup2 = [], [], [], []
        _classcomm1, _classcomm2, _classedge1, _classedge2 = [], [], [], []
        _classpost51, _classpost52, _classpost31, _classpost32 = [], [], [], []
        _mathstru = []
        Faprocess.SubstructureClassification(self.module.ctx, _allsst1, _classedge1, _classtup1,
                                             _classcomm1, _classretup1, _classpost51, _classpost31)
        Faprocess.SubstructureClassification(self.module.ctx, _allsst2, _classedge2, _classtup2,
                                             _classcomm2, _classretup2, _classpost52, _classpost32)
        _summa= Faprocess.CommonSimilarityCalculation(self.module.ctx, _classcomm1, _classcomm2, _summa, _mathstru)
        _summa= Faprocess.Post5SimilarityCalculation(self.module.ctx, _classpost51, _classpost52, _summa, _mathstru)
        _summa= Faprocess.Post3SimilarityCalculation(self.module.ctx, _classpost31, _classpost32, _summa, _mathstru)
        _summa= Faprocess.TupSimilarityCalculation(self.module.ctx, _classtup1, _classtup2, _summa, _mathstru)
        _summa= Faprocess.ReTupSimilarityCalculation(self.module.ctx, _classretup1, _classretup2, _summa, _mathstru)
        _summa= Faprocess.EdgeSimilarityCalculation(self.module.ctx, _classedge1, _classedge2, _summa, _mathstru)
        print("Number of substructures of standard answers:%d" % _sumsta)
        print("Number of substructures of user answers:%d" % _sumsta)
        print("Number of substructures of matching answers:%d" % _summa)
        _Fsc = Faprocess.SimilarityCalculation(_summa, _sumcand, _sumsta)
        Faprocess.MatchStructureStatistics(self.module.ctx, _mathstru, _allsst2, answer, param)
        Faprocess.GenerateSimilarityLink(self.module.ctx, answer, param, _Fsc)
        Utils.finishAgentWork(self.module.ctx, questionNode, answer)
        return ScResult.Ok

