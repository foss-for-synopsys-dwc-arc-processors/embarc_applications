#include "NNLayer.h"
#include "W0.h"
#include "B0.h" 
#include "W1.h"
#include "B1.h"
#include "math.h"
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
 
    gMyCalculatorModel[1].weight = W1[1];
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

double NNLayerloss(NNLayer* model,double* target)
{
    int _indexO;
    double loss = 0,temp = 0;
    for(_indexO = 0 ; _indexO < model[LAYER_TOTAL-1].outDim ; _indexO++)
    {
        temp = target[_indexO]-model[LAYER_TOTAL-1].outVal[_indexO];
        loss += temp * temp;
    }
    return loss/2;
}
void cal_dp_outlayer(NNLayer* model,double* target,double (*dp_w1)[LAYER_0_OUTDIM],double* dp_b1)
{
    int _indexW,_indexO;
	//double temp1 = 0, temp2 = 0, temp3 = 0;
	for(_indexO = 0 ; _indexO < model[LAYER_TOTAL-1].outDim ; _indexO++)
    {
		//temp1 = target[_indexO];
		//temp2 = model[LAYER_TOTAL - 1].outVal[_indexO];
		//dp_b1[_indexO] = -(temp1 - temp2);
		dp_b1[_indexO] = -(target[_indexO] - model[LAYER_TOTAL - 1].outVal[_indexO]);
        for(_indexW = 0 ; _indexW < model[LAYER_TOTAL-2].outDim ; _indexW++)
        {
			//temp3 = model[LAYER_TOTAL - 2].outVal[_indexW];
			//dp_w1[_indexO][_indexW] = dp_b1[_indexO]*temp3;
			dp_w1[_indexO][_indexW] = dp_b1[_indexO] * model[LAYER_TOTAL - 2].outVal[_indexW] ;
			//printf(" dp_w1[%d][%d]=%.2f\t dp_b1[%d]=%.2f\n", _indexO, _indexW, dp_w1[_indexO][_indexW], _indexO, dp_b1[_indexO]);
        }
    }
}
void cal_dp_hidlayer(NNLayer* model,double* x,double* target, double(*dp_w1)[LAYER_0_OUTDIM],double* dp_b1, double(*dp_w0)[LAYER_0_INDIM],double* dp_b0)
{
    int _indexW,_indexO,_indexX;
	double temp ;
	for (_indexX = 0; _indexX < model[LAYER_TOTAL - 2].inDim; _indexX++)
	{
		for (_indexW = 0; _indexW < model[LAYER_TOTAL - 2].outDim; _indexW++)
		{
			temp = 0;
			for (_indexO = 0; _indexO < model[LAYER_TOTAL - 1].outDim; _indexO++)
			{
				//temp += (-(target[_indexO])-model[LAYER_TOTAL-1].outVal[_indexO])*(model[LAYER_TOTAL-1].outVal[_indexO]>0)*_getWeight(model[LAYER_TOTAL-1].weight,_indexO,model[LAYER_TOTAL-1].inDim,_indexW);
				temp += (-(target[_indexO]) - model[LAYER_TOTAL - 1].outVal[_indexO])*_getWeight(model[LAYER_TOTAL - 1].weight, _indexO, model[LAYER_TOTAL - 1].inDim, _indexW);
			}
			//*((double*)dp_w0 + 20 * _indexW + _indexX) = temp*(model[LAYER_TOTAL - 2].outVal[_indexW] > 0)*(x[_indexX]);
			dp_w0[_indexW][_indexX] = temp*(model[LAYER_TOTAL - 2].outVal[_indexW] > 0)*(x[_indexX]);
			dp_b0[_indexW] = temp*(model[LAYER_TOTAL - 2].outVal[_indexW] > 0);
			//printf(" dp_w0[%d][%d]=%.2f\t dp_b0[%d][%d]=%.2f\n", _indexW, _indexX, dp_w0[_indexW][_indexX], _indexW, _indexX, dp_b0[_indexW]);
		}
	}
}

void w_b_upgrate(NNLayer* model, double(*dp_w1)[LAYER_0_OUTDIM], double* dp_b1, double(*dp_w0)[LAYER_0_INDIM], double* dp_b0)
{
    int _indexW,_indexX;
    for(_indexW = 0 ; _indexW < model[0].outDim ; _indexW++)
    {
        for(_indexX = 0 ; _indexX < model[0].inDim ; _indexX++)
        {
            //_getWeight(model[0].weight,_indexW,model[0].inDim,_indexX) =temp - LR *  dp_w0[_indexW][_indexX];

			*(model[0].weight + _indexW*model[0].inDim +_indexX) -= LR *  dp_w0[_indexW][_indexX];
			//model[0].weight[_indexW*model[0].inDim + _indexX] -= LR *  dp_w0[_indexW][_indexX];
			//model[0].weight[_indexW][_indexX] -= LR *  dp_w0[_indexW][_indexX];

            model[0].bais[_indexW] -= LR *  dp_b0[_indexW];
        }
    }
    for(_indexW = 0 ; _indexW < model[1].outDim ; _indexW++)
    {
        for(_indexX = 0 ; _indexX < model[1].inDim ; _indexX++)
        {
            //model[0].weight
			//_getWeight(model[1].weight,_indexW,model[1].inDim,_indexX) -= LR *  dp_w1[_indexW][_indexX];
            *(model[1].weight + _indexW*model[1].inDim +_indexX) -= LR *  dp_w1[_indexW][_indexX];
			//*(model[1].weight + _indexW*model[1].inDim + _indexX) -= LR * (*((double*)dp_w1 + 32 * _indexW + _indexX));

            model[1].bais[_indexW] -= LR *  dp_b1[_indexW];
        }
    }
}

