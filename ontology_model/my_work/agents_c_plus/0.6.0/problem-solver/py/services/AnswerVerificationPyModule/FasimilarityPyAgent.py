from common import ScModule, ScAgent, ScEventParams
from sc import *

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
        _elems_1 = ""
        _elems_2 = ""
        _elem1 = ""
        _elem2 = ""

# prin in windows
        keynode = self.module.ctx.HelperResolveSystemIdtf("nrel_correct_answer", ScType.NodeConstNoRole)
        self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, keynode)
        it_5 = self.module.ctx.Iterator5(param, ScType.EdgeDCommonConst, ScType.NodeConstStruct, ScType.EdgeAccessConstPosPerm, keynode)
        while it_5.Next():
            _elems_1 = it_5.Get(2)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, _elems_1)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, it_5.Get(1))
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, it_5.Get(3))
            _elem1 = self.module.ctx.CreateNode(ScType.NodeConstTuple)
            _arc1 = self.module.ctx.CreateEdge(ScType.EdgeDCommonConst, _elem1, _elems_1)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, _arc1)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, _elem1)
            keynode = self.module.ctx.HelperResolveSystemIdtf("nrel_relation_decomposition", ScType.NodeConstNoRole)
            _arc2 = self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, keynode, _arc1)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, _arc2)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, keynode)

#prin in windows
        keynode = self.module.ctx.HelperResolveSystemIdtf("nrel_user_answer", ScType.NodeConstNoRole)
        self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, keynode)
        it_5 = self.module.ctx.Iterator5(param, ScType.EdgeDCommonConst, ScType.NodeConstStruct, ScType.EdgeAccessConstPosPerm, keynode)
        while it_5.Next():
            _elems_2 = it_5.Get(2)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, _elems_2)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, it_5.Get(1))
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, it_5.Get(3))
            _elem2 = self.module.ctx.CreateNode(ScType.NodeConstTuple)
            _arc1 = self.module.ctx.CreateEdge(ScType.EdgeDCommonConst, _elem2, _elems_2)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, _arc1)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, _elem2)
            keynode = self.module.ctx.HelperResolveSystemIdtf("nrel_relation_decomposition", ScType.NodeConstNoRole)
            _arc2 = self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, keynode, _arc1)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, _arc2)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, keynode)

#find common arc1
        _class_comm1 = []
        it_3 = self.module.ctx.Iterator3(_elems_1, ScType.EdgeAccessConstPosPerm, ScType.EdgeDCommonConst)
        while it_3.Next():
            elem = it_3.Get(2)
            _class_comm1.append(elem)
        for i in _class_comm1:
            _els1, _els2 = self.module.ctx.GetEdgeInfo(i)
            if self.module.ctx.GetElementType(_els1) == ScType.NodeConstTuple:
                continue
            elem = self.module.ctx.CreateNode(ScType.NodeConstStruct)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, _elem1, elem)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, i)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els1)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els2)
            it_3 = self.module.ctx.Iterator3(ScType.NodeConstNoRole, ScType.EdgeAccessConstPosPerm, i)
            while it_3.Next():
                _els3 = it_3.Get(0)
                _arc = it_3.Get(1)
                self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els3)
                self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _arc)
                break


#find common arc2
        _class_comm2 = []
        it_3 = self.module.ctx.Iterator3(_elems_2, ScType.EdgeAccessConstPosPerm, ScType.EdgeDCommonConst)
        while it_3.Next():
            elem = it_3.Get(2)
            _class_comm2.append(elem)
        for i in _class_comm2:
            _els1, _els2 = self.module.ctx.GetEdgeInfo(i)
            if self.module.ctx.GetElementType(_els1) == ScType.NodeConstTuple:
                continue
            elem = self.module.ctx.CreateNode(ScType.NodeConstStruct)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, _elem2, elem)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, i)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els1)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els2)
            it_3 = self.module.ctx.Iterator3(ScType.NodeConstNoRole, ScType.EdgeAccessConstPosPerm, i)
            while it_3.Next():
                _els3 = it_3.Get(0)
                _arc = it_3.Get(1)
                self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els3)
                self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _arc)
                break

# mistakes#######################
# find posarc 51
        _class_arc1 = []
        it_3 = self.module.ctx.Iterator3(_elems_1, ScType.EdgeAccessConstPosPerm, ScType.EdgeAccessConstPosPerm)
        while it_3.Next():
            elem = it_3.Get(2)
            _class_arc1.append(elem)
        for i in _class_arc1:
            _els1, _els2 = self.module.ctx.GetEdgeInfo(i)
            if (self.module.ctx.GetElementType(_els2) == ScType.EdgeDCommonConst or self.module.ctx.GetElementType(_els2) == ScType.EdgeUCommonConst or
               self.module.ctx.GetElementType(_els2) == ScType.EdgeAccessConstPosPerm or self.module.ctx.GetElementType(_els2) == ScType.NodeConstTuple or
               self.module.ctx.GetElementType(_els1) == ScType.NodeConstTuple):
               continue
            elem = self.module.ctx.CreateNode(ScType.NodeConstStruct)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, _elems_1, elem)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els1)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els2)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, i)
            it_3 = self.module.ctx.Iterator3(ScType.NodeConstRole, ScType.EdgeAccessConstPosPerm, i)
            while it_3.Next():
                _els3 = it_3.Get(0)
                _arc = it_3.Get(1)
                self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els3)
                self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _arc)
                break


# find posarc 52
        _class_arc2 = []
        it_3 = self.module.ctx.Iterator3(_elems_2, ScType.EdgeAccessConstPosPerm, ScType.EdgeAccessConstPosPerm)
        while it_3.Next():
            elem = it_3.Get(2)
            _class_arc2.append(elem)
        for i in _class_arc2:
            _els1, _els2 = self.module.ctx.GetEdgeInfo(i)
            if (self.module.ctx.GetElementType(_els2) == ScType.EdgeDCommonConst or self.module.ctx.GetElementType(_els2) == ScType.EdgeUCommonConst or
                    self.module.ctx.GetElementType(_els2) == ScType.EdgeAccessConstPosPerm or self.module.ctx.GetElementType(_els2) == ScType.NodeConstTuple or
                    self.module.ctx.GetElementType(_els1) == ScType.NodeConstTuple):
                continue
            elem = self.module.ctx.CreateNode(ScType.NodeConstStruct)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, _elems_2, elem)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els1)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els2)
            self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, i)
            it_3 = self.module.ctx.Iterator3(ScType.NodeConstRole, ScType.EdgeAccessConstPosPerm, i)
            while it_3.Next():
                _els3 = it_3.Get(0)
                _arc = it_3.Get(1)
                self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els3)
                self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _arc)
                break


#'''





        #        str = self.module.ctx.HelperGetSystemIdtf(param)
        #       print(str)
        #      print("Answer verification agent")






        result_arc = self.module.ctx.CreateEdge(ScType.EdgeDCommonConst, questionNode, answer)
        result_answer = self.module.ctx.HelperResolveSystemIdtf("nrel_answer", ScType.NodeConstNoRole)
        self.module.ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, result_answer, result_arc)
        return ScResult.Ok
