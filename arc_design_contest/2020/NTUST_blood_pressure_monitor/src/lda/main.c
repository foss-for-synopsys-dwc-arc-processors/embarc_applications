#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LEN(x) sizeof(x) / sizeof(x[0])


typedef struct matrix_s matrix_s;
struct matrix_s {
    size_t row;
    size_t col;
};

int m_add(float**,float**,float**,matrix_s,matrix_s,matrix_s);
int m_sub(float**,float**,float**,matrix_s,matrix_s,matrix_s);
int m_mul(float**,float**,float**,matrix_s,matrix_s,matrix_s);
int m_inv(float**,float**,matrix_s,matrix_s);
int m_clr(float**,matrix_s);
int m_cpy(float**,float**,matrix_s,matrix_s);
int m_init(float ***,matrix_s);
int m_free(float ***,matrix_s);
int m_print(float**,matrix_s);
int m_qr(float**,float**,float**,matrix_s,matrix_s,matrix_s);
int eig_val_sort(float**,matrix_s,int);
int eigVector(float**,float**,float**,matrix_s,matrix_s,matrix_s);
int Eig(float**,float**,float**,matrix_s,matrix_s,matrix_s,int);

int  main(int argc,char** argv){
	printf("asd");
    FILE* pFile=fopen("ECG_DATA.out","r");
    if (pFile==NULL){
        printf("找不到 ECG_DATA.out");
        return -1;
    }
    FILE* plable=fopen("lable.out","r");
    if (pFile==NULL){
        printf("找不到 lable.out");
        return -1;
    }
    int n,m,n_l;
    n=0;
    m=260;
    char line[10000];
    while(fscanf(pFile,"%[^\n]",line)!=EOF){ 
        fgetc(pFile);
        n++;
    }
    printf("n=%d\n",n);
    n_l=0;
    while(fscanf(plable,"%[^\n]",line)!=EOF){ 
        fgetc(plable);
        n_l++;
    }
    if(n != n_l){
        printf("size of data & lable is not equal");
        return -1;
    }
    int *d_lable = (int *) malloc(n_l * sizeof(int));
    if (d_lable==NULL){
        printf("malloc d_lable 失敗");
        return -1;
    }
    fseek(plable, 0, SEEK_SET);
    int temp=999;
    int l_count=0;
    int unique_lable[50];
    
    for(int i=0;i<n_l;i++){
        fscanf(plable,"%d",&d_lable[i]);
        if(temp != d_lable[i]){
            temp=d_lable[i];
            unique_lable[l_count]=temp;
            l_count++;
            if(l_count>50){
                printf("unique_lable size > 50");
                return -1;
            }
        }
        //printf("%d  \n",d_lable[i]);
        fscanf(plable,"\n");
    }
    
       
    //float* data;
    //data=malloc(n*m*sizeof(float));
    /*float **data = (float **) malloc(m * sizeof(float *)); //r
    for (int i=0;i<m;i++)
        data[i]=(float *) malloc(n*sizeof(float)); //c
    if (data==NULL){
        printf("malloc 失敗");
        return -1;
    }*/
  /*  matrix_s data_s={m,n};
    float **data;
    m_init(&data,data_s);
    fseek(pFile, 0, SEEK_SET);
    for(int j=0;j<n;j++){
        for(int i=0;i<m;i++){
            fscanf(pFile,"%lf",&data[i][j]);
            //printf("%lf  ",data[i][j]);
        }
        fscanf(pFile,"\n");
        //printf("\n");
    }
    fclose(pFile);
    fclose(plable);*/
    //int n,m,n_l;
    matrix_s data_s={m,n};
    float **data;
    m_init(&data,data_s);
    for(int j=0;j<n;j++){
        for(int i=0;i<m;i++){
            data[i][j]=rand()%100/100;
            printf("%lf  ",data[i][j]);
        }
        //fscanf(pFile,"\n");
        //printf("\n");
    }
    /*float **mean_v = (float **) malloc(m * sizeof(float*));
    for (int i=0;i<m;i++)
        mean_v[i]=(float *) malloc(1*sizeof(float));
    if (mean_v==NULL){
        printf("malloc mean_v 失敗");
        return -1;
    }*/
    float **mean_v;
    matrix_s mean_v_s={m,1};
    m_init(&mean_v,mean_v_s);
    float sum=0;
    //printf("mean_v=[\n");
    for (int i=0;i<m;i++){
        sum=0;
        for(int j=0;j<n;j++){
            sum+=data[i][j];
        }
        mean_v[i][0]=sum/n;
        //printf("%lf\n",mean_v[i]);
    }
    //printf("]\n");

    /*float **mean_class_v = (float **) malloc(m * sizeof(float *));
    for (int i=0;i<m;i++)
        mean_class_v[i]=(float *) malloc(l_count*sizeof(float));
    if (mean_class_v==NULL){
        printf("malloc mean_class_v 失敗");
        return -1;
    }*/
    float **mean_class_v;
    matrix_s mean_class_v_s={m,l_count};
    m_init(&mean_class_v,mean_class_v_s);

    int class_size[50];
    for(int k=0;k<l_count;k++){
        for (int i=0;i<m;i++){
            sum=0;
            class_size[k]=0;
            for(int j=0;j<n;j++){
                if(d_lable[j]==unique_lable[k]){
                    sum+=data[i][j];
                    class_size[k]++;
                }
            }
            mean_class_v[i][k]=sum/class_size[k];
        }
    }

    /*float **col_v = (float **) malloc(m * sizeof(float*));
    for (int i=0;i<m;i++)
        col_v[i]=(float *) malloc(1*sizeof(float));
    if (col_v==NULL){
        printf("malloc col_v 失敗");
        return -1;
    }*/
    float **col_v;
    matrix_s col_v_s={m,1};
    m_init(&col_v,col_v_s);

    /*float **row_v = (float **) malloc(1 * sizeof(float*));
    for (int i=0;i<m;i++)
        row_v[i]=(float *) malloc(m*sizeof(float));
    if (row_v==NULL){
        printf("malloc row_v 失敗");
        return -1;
    }*/
    float **row_v;
    matrix_s row_v_s={1,m};
    m_init(&row_v,row_v_s);

    /*float **sw = (float **) malloc(m * sizeof(float*));
    for (int i=0;i<m;i++)
        sw[i]=(float *) malloc(m*sizeof(float));
    if (sw==NULL){
        printf("malloc sw 失敗");
        return -1;
    }*/
    float **sw;
    matrix_s sw_s={m,m};
    m_init(&sw,sw_s);

    /*float **sk = (float **) malloc(m * sizeof(float*));
    for (int i=0;i<m;i++)
        sk[i]=(float *) malloc(m*sizeof(float));
    if (sk==NULL){
        printf("malloc sk 失敗");
        return -1;
    }*/
    float **sk;
    matrix_s sk_s={m,m};
    m_init(&sk,sk_s);

    /*for(int i=0;i<m;i++){
        for(int j=0;j<m;j++){
            sw[i][j]=0;
        }
    }*/
    m_clr(sw,sw_s);

    for(int k=0;k<l_count;k++){
        for(int j=0;j<n;j++){
            if(d_lable[j]==unique_lable[k]){
                for (int i=0;i<m;i++){
                    col_v[i][0]=data[i][j]-mean_class_v[i][k];
                    row_v[0][i]=col_v[i][0];
                }
                m_clr(sk,sk_s);
                m_mul(col_v,row_v,sk,col_v_s,row_v_s,sk_s);
                m_add(sw,sk,sw,sw_s,sk_s,sw_s); 
            }
        }
    }

    /*float **sb = (float **) malloc(m * sizeof(float*));
    for (int i=0;i<m;i++)
        sb[i]=(float *) malloc(m*sizeof(float));
    if (sb==NULL){
        printf("malloc sb 失敗");
        return -1;
    }*/
    float **sb;
    matrix_s sb_s={m,m};
    m_init(&sb,sb_s);

    /*for(int i=0;i<m;i++){
        for(int j=0;j<m;j++){
            sb[i][j]=0;
        }
    }*/

    m_clr(sb,sb_s);

    for(int k=0;k<l_count;k++){
        for (int i=0;i<m;i++){
            col_v[i][0]=mean_class_v[i][k]-mean_v[i][0];
            row_v[0][i]=col_v[i][0];
        }
        m_clr(sk,sk_s);
        m_mul(col_v,row_v,sk,col_v_s,row_v_s,sk_s);
        for(int i=0;i<m;i++){
            for(int j=0;j<m;j++){
                sk[i][j]=sk[i][j]*class_size[k];
            }
        }
        m_add(sb,sk,sb,sb_s,sk_s,sb_s); 
    }

    /*float **inv_sw = (float **) malloc(m * sizeof(float*));
    for (int i=0;i<m;i++)
        inv_sw[i]=(float *) malloc(m*sizeof(float));
    if (inv_sw==NULL){
        printf("malloc inv_sw 失敗");
        return -1;
    }*/
    float **inv_sw;
    matrix_s inv_sw_s={m,m};
    m_init(&inv_sw,inv_sw_s);

    /*float **ss = (float **) malloc(m * sizeof(float*));
    for (int i=0;i<m;i++)
        ss[i]=(float *) malloc(m*sizeof(float));
    if (ss==NULL){
        printf("malloc ss 失敗");
        return -1;
    }*/
    float **ss;
    matrix_s ss_s={m,m};
    m_init(&ss,ss_s);

    /*float **sw_t = (float **) malloc(m * sizeof(float*));
    for (int i=0;i<m;i++)
        sw_t[i]=(float *) malloc(m*sizeof(float));
    if (sw_t==NULL){
        printf("malloc ss 失敗");
        return -1;
    }*/
    float **sw_t;
    matrix_s sw_t_s={m,m};
    m_init(&sw_t,sw_t_s);

    if(m_inv(sw,inv_sw,sw_s,inv_sw_s) == 0){
        m_clr(ss,ss_s);
        m_mul(inv_sw,sb,ss,inv_sw_s,sb_s,ss_s);
    }
    else{
        printf("inv sw failed");
        for(int i=0;i<sw_s.row;i++){
            for(int j=0;j<sw_s.col;j++){
                sw_t[i][j]=sw[j][i];
            }
        }
        m_clr(ss,ss_s);
        m_mul(sw_t,sw,ss,sw_t_s,sw_s,ss_s);
        if(m_inv(ss,inv_sw,ss_s,inv_sw_s) == 0){
            m_clr(ss,ss_s);
            m_mul(inv_sw,sw_t,ss,inv_sw_s,sw_t_s,ss_s);
            m_cpy(ss,inv_sw,ss_s,inv_sw_s);
            m_clr(ss,ss_s);
            m_mul(inv_sw,sb,ss,inv_sw_s,sb_s,ss_s);
        }
        else{
            printf("left pseudo inv sw failed");
            return -1;
        }
    }

    float** eigVal;
    matrix_s eigVal_s={m,1};
    m_init(&eigVal,eigVal_s);

    float** eigVec;
    matrix_s eigVec_s={m,m};
    m_init(&eigVec,eigVec_s);

    Eig(ss,eigVec,eigVal,ss_s,eigVec_s,eigVal_s,50);






    //free
    m_free(&eigVec,eigVec_s);
    m_free(&eigVal,eigVal_s);
    m_free(&data,data_s);
    m_free(&mean_v,mean_v_s);
    m_free(&mean_class_v,mean_class_v_s);
    m_free(&col_v,col_v_s);
    m_free(&row_v,row_v_s);
    m_free(&sw,sw_s);
    m_free(&sk,sk_s);
    m_free(&sb,sb_s);
    m_free(&inv_sw,inv_sw_s);
    m_free(&ss,ss_s);
    m_free(&sw_t,sw_t_s);

    /*for(int i=0;i<m;i++)
        free(sw_t[i]);
    free(sw_t);
    for(int i=0;i<m;i++)
        free(ss[i]);
    free(ss);
    for(int i=0;i<m;i++)
        free(inv_sw[i]);
    free(inv_sw);
    for(int i=0;i<m;i++)
        free(sw[i]);
    free(sw);
    for(int i=0;i<m;i++)
        free(sk[i]);
    free(sk);
    for(int i=0;i<m;i++)
        free(sb[i]);
    free(sb);
    for(int i=0;i<m;i++)
        free(col_v[i]);
    free(col_v);
    for(int i=0;i<1;i++)
        free(row_v[i]);
    free(row_v);
    for(int i=0;i<m;i++)
        free(mean_class_v[i]);
    free(mean_class_v);
    for(int i=0;i<m;i++)
        free(data[i]);
    free(data);
    for(int i=0;i<m;i++)
        free(mean_v[i]);
    free(mean_v);*/
    free(d_lable);
    return 0;
}

int m_add(float** a,float** b,float** c,matrix_s a_s,matrix_s b_s,matrix_s c_s){  //c=a+b
    int m=a_s.row;
    //printf("m=%d\n",m);
    if(m != b_s.row || m != c_s.row){
        printf("size error");
        return -1;
    }
    int n=a_s.col;
    //printf("n=%d\n",n);
    if(n != b_s.col || n != c_s.col){
        printf("size error");
        return -1;
    }
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            c[i][j]=a[i][j]+b[i][j];
        }
    }
    return 0;
}

int m_sub(float** a,float** b,float** c,matrix_s a_s,matrix_s b_s,matrix_s c_s){  //c=a-b
    int m=a_s.row;
    printf("m=%d\n",m);
    if(m != b_s.row || m != c_s.row){
        printf("size error");
        return -1;
    }
    int n=a_s.col;
    printf("n=%d\n",n);
    if(n != b_s.col || n != c_s.col){
        printf("size error");
        return -1;
    }
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            c[i][j]=a[i][j]-b[i][j];
        }
    }
    return 0;
}

int m_mul(float** a,float** b,float** c,matrix_s a_s,matrix_s b_s,matrix_s c_s){  //c=a*b
    
    if(a_s.row != c_s.row){
        printf("size error");
        return -1;
    }
    
    if(a_s.col != b_s.row ){
        printf("size error");
        return -1;
    }

    if(b_s.col != c_s.col ){
        printf("size error");
        return -1;
    }

    for (int i=0;i<a_s.row;i++){
        for(int j=0;j<b_s.col;j++){
            for(int k=0;k<a_s.col;k++){
                c[i][j]+=a[i][k]*b[k][j];
            }
        }
    }
    return 0;
}

int m_clr(float** a,matrix_s a_s){    //a=zeros;
    for (int i=0;i<a_s.row;i++){
        for(int j=0;j<a_s.col;j++){
            a[i][j]=0;
        }
    }
    return 0;
}

int m_cpy(float** a,float** b,matrix_s a_s,matrix_s b_s){    //b=a;
    if(a_s.col != b_s.col ){
        printf("size error");
        return -1;
    }

    if(a_s.row != b_s.row ){
        printf("size error");
        return -1;
    }

    for (int i=0;i<a_s.row;i++){
        for(int j=0;j<a_s.col;j++){
            b[i][j]=a[i][j];
        }
    }
    return 0;
}

int m_inv(float** a,float** b,matrix_s a_s,matrix_s b_s){  //b=a^-1
    //gauss_jordan_elimination
    if(a_s.row != a_s.col || b_s.row != b_s.col || a_s.row != b_s.row){
        printf("size error");
        return -1;
    }
    // 填好參數化的部分
    for (int i=0; i<b_s.row; ++i)
        for (int j=0; j<b_s.col; ++j)
            b[i][j] = 0;
 
    for (int i=0; i<b_s.row; ++i)
        b[i][i] = 1;
 
    // 開始進行高斯喬登消去法
    // 內容幾乎與高斯消去法相同
    for (int i=0; i<a_s.row; ++i)
    {
        if (a[i][i] == 0){
            for (int j=i+1; j<a_s.row; ++j){
                if (a[j][i] != 0)
                {
                    for (int k=i; k<a_s.row; ++k){
                        float tmp=a[i][k];
                        a[i][k]=a[j][k];
                        a[j][k]=tmp;
                    }
                    for (int k=0; k<a_s.row; ++k){
                        float tmp=b[i][k];
                        b[i][k]=b[j][k];
                        b[j][k]=tmp;
                    }
                    break;
                }
            }
        }
        // 反矩陣不存在。
        if (a[i][i] == 0) return -1;
 
        float t = a[i][i];
        for (int k=i; k<a_s.row; ++k){
            a[i][k] /= t;
        }
        for (int k=0; k<a_s.row; ++k){
            b[i][k] /= t;
        }
            
 
        // 消去時，所有橫條通通消去。
        for (int j=0; j<a_s.row; ++j)
            if (i != j && a[j][i] != 0)
            {
                float t = a[j][i];
                for (int k=i; k<a_s.row; ++k){
                    a[j][k] -= a[i][k] * t;
                }
                for (int k=0; k<a_s.row; ++k){
                    b[j][k] -= b[i][k] * t;
                }
                    
            }
    }
    return 0;
}

int m_init(float *** a,matrix_s a_s){
    float **t = (float **) malloc(a_s.row * sizeof(float *)); //r
    for (int i=0;i<a_s.row;i++)
        t[i]=(float *) malloc(a_s.col * sizeof(float)); //c
    if (t==NULL){
        printf("malloc 失敗");
        return -1;
    }
    *a=t;
    return 0;
}

int m_free(float *** a,matrix_s a_s){
    float **t=*a;
    for(int i=0;i<a_s.row;i++)
        free(t[i]);
    free(t);
    return 0;
}

int m_print(float** a,matrix_s a_s){
	for (int i = 0; i < a_s.row; i++){
        for (int j = 0; j < a_s.col; j++){
            printf("%12.4g  ", a[i][j]);
        }
		printf("\n");
	}
	printf("\n");
    return 0;
}

float m_norm2(float** a,matrix_s a_s){
    float norm2_ans = 0.0;
    for (int i = 0; i < a_s.row; i++){
        for (int j = 0; j < a_s.col; j++){
            norm2_ans+=(a[i][j]) * (a[i][j]);
        }
	}
    norm2_ans = (float)sqrt(norm2_ans);
	return norm2_ans;
}

/************************************************************************
函式功能：對一個方陣A進行QR分解
輸入：需要分解的矩陣A、分解後的正交矩陣Q和上三角矩陣R
輸出：無
************************************************************************/
int m_qr(float** a,float** q,float** r,matrix_s a_s,matrix_s q_s,matrix_s r_s){
    float** col_A,** col_Q;
    matrix_s col_A_s = { a_s.row, 1 };
    matrix_s col_Q_s = { a_s.row, 1 };
    m_init(&col_A,col_A_s); //用來存A的每一列
    m_init(&col_Q,col_Q_s); //用來存Q的每一列

    if (a_s.row != a_s.col){
        printf("A is not a square matrix!");
        return -1;
    }

    if(a_s.row != q_s.row || a_s.col != q_s.col){
        m_free(&q,q_s);
        q_s.row=a_s.row;
        q_s.col=a_s.col;
        m_init(&q,q_s);
    }

    if(a_s.row != r_s.row || a_s.col != r_s.col){
        m_free(&r,r_s);
        r_s.row=a_s.row;
        r_s.col=a_s.col;
        m_init(&r,r_s);
    }

    m_clr(q,q_s);
    m_clr(r,r_s);

    //施密特正交化
    for(int j=0;j<a_s.col;j++){
        for(int i=0;i<a_s.col;i++){
            col_A[i][0]=a[i][j];
            col_Q[i][0]=a[i][j];
        }
        for(int k=0;k<j;k++){   //計算第j列以前
            r[k][j]=0;
            for (int i1 = 0; i1 < col_A_s.row; i1++){   //R=Q'A(Q'即Q的轉置) 即Q的第k列和A的第j列做內積
                r[k][j] += col_A[i1][0] * q[i1][k]; //Q的第k列
            }
            for (int i2 = 0; i2 < a_s.col; i2++){   
                col_Q[i2][0] -= r[k][j] * q[i2][k]; 
            }
        }

        float temp = m_norm2(col_Q,col_Q_s);
        r[j][j] = temp;
        for(int i3=0;i3<q_s.col;i3++){
            //單位化Q
            q[i3][j] = col_Q[i3][0] / temp;
        }
    }
	m_free(&col_A,col_A_s);
    m_free(&col_Q,col_Q_s);	

    return 0;
}

/************************************************************************
函式功能：給特徵值排序，當flag=1時，則升序，當flag=0，則降序
輸入：需要排序的序列eValue，升序還是降序的選擇flag
輸出：排序成功則返回true，否則返回false
************************************************************************/
int eig_val_sort(float** a,matrix_s a_s,int flag){
    for(int i=0;i<a_s.row;i++){
        int k=i;
        for(int j=i+1;j<a_s.row;j++){
            if(flag==1){
                if(a[k][0] > a[j][0]){
                    k=j;
                }
            }
            else if(flag==0){
                if(a[k][0] < a[j][0]){
                    k=j;
                }
            }
            else return -1;
        }
        if(k!=i){
            float temp;
            temp = a[i][0];
            a[i][0]=a[k][0];
            a[k][0]=temp;
        }
    }
    return 0;
}

/************************************************************************
函式功能：已知一個矩陣的特徵值求對應的特徵向量
輸入：一個矩陣A、用來存結果的特徵向量eigenVector、已知的特徵值eigenValue
輸出：無
************************************************************************/
int eigVector(float** a,float** evec,float** eval,matrix_s a_s,matrix_s evec_s,matrix_s eval_s){
    int num=a_s.col;
    float eValue;
    float** temp1;
    matrix_s temp1_s={a_s.row,a_s.col};
    m_init(&temp1,temp1_s);
    m_clr(temp1,temp1_s);

    for(int count=0;count<num;count++){
        eValue=eval[count][0]; //當前的特徵值
        m_cpy(a,temp1,a_s,temp1_s); //這個每次都要重新複製，因為後面會破壞原矩陣(剛開始沒注意到這個找bug找了好久。。)
        for(int i=0;i<temp1_s.row;i++){
            temp1[i][i] -= eValue;
        }
        //將temp1化為階梯型矩陣(歸一性)對角線值為一
        for(int i=0;i<temp1_s.row-1;i++){
            float coe = temp1[i][i];
            for(int j=i;j<temp1_s.col;j++){
                temp1[i][j] /= coe;  //讓對角線值為一
            }
            for(int i1=i+1;i1<temp1_s.row;i1++){
                coe=temp1[i1][i];
                for(int j1=i;j1<temp1_s.col;j1++){
                    temp1[i1][j1] -= coe*temp1[i][j1];
                }
            }
        }
        //讓最後一行為1
        float sum1 = evec[evec_s.row-1][count] = 1;
        for(int i2=temp1_s.row-2;i2>=0;i2--){
            float sum2 = 0;
            for(int j2=i2+1;j2<temp1_s.col;j2++){
                sum2 += temp1[i2][j2] * evec[j2][count];
            }
            sum2 = -sum2 / temp1[i2][i2];
            sum1 += sum2 * sum2;
            evec[i2][count]=sum2;
        }
        sum1 = sqrt(sum1);  //當前列的模
        for(int i=0;i<evec_s.row;i++){
            //單位化
            evec[i][count] /= sum1;
        }
    }

    m_free(&temp1,temp1_s);
    return 0;
}

int Eig(float** a,float** evec,float** eval,matrix_s a_s,matrix_s evec_s,matrix_s eval_s,int NUM){
    float** temp;
    matrix_s temp_s={a_s.row,a_s.col};
    m_init(&temp,temp_s);

    m_cpy(a,temp,a_s,temp_s);

    float** temp_Q;
    matrix_s temp_Q_s={a_s.row,a_s.col};
    m_init(&temp_Q,temp_Q_s);

    float** temp_R;
    matrix_s temp_R_s={a_s.row,a_s.col};
    m_init(&temp_R,temp_R_s);

    float** eValue;
    matrix_s eValue_s={a_s.row,a_s.col};
    
    for(int k=0;k<NUM;k++){ //使用QR分解求矩陣特徵值
        m_qr(temp,temp_Q,temp_R,temp_s,temp_Q_s,temp_R_s);
        m_clr(temp,temp_s);
        m_mul(temp_R,temp_Q,temp,temp_R_s,temp_Q_s,temp_s);
    }

    for(int k=0;k<temp_s.col;k++){ //獲取特徵值，將之儲存於eValue
        eval[k][0]=temp[k][k];
    }

    eig_val_sort(eval,eval_s,1); //給特徵值排序，1為升序，0為降序

    printf("特徵值：\n");
    m_print(eval,eval_s);

    eigVector(a,evec,eval,a_s,evec_s,eval_s);
    printf("特徵向量：\n");
    m_print(evec,evec_s);

    m_free(&temp,temp_s);
    m_free(&temp_Q,temp_Q_s);
    m_free(&temp_R,temp_R_s);

    return 0;
}