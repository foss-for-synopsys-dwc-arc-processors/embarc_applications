#include "NNLayer.h"
#include "W0.h"
#include "B0.h" 
#include "W1.h"
#include "B1.h"
//#include "W2.h"
//#include "B2.h"
//extern xdata NNLayer gMyCalculatorModel[LAYER_TOTAL];
//extern xdata double gLayer0OutVal[LAYER_0_OUTDIM],gLayer1OutVal[LAYER_1_OUTDIM];

extern NNLayer gMyCalculatorModel[LAYER_TOTAL];
extern double gLayer0OutVal[LAYER_0_OUTDIM],gLayer1OutVal[LAYER_1_OUTDIM];//,gLayer2OutVal[LAYER_2_OUTDIM];
  
/*
*   训练好的模型参数
*/


 
/*
*   return arr[r][c]
*/
double _getWeight(const double* arr,int r,int rNum,int c)      
{
    return *(arr + r*rNum + c);
}
 
/*
*   NNLayer初始化
*   绑定每层的权重矩阵、偏置矩阵
*   设置每层的输入维度，输出维度
*   绑定每层的输出暂存数组
*/
void NNLayerInit()
{
    gMyCalculatorModel[0].weight = W0[0];
    gMyCalculatorModel[0].bais = B0;
    gMyCalculatorModel[0].inDim = LAYER_0_INDIM;
    gMyCalculatorModel[0].outDim = LAYER_0_OUTDIM;
    gMyCalculatorModel[0].outVal = gLayer0OutVal;
 
    gMyCalculatorModel[1].weight = W1[0];
    gMyCalculatorModel[1].bais =B1;
    gMyCalculatorModel[1].inDim = LAYER_1_INDIM;
    gMyCalculatorModel[1].outDim = LAYER_1_OUTDIM;
    gMyCalculatorModel[1].outVal = gLayer1OutVal;

//    gMyCalculatorModel[2].weight = W2;
//    gMyCalculatorModel[2].bais =B2;
//    gMyCalculatorModel[2].inDim = LAYER_2_INDIM;
//    gMyCalculatorModel[2].outDim = LAYER_2_OUTDIM;
//    gMyCalculatorModel[2].outVal = gLayer2OutVal;

}
 
/*
*   NNLayer进行预测
*   model - NNLayer数组
*   x - 输入向量，数目符合LAYER_0_INDIM
*/
void NNLayerPredict(NNLayer* model, double* x)
{
    int _indexW,_indexX,_indexLayer;
 
    for(_indexLayer = 0 ; _indexLayer < LAYER_TOTAL ; _indexLayer++)
    {
        for(_indexW = 0 ; _indexW < model[_indexLayer].outDim ; _indexW++)
        {
            for(_indexX = 0 ; _indexX < model[_indexLayer].inDim ; _indexX++)
            {
                if(_indexLayer == 0)
                {
                    model[_indexLayer].outVal[_indexW] += x[_indexX] * _getWeight(model[_indexLayer].weight,_indexW,model[_indexLayer].inDim,_indexX);
                }
                else
                {
                    model[_indexLayer].outVal[_indexW] += model[_indexLayer - 1].outVal[_indexX] * _getWeight(model[_indexLayer].weight,_indexW,model[_indexLayer].inDim,_indexX);
                
//                	printf("%f * %f\n",model[_indexLayer - 1].outVal[_indexX],_getWeight(model[_indexLayer].weight,_indexW,model[_indexLayer].inDim,_indexX));
				}
                
                
            }
            model[_indexLayer].outVal[_indexW] += model[_indexLayer].bais[_indexW];
            
			if(_indexLayer == 0)
                model[_indexLayer].outVal[_indexW] = (model[_indexLayer].outVal[_indexW]<0)?0:model[_indexLayer].outVal[_indexW];
                
        }
    }
 
}
