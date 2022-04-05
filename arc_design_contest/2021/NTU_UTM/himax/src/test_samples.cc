#include "test_samples.h"

const int kNumSamples = 25;

TestSample sample1 = {
	.label = 2,.image = {
		135,1,2,12
	}
};

TestSample sample2 = {
	.label = 1,
	.image = {
		 135,2,8,12
    

	}
};

const TestSample test_samples[kNumSamples] = {sample1, sample2};