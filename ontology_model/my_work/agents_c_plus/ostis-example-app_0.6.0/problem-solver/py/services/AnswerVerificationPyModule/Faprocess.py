from sc import *
from Utils import Utils
class Faprocess:
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
                        keynode = ctx.HelperResolveSystemIdtf("nrel_system_identifier", ScType.NodeConstNoRole)
                        elemid1 = Utils.getFirstByInRelation(ctx, _els1, keynode)
                        elemid2 = Utils.getFirstByInRelation(ctx, _els12, keynode)
                        if elemid1.IsValid() or elemid2.IsValid():
                            continue
                    elif ctx.GetElementType(_els1).IsLink() and ctx.GetElementType(_els12).IsLink():
                        data1 = ctx.GetLinkContent(_els1).AsString()
                        data2 = ctx.GetLinkContent(_els12).AsString()
                        if data1 != data2:
                            continue
                    else:
                        continue
                if _els2 != _els22:
                    if ctx.GetElementType(_els2) == ScType.NodeConst and ctx.GetElementType(_els22) == ScType.NodeConst:
                        keynode = ctx.HelperResolveSystemIdtf("nrel_system_identifier", ScType.NodeConstNoRole)
                        elemid1 = Utils.getFirstByInRelation(ctx, _els2, keynode)
                        elemid2 = Utils.getFirstByInRelation(ctx, _els22, keynode)
                        if elemid1.IsValid() or elemid2.IsValid():
                            continue
                    elif ctx.GetElementType(_els2).IsLink() and ctx.GetElementType(_els22).IsLink():
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
                break
        return summa
    @staticmethod
    def Post5SimilarityCalculation(ctx, classpost51, classpost52, summa, mathstru):
        for elem in classpost51:
            _comarc, _els1, _els2,  _els3 = "", "", "", ""
            it_3 = ctx.Iterator3(elem, ScType.EdgeAccessConstPosPerm, ScType.NodeConstRole)
            while it_3.Next():
                _els3 = it_3.Get(2)
            it_31 = ctx.Iterator3(elem, ScType.EdgeAccessConstPosPerm, ScType.EdgeAccessConstPosPerm)
            while it_31.Next():
                _comarc = it_31.Get(2)
                _els1, _ = ctx.GetEdgeInfo(_comarc)
                if _els1 == _els3:
                    continue
                _, _els2 = ctx.GetEdgeInfo(_comarc)
                break
            for elemcp in classpost52:
                _els12, _els22, _els32 = "", "", ""
                it_3 = ctx.Iterator3(elemcp, ScType.EdgeAccessConstPosPerm, ScType.NodeConstRole)
                while it_3.Next():
                    _els32 = it_3.Get(2)
                it_31 = ctx.Iterator3(elemcp, ScType.EdgeAccessConstPosPerm, ScType.EdgeAccessConstPosPerm)
                while it_31.Next():
                    _comarc = it_31.Get(2)
                    _els12, _ = ctx.GetEdgeInfo(_comarc)
                    if _els12 == _els32:
                        continue
                    _, _els22 = ctx.GetEdgeInfo(_comarc)
                    break
                if _els1 != _els12:
                    if ctx.GetElementType(_els1) == ScType.NodeConst and ctx.GetElementType(_els12) == ScType.NodeConst:
                        keynode = ctx.HelperResolveSystemIdtf("nrel_system_identifier", ScType.NodeConstNoRole)
                        elemid1 = Utils.getFirstByInRelation(ctx, _els1, keynode)
                        elemid2 = Utils.getFirstByInRelation(ctx, _els12, keynode)
                        if elemid1.IsValid() or elemid2.IsValid():
                            continue
                    elif ctx.GetElementType(_els1).IsLink() and ctx.GetElementType(_els12).IsLink():
                        data1 = ctx.GetLinkContent(_els1).AsString()
                        data2 = ctx.GetLinkContent(_els12).AsString()
                        if data1 != data2:
                            continue
                    else:
                        continue
                if _els2 != _els22:
                    if ctx.GetElementType(_els2) == ScType.NodeConst and ctx.GetElementType(_els22) == ScType.NodeConst:
                        keynode = ctx.HelperResolveSystemIdtf("nrel_system_identifier", ScType.NodeConstNoRole)
                        elemid1 = Utils.getFirstByInRelation(ctx, _els2, keynode)
                        elemid2 = Utils.getFirstByInRelation(ctx, _els22, keynode)
                        if elemid1.IsValid() or elemid2.IsValid():
                            continue
                    elif ctx.GetElementType(_els2).IsLink() and ctx.GetElementType(_els22).IsLink():
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
                classpost52.remove(elemcp)
                break
        return summa
    @staticmethod
    def Post3SimilarityCalculation(ctx, classpost31, classpost32, summa, mathstru):
        for elem in classpost31:
            _comarc = ""
            it_3 = ctx.Iterator3(elem, ScType.EdgeAccessConstPosPerm, ScType.EdgeAccessConstPosPerm)
            while it_3.Next():
                _comarc = it_3.Get(2)
            _els1, _els2 = ctx.GetEdgeInfo(_comarc)
            for elemcp in classpost32:
                it_3 = ctx.Iterator3(elemcp, ScType.EdgeAccessConstPosPerm, ScType.EdgeAccessConstPosPerm)
                while it_3.Next():
                    _comarc = it_3.Get(2)
                _els12, _els22 = ctx.GetEdgeInfo(_comarc)
                if _els1 != _els12:
                    if ctx.GetElementType(_els1) == ScType.NodeConst and ctx.GetElementType(_els12) == ScType.NodeConst:
                        keynode = ctx.HelperResolveSystemIdtf("nrel_system_identifier", ScType.NodeConstNoRole)
                        elemid1 = Utils.getFirstByInRelation(ctx, _els1, keynode)
                        elemid2 = Utils.getFirstByInRelation(ctx, _els12, keynode)
                        if elemid1.IsValid() or elemid2.IsValid():
                            continue
                    elif ctx.GetElementType(_els1).IsLink() and ctx.GetElementType(_els12).IsLink():
                        data1 = ctx.GetLinkContent(_els1).AsString()
                        data2 = ctx.GetLinkContent(_els12).AsString()
                        if data1 != data2:
                            continue
                    else:
                        continue
                if _els2 != _els22:
                    if ctx.GetElementType(_els2) == ScType.NodeConst and ctx.GetElementType(_els22) == ScType.NodeConst:
                        keynode = ctx.HelperResolveSystemIdtf("nrel_system_identifier", ScType.NodeConstNoRole)
                        elemid1 = Utils.getFirstByInRelation(ctx, _els2, keynode)
                        elemid2 = Utils.getFirstByInRelation(ctx, _els22, keynode)
                        if elemid1.IsValid() or elemid2.IsValid():
                            continue
                    elif ctx.GetElementType(_els2).IsLink() and ctx.GetElementType(_els22).IsLink():
                        data1 = ctx.GetLinkContent(_els2).AsString()
                        data2 = ctx.GetLinkContent(_els22).AsString()
                        if data1 != data2:
                            continue
                    else:
                        continue
                summa+=1
                mathstru.append(elemcp)
                classpost32.remove(elemcp)
                break
        return summa
    @staticmethod
    def TupSimilarityCalculation(ctx, classtup1, classtup2, summa, mathstru):
        for elem in classtup1:
            _comarc, _els3 = "", ""
            it_3 = ctx.Iterator3(elem, ScType.EdgeAccessConstPosPerm, ScType.EdgeDCommonConst)
            while it_3.Next():
                _comarc = it_3.Get(2)
            _elstup1, _els1 = ctx.GetEdgeInfo(_comarc)
            it_3 = ctx.Iterator3(elem, ScType.EdgeAccessConstPosPerm, ScType.NodeConstNoRole)
            while it_3.Next():
                _els3 = it_3.Get(2)
            for elemcp in classtup2:
                _els32 = ""
                it_3 = ctx.Iterator3(elemcp, ScType.EdgeAccessConstPosPerm, ScType.EdgeDCommonConst)
                while it_3.Next():
                    _comarc = it_3.Get(2)
                _elstup12, _els12 = ctx.GetEdgeInfo(_comarc)
                it_3 = ctx.Iterator3(elemcp, ScType.EdgeAccessConstPosPerm, ScType.NodeConstNoRole)
                while it_3.Next():
                    _els32 = it_3.Get(2)
                if _els1 != _els12:
                    if ctx.GetElementType(_els1) == ScType.NodeConst and ctx.GetElementType(_els12) == ScType.NodeConst:
                        keynode = ctx.HelperResolveSystemIdtf("nrel_system_identifier", ScType.NodeConstNoRole)
                        elemid1 = Utils.getFirstByInRelation(ctx, _els1, keynode)
                        elemid2 = Utils.getFirstByInRelation(ctx, _els12, keynode)
                        if elemid1.IsValid() or elemid2.IsValid():
                            continue
                    elif ctx.GetElementType(_els1).IsLink() and ctx.GetElementType(_els12).IsLink():
                        data1 = ctx.GetLinkContent(_els1).AsString()
                        data2 = ctx.GetLinkContent(_els12).AsString()
                        if data1 != data2:
                            continue
                    else:
                        continue
                if _els3 != _els32:
                    continue
                _elstup1cp = Utils.getAllWithType(ctx, _elstup1, ScType.Unknown)
                _elstup12cp = Utils.getAllWithType(ctx, _elstup12, ScType.Unknown)
                if len(_elstup1cp) != len(_elstup12cp):
                    continue
                Step, Step1 = False, False
                for currelem in _elstup1cp:
                    it_5 = ctx.Iterator5(_elstup1, ScType.EdgeAccessConstPosPerm, currelem,
                                        ScType.EdgeAccessConstPosPerm, ScType.NodeConstRole)
                    if it_5.Next():
                        _els4 = it_5.Get(4)
                        if currelem not in _elstup12cp:
                            if ctx.GetElementType(currelem).IsLink():
                                _elstup12cplink = Utils.getAllWithType(ctx, _elstup12, ScType.Link)
                                for elemlink in _elstup12cplink:
                                    data1 = ctx.GetLinkContent(currelem).AsString()
                                    data2 = ctx.GetLinkContent(elemlink).AsString()
                                    if data1 != data2:
                                        continue
                                    it_51 = ctx.Iterator5(_elstup12, ScType.EdgeAccessConstPosPerm, elemlink,
                                                          ScType.EdgeAccessConstPosPerm, _els4)
                                    if not it_51.Next():
                                        continue
                                    Step1 = True
                                    break
                                if not Step1:
                                    Step = True
                                    break
                            else:
                                Step = True
                                break
                        if Step1:
                            continue
                        it_51 = ctx.Iterator5(_elstup12, ScType.EdgeAccessConstPosPerm, currelem,
                                              ScType.EdgeAccessConstPosPerm, _els4)
                        if not it_51.Next():
                            Step = True
                            break
                    else:
                        if currelem not in _elstup12cp:
                            if ctx.GetElementType(currelem).IsLink():
                                _elstup12cplink = Utils.getAllWithType(ctx, _elstup12, ScType.Link)
                                for elemlink in _elstup12cplink:
                                    data1 = ctx.GetLinkContent(currelem).AsString()
                                    data2 = ctx.GetLinkContent(elemlink).AsString()
                                    if data1 != data2:
                                        continue
                                    it_51 = ctx.Iterator5(_elstup12, ScType.EdgeAccessConstPosPerm, elemlink,
                                                          ScType.EdgeAccessConstPosPerm, ScType.NodeConstRole)
                                    if it_51.Next():
                                        continue
                                    Step1 = True
                                    break
                                if not Step1:
                                    Step = True
                                    break
                            else:
                                Step = True
                                break
                        if Step1:
                            continue
                        it_51 = ctx.Iterator5(_elstup12, ScType.EdgeAccessConstPosPerm, currelem,
                                              ScType.EdgeAccessConstPosPerm, ScType.NodeConstRole)
                        if it_51.Next():
                            Step = True
                            break
                if Step:
                    continue
                summa+=1
                mathstru.append(elemcp)
                classtup2.remove(elemcp)
                break
        return summa
    @staticmethod
    def ReTupSimilarityCalculation(ctx, classretup1, classretup2, summa, mathstru):
        for elem in classretup1:
            _els1, _elstup1 = "", ""
            res = Utils.getAllWithType(ctx, elem, ScType.EdgeAccessConstPosPerm)
            for _comarc in res:
                _els1, _elstup1 = ctx.GetEdgeInfo(_comarc)
                if not ctx.GetElementType(_elstup1) == ScType.NodeConstTuple:
                    continue
            for elemcp in classretup2:
                _els12, _elstup12 = "", ""
                res = Utils.getAllWithType(ctx, elemcp, ScType.EdgeAccessConstPosPerm)
                for _comarc in res:
                    _els12, _elstup12 = ctx.GetEdgeInfo(_comarc)
                    if not ctx.GetElementType(_elstup12) == ScType.NodeConstTuple:
                        continue
                if _els1 != _els12:
                    continue
                _elstup1cp = Utils.getAllWithType(ctx, _elstup1, ScType.Unknown)
                _elstup12cp = Utils.getAllWithType(ctx, _elstup12, ScType.Unknown)
                if len(_elstup1cp) != len(_elstup12cp):
                    continue
                Step, Step1 = False, False
                for currelem in _elstup1cp:
                    it_5 = ctx.Iterator5(_elstup1, ScType.EdgeAccessConstPosPerm, currelem,
                                         ScType.EdgeAccessConstPosPerm, ScType.NodeConstRole)
                    if it_5.Next():
                        _els4 = it_5.Get(4)
                        if currelem not in _elstup12cp:
                            if ctx.GetElementType(currelem).IsLink():
                                _elstup12cplink = Utils.getAllWithType(ctx, _elstup12, ScType.Link)
                                for elemlink in _elstup12cplink:
                                    data1 = ctx.GetLinkContent(currelem).AsString()
                                    data2 = ctx.GetLinkContent(elemlink).AsString()
                                    if data1 != data2:
                                        continue
                                    it_51 = ctx.Iterator5(_elstup12, ScType.EdgeAccessConstPosPerm, elemlink,
                                                          ScType.EdgeAccessConstPosPerm, _els4)
                                    if not it_51.Next():
                                        continue
                                    Step1 = True
                                    break
                                if not Step1:
                                    Step = True
                                    break
                            else:
                                Step = True
                                break
                        if Step1:
                            continue
                        it_51 = ctx.Iterator5(_elstup12, ScType.EdgeAccessConstPosPerm, currelem,
                                              ScType.EdgeAccessConstPosPerm, _els4)
                        if not it_51.Next():
                            Step = True
                            break
                    else:
                        if currelem not in _elstup12cp:
                            if ctx.GetElementType(currelem).IsLink():
                                _elstup12cplink = Utils.getAllWithType(ctx, _elstup12, ScType.Link)
                                for elemlink in _elstup12cplink:
                                    data1 = ctx.GetLinkContent(currelem).AsString()
                                    data2 = ctx.GetLinkContent(elemlink).AsString()
                                    if data1 != data2:
                                        continue
                                    it_51 = ctx.Iterator5(_elstup12, ScType.EdgeAccessConstPosPerm, elemlink,
                                                          ScType.EdgeAccessConstPosPerm, ScType.NodeConstRole)
                                    if it_51.Next():
                                        continue
                                    Step1 = True
                                    break
                                if not Step1:
                                    Step = True
                                    break
                            else:
                                Step = True
                                break
                        if Step1:
                            continue
                        it_51 = ctx.Iterator5(_elstup12, ScType.EdgeAccessConstPosPerm, currelem,
                                              ScType.EdgeAccessConstPosPerm, ScType.NodeConstRole)
                        if it_51.Next():
                            Step = True
                            break
                if Step:
                    continue
                summa+=1
                mathstru.append(elemcp)
                classretup2.remove(elemcp)
                break
        return summa
    @staticmethod
    def EdgeSimilarityCalculation(ctx, classedge1, classedge2, summa, mathstru):
        for elem in classedge1:
            _comarc, _els3 = "", ""
            it_3 = ctx.Iterator3(elem, ScType.EdgeAccessConstPosPerm, ScType.EdgeUCommonConst)
            while it_3.Next():
                _comarc = it_3.Get(2)
            _els1, _els2 = ctx.GetEdgeInfo(_comarc)
            it_3 = ctx.Iterator3(elem, ScType.EdgeAccessConstPosPerm, ScType.NodeConstNoRole)
            while it_3.Next():
                _els3 = it_3.Get(2)
            Step, Step1 = False, False
            for elemcp in classedge2:
                _els32 = ""
                it_3 = ctx.Iterator3(elemcp, ScType.EdgeAccessConstPosPerm, ScType.EdgeUCommonConst)
                while it_3.Next():
                    _comarc = it_3.Get(2)
                _els12, _els22 = ctx.GetEdgeInfo(_comarc)
                it_3 = ctx.Iterator3(elemcp, ScType.EdgeAccessConstPosPerm, ScType.NodeConstNoRole)
                while it_3.Next():
                    _els32 = it_3.Get(2)
                if _els3 != _els32:
                    continue
                if _els1 != _els12:
                    if _els1 != _els22:
                        if not ctx.GetElementType(_els1).IsLink():
                            continue
                        if ctx.GetElementType(_els12).IsLink():
                            data1 = ctx.GetLinkContent(_els1).AsString()
                            data2 = ctx.GetLinkContent(_els12).AsString()
                            if data1 == data2:
                                Step1 = True
                        if not Step1:
                            if not ctx.GetElementType(_els22).IsLink():
                                continue
                            data1 = ctx.GetLinkContent(_els1).AsString()
                            data2 = ctx.GetLinkContent(_els22).AsString()
                            if data1 != data2:
                                continue
                    if not Step1:
                        if _els2 != _els12:
                            if not ctx.GetElementType(_els2).IsLink():
                                continue
                            if not ctx.GetElementType(_els12).IsLink():
                                continue
                            data1 = ctx.GetLinkContent(_els2).AsString()
                            data2 = ctx.GetLinkContent(_els12).AsString()
                            if data1 != data2:
                                continue
                        Step = True
                if not Step:
                    if _els2 != _els22:
                        if not ctx.GetElementType(_els2).IsLink():
                            continue
                        if not ctx.GetElementType(_els22).IsLink():
                            continue
                        data1 = ctx.GetLinkContent(_els2).AsString()
                        data2 = ctx.GetLinkContent(_els22).AsString()
                        if data1 != data2:
                            continue
                summa+=1
                mathstru.append(elemcp)
                classedge2.remove(elemcp)
                break
        return summa
    @staticmethod
    def SimilarityCalculation(summa, sumcand, sumsta):
        Psc = summa*1.0/sumcand
        Rsc = summa*1.0/sumsta
        Fsc = (2*Psc*Rsc*1.0)/(Psc+Rsc)
        print("Precision:%f" % Psc)
        print("Recall:%f" % Rsc)
        print("Similarity:%f" % Fsc)
        return Fsc
    @staticmethod
    def MatchStructureStatistics(ctx, mathstru, allsst2, answer, param):
        for i in mathstru:
            for j in allsst2:
                if i == j:
                    allsst2.remove(i)
                    break
        mismathstru = allsst2
        if len(mismathstru) == 0:
            print("The user answers are all correct:")
        elif len(mathstru) == 0:
            print("The user answers are all incorrect:")
        else:
            print("The user answer is partially correct:")
            node = ctx.CreateNode(ScType.NodeConst)
            for i in mismathstru:
                ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, node, i)
            arc = ctx.CreateEdge(ScType.EdgeDCommonConst, param, node)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, arc)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, node)
            keynode = ctx.HelperResolveSystemIdtf("nrel_incorrect_answer", ScType.NodeConstNoRole)
            arc1= ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, keynode, arc)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, arc1)
            ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, keynode)
    @staticmethod
    def GenerateSimilarityLink(ctx, answer, param, Fsc):
        link = ctx.CreateLink()
        arc = ctx.CreateEdge(ScType.EdgeDCommonConst, param, link)
        keynode = ctx.HelperResolveSystemIdtf("nrel_coefficient", ScType.NodeConstNoRole)
        arc1 = ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, keynode, arc)
        ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, arc)
        ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, link)
        ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, arc1)
        ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, answer, keynode)
        ctx.SetLinkContent(link, str(Fsc))




