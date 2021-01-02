from sc import *
from Utils import Utils
class Faprocess:
# prin in windows
    @staticmethod
    def generateTuple(ctx, answer, param, key_node, key_node1):
        mid_elems, mid_elem = "", ""
        ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, key_node)
        it_5 = ctx.Iterator5(param, ScType.EdgeDCommonConst, ScType.NodeConstStruct, ScType.EdgeAccessConstPosPerm, key_node)
        while it_5.Next():
            mid_elems = it_5.Get(2)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, mid_elems)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, it_5.Get(1))
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, it_5.Get(3))
            mid_elem = ctx.CreateNode(ScType.NodeConstTuple)
            _arc1 = ctx.CreateEdge(ScType.EdgeDCommonConst, mid_elem, mid_elems)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, _arc1)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, mid_elem)
            _arc2 = ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, key_node1, _arc1)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, _arc2)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, key_node1)
        return  mid_elems, mid_elem
#find common arc
    @staticmethod
    def decompositionCommon(ctx, mid_elems, mid_elem):
        _class_comm = Utils.getAllWithType(ctx, mid_elems, ScType.EdgeDCommonConst)
        for i in _class_comm:
            _els1, _els2 = ctx.GetEdgeInfo(i)
            if ctx.GetElementType(_els1) == ScType.NodeConstTuple:
                continue
            elem = ctx.CreateNode(ScType.NodeConstStruct)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, mid_elem, elem)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, i)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els1)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els2)
            it_3 = ctx.Iterator3(ScType.NodeConstNoRole, ScType.EdgeAccessConstPosPerm, i)
            while it_3.Next():
                _els3 = it_3.Get(0)
                _arc = it_3.Get(1)
                ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els3)
                ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _arc)
                break
# find posarc 3 or 5
    @staticmethod
    def decompositionArcwithrole(ctx, mid_elems, mid_elem):
        _class_arc = Utils.getAllWithType(ctx, mid_elems, ScType.EdgeAccessConstPosPerm)
        for i in _class_arc:
            _els1, _els2 = ctx.GetEdgeInfo(i)
            if (ctx.GetElementType(_els2) == ScType.EdgeDCommonConst or ctx.GetElementType(_els2) == ScType.EdgeUCommonConst or
                ctx.GetElementType(_els2) == ScType.EdgeAccessConstPosPerm or ctx.GetElementType(_els2) == ScType.NodeConstTuple or
                ctx.GetElementType(_els1) == ScType.NodeConstTuple):
                continue
            elem = ctx.CreateNode(ScType.NodeConstStruct)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, mid_elem, elem)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els1)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els2)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, i)
            it_3 = ctx.Iterator3(ScType.NodeConstRole, ScType.EdgeAccessConstPosPerm, i)
            while it_3.Next():
                _els3 = it_3.Get(0)
                _arc = it_3.Get(1)
                ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els3)
                ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _arc)
                break
# find the structure of subdividing
    @staticmethod
    def decompositionTupleWithSub(ctx, mid_elems, mid_elem):
        _class_tups = Utils.getAllWithType(ctx, mid_elems, ScType.NodeConstTuple)
        for i in _class_tups:
            it_5 = ctx.Iterator5(i, ScType.EdgeDCommonConst,
                                             ScType.Unknown,
                                             ScType.EdgeAccessConstPosPerm,
                                             ScType.NodeConstNoRole)
            if not it_5.Next():
                continue
            _arccom = it_5.Get(1)
            _els2 = it_5.Get(2)
            _arc = it_5.Get(3)
            _els3 = it_5.Get(4)
            _elemtup = Utils.getAllWithType(ctx, i, ScType.Unknown)
            if len(_elemtup) == 0:
                continue
            elem = ctx.CreateNode(ScType.NodeConstStruct)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, mid_elem, elem)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, i)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _arccom)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els2)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _arc)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els3)
            for j in _elemtup:
                _arcsu = ""
                it_3su = ctx.Iterator3(i, ScType.EdgeAccessConstPosPerm, j)
                while it_3su.Next():
                    _arcsu = it_3su.Get(1)
                    break
                ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, j)
                ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _arcsu)
                it_3su1 = ctx.Iterator3(ScType.NodeConstRole, ScType.EdgeAccessConstPosPerm, _arcsu)
                while it_3su1.Next():
                    _els1 = it_3su1.Get(0)
                    _arcr = it_3su1.Get(1)
                    ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els1)
                    ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _arcr)
                    break
# find the structure of tuples with relation
    @staticmethod
    def decompositionTupleWithRelation(ctx, mid_elems, mid_elem):
        _class_tupr = Utils.getAllWithType(ctx, mid_elems, ScType.NodeConstTuple)
        for i in _class_tupr:
            it_3 = ctx.Iterator3(ScType.NodeConstNoRole, ScType.EdgeAccessConstPosPerm, i)
            if not it_3.Next():
                continue
            _els1 = it_3.Get(0)
            _arcpost = it_3.Get(1)
            _elemtup = Utils.getAllWithType(ctx, i, ScType.Unknown)
            if len(_elemtup) == 0:
                continue
            elem = ctx.CreateNode(ScType.NodeConstStruct)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, mid_elem, elem)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, i)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els1)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _arcpost)
            for j in _elemtup:
                it_3su = ctx.Iterator3(i, ScType.EdgeAccessConstPosPerm, j)
                while it_3su.Next():
                    _arc = it_3su.Get(1)
                    break
                ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, j)
                ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _arc)
                it_3su1 = ctx.Iterator3(ScType.NodeConstRole, ScType.EdgeAccessConstPosPerm, _arc)
                while it_3su1.Next():
                    _els2 = it_3su1.Get(0)
                    _arcr = it_3su1.Get(1)
                    ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els2)
                    ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _arcr)
                    break
# find the structure of edge
    @staticmethod
    def decompositionEdge(ctx, mid_elems, mid_elem):
        _class_edge = Utils.getAllWithType(ctx, mid_elems, ScType.EdgeUCommonConst)
        for i in _class_edge:
            _els1, _els2 = ctx.GetEdgeInfo(i)
            elem = ctx.CreateNode(ScType.NodeConstStruct)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, mid_elem, elem)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, i)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els1)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els2)
            it_3 = ctx.Iterator3(ScType.NodeConstNoRole, ScType.EdgeAccessConstPosPerm, i)
            while it_3.Next():
                _els3 = it_3.Get(0)
                _arcr = it_3.Get(1)
                ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _els3)
                ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, elem, _arcr)
                break
# substructureClassification
    @staticmethod
    def SubstructureClassification(ctx, allsst, classedge, classtup, classcomm,
                                   classretup, classpost5, classpost3):
        for i in allsst:
            it_edge = ctx.Iterator3(i, ScType.EdgeAccessConstPosPerm, ScType.EdgeUCommonConst)
            if it_edge.Next():
                classedge.append(i)
                continue
            it_common = ctx.Iterator3(i, ScType.EdgeAccessConstPosPerm, ScType.EdgeDCommonConst)
            if it_common.Next():
                els1, _ = ctx.GetEdgeInfo(it_common.Get(2))
                if ctx.GetElementType(els1) == ScType.NodeConstTuple:
                    classtup.append(i)
                    continue
                classcomm.append(i)
                continue
            it_retup = ctx.Iterator3(i, ScType.EdgeAccessConstPosPerm, ScType.NodeConstNoRole)
            it_retup1 = ctx.Iterator3(i, ScType.EdgeAccessConstPosPerm, ScType.NodeConstTuple)
            if it_retup.Next() and it_retup1.Next():
                classretup.append(i)
                continue
            it_post5 = ctx.Iterator3(i, ScType.EdgeAccessConstPosPerm, ScType.NodeConstRole)
            it_post51 = ctx.Iterator3(i, ScType.EdgeAccessConstPosPerm, ScType.EdgeAccessConstPosPerm)
            if it_post51.Next() and it_post5.Next():
                els1 = it_post5.Get(2)
                els2 = it_post51.Get(2)
                if ctx.HelperCheckEdge(els1, els2, ScType.EdgeAccessConstPosPerm):
                    classpost5.append(i)
                    continue
                els3, _ = ctx.GetEdgeInfo(els2)
                if els1 == els3:
                    classpost5.append(i)
                    continue
            classpost3.append(i)
# CommonSimilarityCalculation
    @staticmethod
    def CommonSimilarityCalculation(ctx, classcomm1, classcomm2, summa, mathstru):
        for elem in classcomm1:
            _comarc, _els3 = "", ""
            it_3 = ctx.Iterator3(elem, ScType.EdgeAccessConstPosPerm, ScType.EdgeDCommonConst)
            while it_3.Next():
                _comarc = it_3.Get(2)
            _els1, _els2 = ctx.GetEdgeInfo(_comarc)
            it_31 = ctx.Iterator3(elem, ScType.EdgeAccessConstPosPerm, ScType.NodeConstNoRole)
            while it_31.Next():
                _els3 = it_31.Get(2)

            print("standard answer:")
            Utils.printEl(ctx, _comarc)

            for elemcp in classcomm2:
                _els32 = ""
                it_3 = ctx.Iterator3(elemcp, ScType.EdgeAccessConstPosPerm, ScType.EdgeDCommonConst)
                while it_3.Next():
                    _comarc = it_3.Get(2)
                _els12, _els22 = ctx.GetEdgeInfo(_comarc)
                it_31 = ctx.Iterator3(elemcp, ScType.EdgeAccessConstPosPerm, ScType.NodeConstNoRole)
                while it_31.Next():
                    _els32 = it_31.Get(2)
                if _els1 != _els12:
                    if ctx.GetElementType(_els1) == ScType.NodeConst and ctx.GetElementType(_els12) == ScType.NodeConst:
                        pass
                    elif ctx.GetElementType(_els1).IsLink and ctx.GetElementType(_els12).IsLink:
                        data1 = ctx.GetLinkContent(_els1).AsString()
                        data2 = ctx.GetLinkContent(_els12).AsString()
                        if data1 != data2:
                            continue
                    else:
                        continue
                if _els2 != _els22:
                    if ctx.GetElementType(_els2) == ScType.NodeConst and ctx.GetElementType(_els22) == ScType.NodeConst:
                        pass
                    elif ctx.GetElementType(_els2).IsLink and ctx.GetElementType(_els22).IsLink:
                        data1 = ctx.GetLinkContent(_els2).AsString()
                        data2 = ctx.GetLinkContent(_els22).AsString()
                        if data1 != data2:
                            continue
                    else:
                        continue
                if _els3 != _els32:
                    continue
                summa+=1
                mathstru.append(elemcp)
                classcomm2.remove(elemcp)

                print("user answer:")
                Utils.printEl(ctx, _comarc)

                break
        return summa










