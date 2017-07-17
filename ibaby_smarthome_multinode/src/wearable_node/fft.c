#include "fft.h"

/* add function of complex number */
static complex_num complex_add(complex_num a, complex_num b)
{
	complex_num c;
	c.real = a.real + b.real;
	c.img  = a.img  + b.img;
	return c;
}

/* subtraction function of complex number */
static complex_num complex_sub(complex_num a, complex_num b)
{
	complex_num c;
	c.real = a.real - b.real;
	c.img  = a.img  - b.img;
	return c;
}

/* multiplication function of complex number */
static complex_num complex_mul(complex_num a, complex_num b)
{
	complex_num c;
	c.real = a.real * b.real - a.img * b.img;
	c.img  = a.real * b.img  + a.img * b.real;
	return c;
}

/* the rotation factor initialize function */
extern void rotation_factor_init(void)
{
	int i;

	/*
	 * in stage m, there are L rotation factors
	 *    rota_fact(k) = W(k, 2^m) = W(N*k/(2L), N)
	 */
	for (i = 0; i < DATA_SIZE; i++) {
		rota_fac[i].real = cos(2 * PI * i / DATA_SIZE);
		rota_fac[i].img = -1 * sin(2 * PI * i / DATA_SIZE);
	}
}

/* reverse each bit of index number, arrange index number of input data */
extern void reverse(void)
{
	unsigned int i, j, k;
	unsigned int t;
	complex_num data_temp;

	for (i = 0; i < DATA_SIZE; i++)
	{
		k = i; /* current index number : i */
		j = 0; /* index number reversed */
		for (t = 0; t < BIT_SIZE; t++) /* index number has BIT_SIZE bits */
		{
			j <<= 1;
			j |= (k & 1); /* j shift to the left 1 bit, then complex_add the last bit of k to it */
			k >>= 1;      /* k shift to the right 1 bit */
		}

		/*
		 * if the index number reversed is bigger than the origin, exchange their value storaged
		 * it can avoid repeated exchange if we compare their(index number reversed and the origin)
		 * size before exchanging
		 */
		if (j > i)
		{
			data_temp = data[i];
			data[i] = data[j];
			data[j] = data_temp;
		}
	}
}

/*
 * fast fourier transformation function(fft)
 * for N points fft computation, there has log2(N) stages
 *    in stage m, there has N/2/L groups, L = 2^(m-1), and L butterfly forms in each group
 */
extern void fft(void)
{
	unsigned int i, j, k, l;
	complex_num top, bottom, common;

	for (i = 0; i < STAGE_SIZE; i++) /* loop for stage */
	{
		/*
		 * distance between the two point in butterfly form
		 * it also means the number of butterfly form per group
		 */
		l = 1 << i;

		/* loop for group */
		for (j = 0; j < DATA_SIZE; j += 2 * l)
		{
			/* loop for butterfly form */
			for (k = 0; k < l; k++)
			{
				/* once butterfly computation */
				common = complex_mul(data[j + k + l], rota_fac[DATA_SIZE / (2 * l) * k]);
				top    = complex_add(data[j + k], common);
				bottom = complex_sub(data[j + k], common);

				data[j + k]     = top;
				data[j + k + l] = bottom;
			}
		}
	}
}

/* print out the data after fft */
extern void pr_data_fft(void)
{
	unsigned int i;

	printf("Print out the data.\n");
	for (i = 0; i < DATA_SIZE; i++) {
		printf("data[%d] = [%.4f\t%.4f]\n", i, data[i].real, data[i].img);
		if(i != 0 && (i+1) % 8 == 0)
			printf("\n");
	}
	printf("\n");
}

// void data_init(void)
// {
// 	unsigned int i, j;

// 	for (i = 0; i < 8; ++i)
// 	{
// 		for (j = 0; j < 8; ++j)
// 		{
// 			data[i * 8 + j].real = (i + j + 1) % 10;
// 		}
// 	}
// }
// 
// void main()
// {
// 	printf("Data initialize.\n");
// 	data_init();
// 	pr_data_fft();

// 	/* initialize the rotation factor : rota_fac[DATA_SIZE] */
// 	rotation_factor_init();

// 	/* arrange the index number */
// 	reverse();

// 	/* fast fourier transformation */
// 	fft();

// 	/* print out the data after fft */
// 	pr_data_fft();
// }
