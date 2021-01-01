from sc import *

class Utils:
    @staticmethod
    def getAllWithType(ctx, set, scType):
        elementList = []
        iterator3 = ctx.Iterator3(set, ScType.EdgeAccessConstPosPerm, scType)
        while iterator3.Next():
            elementList.append(iterator3.Get(2))
        return elementList
    @staticmethod
    def printEl(ctx, element):
        type = ctx.GetElementType(element)
        if type.IsNode():
            data = ctx.HelperGetSystemIdtf(element)
            print(data)
        elif type.IsLink():
            data = ctx.GetLinkContent(element).AsString()
            print(data)
        else:
            elem1, elem2 = ctx.GetEdgeInfo(element)
            Utils.printEl(ctx, elem1)
            print("->")
            Utils.printEl(ctx, elem2)
            print("   ")
    @staticmethod
    def finishAgentWork(ctx, questionNode, answer):
        result_arc = ctx.CreateEdge(ScType.EdgeDCommonConst, questionNode, answer)
        result_answer = ctx.HelperResolveSystemIdtf("nrel_answer", ScType.NodeConstNoRole)
        ctx.CreateEdge(ScType.EdgeAccessConstPosPerm, result_answer, result_arc)