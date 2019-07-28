#include "cnn.h"
#include "embARC.h"
#include "embARC_debug.h"
#include <stdio.h>
extern char imge[imge_len][imge_wid]; 

char cnn(void)
{
	float c1_max1 = 0;
	float c1_com1 = 0;
	float c2_max1 = 0;
	float c2_com1 = 0;
	float c3_max1 = 0;
	float c3_com1 = 0;
	float l4_com = 0;
	float l5_com = 0;
	float so_com = 0;
	float sum = 0;
	char result = 4;
	float max = 0;
	unsigned char l = 0;
	
	for (unsigned char i = 0; i < (imge_len + 2); i++)
	{
		for (unsigned char j = 0; j < (imge_wid + 2); j++)
		{
			if (i == 0 || j == 0 || i == imge_len + 1 || j == imge_wid + 1)
				imge1[i][j] = 0;
			else
				imge1[i][j] = imge[i - 1][j - 1];
		}
	}
	//c1+p1
	for (unsigned char i = 0; i < conv1_num; i++) {
		for (unsigned char x = 0;x < (imge_len / 2); x++)
			for (unsigned char y = 0; y < (imge_wid / 2); y++)
			{
				c1_com1 = 0;
				c1_max1 = 0;
				for (unsigned char a = 0; a < 3; a++)
					for (unsigned char b = 0; b < 3; b++)
					{
						c1_max1 = c1_max1 + imge1[2*x + a][2*y + b] * c1_weight[a][b][i];
					}
				for (unsigned char a = 0; a < 3; a++)
					for (unsigned char b = 0; b < 3; b++)
					{
						c1_com1 = c1_com1 + imge1[2*x + a + 1][2*y + b] * c1_weight[a][b][i];
					}
				if (c1_com1 > c1_max1)
					c1_max1 = c1_com1;
				c1_com1 = 0;
				for (unsigned char a = 0; a < 3; a++)
					for (unsigned char b = 0; b < 3; b++)
					{
						c1_com1 = c1_com1 + imge1[2 * x + a][2 * y + b + 1] * c1_weight[a][b][i];
					}
				if (c1_com1 > c1_max1)
					c1_max1 = c1_com1;
				c1_com1 = 0;
				for (unsigned char a = 0; a < 3; a++)
					for (unsigned char b = 0; b < 3; b++)
					{
						c1_com1 = c1_com1 + imge1[2 * x + a + 1 ][2 * y + b + 1] * c1_weight[a][b][i];
					}
				if (c1_com1 > c1_max1)
					c1_max1 = c1_com1;
				c1_com1 = 0;

				p1_data[x][y][i] = c1_max1 + c1_biases[i];
				if (p1_data[x][y][i] < 0)
					p1_data[x][y][i] = 0;
			}	
	}
	//c2+p2
	for (unsigned char i = 0; i < conv2_num; i++)
	{
		for (unsigned char x = 0; x < p2_o_len; x++)
			for (unsigned char y = 0; y < p2_o_wid; y++)
			{
				c2_max1 = 0;
				c2_com1 = 0;
				for (unsigned char a = 0; a < 3; a++)
					for (unsigned char b = 0; b < 3; b++)
					{ 
						for (unsigned char c = 0; c < conv1_num; c++)
						{
							if (((2 * x + a - 1) < 0) || ((2 * y + b - 1) < 0))
								c2_max1 = c2_max1;
							else
                c2_max1 = c2_max1 + p1_data[2 * x + a - 1][2 * y + b - 1][c] * c2_weight[a][b][i][c];
						}
					}
				for (unsigned char a = 0; a < 3; a++)
					for (unsigned char b = 0; b < 3; b++)
					{
						for (unsigned char c = 0; c < conv1_num; c++)
						{
							if (((2 * x + a ) == p1_o_len) || ((2 * y + b - 1) < 0))
								c2_com1 = c2_com1;
							else
								c2_com1 = c2_com1 + p1_data[2 * x + a][2 * y + b - 1][c] * c2_weight[a][b][i][c];
						}
					}
				if (c2_com1 > c2_max1)
					c2_max1 = c2_com1;
				c2_com1 = 0;
				for (unsigned char a = 0; a < 3; a++)
					for (unsigned char b = 0; b < 3; b++)
					{
						for (unsigned char c = 0; c < conv1_num; c++)
						{
							if (((2 * x + a - 1) < 0) || ((2 * y + b) == p1_o_len))
								c2_com1 = c2_com1;
							else
								c2_com1 = c2_com1 + p1_data[2 * x + a -1][2 * y + b][c] * c2_weight[a][b][i][c];
						}
					}
				if (c2_com1 > c2_max1)
					c2_max1 = c2_com1;
				c2_com1 = 0;
				for (unsigned char a = 0; a < 3; a++)
					for (unsigned char b = 0; b < 3; b++)
					{
						for (unsigned char c = 0; c < conv1_num; c++)
						{
							if (((2 * x + a) == p1_o_len) || ((2 * y + b) == p1_o_len))
								c2_com1 = c2_com1;
							else
								c2_com1 = c2_com1 + p1_data[2 * x + a][2 * y + b][c] * c2_weight[a][b][i][c];
						}
					}
				if (c2_com1 > c2_max1)
					c2_max1 = c2_com1;
				c2_com1 = 0;
				p2_data[x][y][i] = c2_max1 + c2_biases[i];
				if (p2_data[x][y][i] <= 0)
					p2_data[x][y][i] = 0;
			}
	}	
	//c3+p3
	for (unsigned char i = 0; i < conv3_num; i++)
	{
		for (unsigned char x = 0; x < p3_o_len; x++)
			for (unsigned char y = 0; y < p3_o_wid; y++)
			{
				c3_max1 = 0;
				c3_com1 = 0;
				for (unsigned char a = 0; a < 3; a++)
					for (unsigned char b = 0; b < 3; b++)
					{
						for (unsigned char c = 0; c < conv2_num; c++)
						{
							if (((2 * x + a - 1) < 0) || ((2 * y + b - 1) < 0))
								c3_max1 = c3_max1;
							else
								c3_max1 = c3_max1 + p2_data[2 * x + a - 1][2 * y + b - 1][c] * c3_weight[a][b][i][c];
						}
					}
				for (unsigned char a = 0; a < 3; a++)
					for (unsigned char b = 0; b < 3; b++)
					{
						for (unsigned char c = 0; c < conv2_num; c++)
						{
							if (((2 * x + a) == p2_o_len) || ((2 * y + b - 1) < 0))
								c3_com1 = c3_com1;
							else
								c3_com1 = c3_com1 + p2_data[2 * x + a][2 * y + b - 1][c] * c3_weight[a][b][i][c];
						}
					}
				if (c3_com1 > c3_max1)
					c3_max1 = c3_com1;
				c3_com1 = 0;
				for (unsigned char a = 0; a < 3; a++)
					for (unsigned char b = 0; b < 3; b++)
					{
						for (unsigned char c = 0; c < conv2_num; c++)
						{
							if (((2 * x + a - 1) < 0) || ((2 * y + b) == p2_o_len))
								c3_com1 = c3_com1;
							else
								c3_com1 = c3_com1 + p2_data[2 * x + a - 1][2 * y + b][c] * c3_weight[a][b][i][c];
						}
					}
				if (c3_com1 > c3_max1)
					c3_max1 = c3_com1;
				c3_com1 = 0;
				for (unsigned char a = 0; a < 3; a++)
					for (unsigned char b = 0; b < 3; b++)
					{
						for (unsigned char c = 0; c < conv2_num; c++)
						{
							if (((2 * x + a) == p2_o_len) || ((2 * y + b) == p2_o_len))
								c3_com1 = c3_com1;
							else
								c3_com1 = c3_com1 + p2_data[2 * x + a][2 * y + b][c] * c3_weight[a][b][i][c];
						}
					}
				if (c3_com1 > c3_max1)
					c3_max1 = c3_com1;
				c3_com1 = 0;
				p3_data[x][y][i] = c3_max1 + c3_biases[i];
				if (p3_data[x][y][i] <= 0)
					p3_data[x][y][i] = 0;
			}
	}	
	//l4
	for (unsigned char i = 0; i < l4out_num; i++)
	{
		l4_com = 0;
		l = 0;
		for (unsigned char a = 0; a < conv3_num; a++)
			for (unsigned char x = 0; x < p3_o_len; x++)
				for (unsigned char y = 0; y < p3_o_wid; y++) {
                      l4_com = l4_com + p3_data[x][y][a] * l4_weight[l][i];
					  l++;
				}
					
		l4_data[i] = l4_com + l4_biases[i];
		if (l4_data[i] <= 0)
			l4_data[i] = 0;
	}	
	//l5
	for (unsigned char i = 0; i < l5out_num; i++) 
	{
		l5_com = 0;
        for (unsigned char j = 0; j < l4out_num; j++)
			l5_com = l5_com + l4_data[j] * l5_weight[j][i];
		l5_data[i] = l5_com + l5_biases[i];
		if (l5_data[i] <= 0)
			l5_data[i] = 0;
	}
	//so
	sum = 0;
	for (unsigned char i = 0; i < so_num; i++)
	{
		so_com = 0;
		for (unsigned char j = 0; j < l5out_num; j++)
			so_com = so_com + l5_data[j] * so_weight[j][i];
		so_data[i] = so_com + so_biases[i];
		printf("so_biases is :%f\n",so_data[i]);//(int)(so_data[i] * 1000));
		sum = sum + so_data[i];
	}
	result = 4;
	max = so_data[0];
	for (unsigned char i = 0; i < so_num; i++)
	{
		if (max <= so_data[i])
		{
			max = so_data[i];
			result = i;
		}
	}	
	if((3*max - so_data[0] - so_data[1]- so_data[0])<=0.3)
		result = 4;
	return result;	
}