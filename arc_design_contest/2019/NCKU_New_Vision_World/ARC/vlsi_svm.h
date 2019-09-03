#ifndef VLSI_SVM_H
#define VLSI_SVM_H

#define SVM_INPUT_SIZE      80
#define SVM_KERNEL_SIZE     95
#define SVM_RESULT_SIZE     3
#define SVM_INTERC_SIZE     3

/** \brief svm predict
*/
int vlsi_svm_predict(float svm_test_data[SVM_INPUT_SIZE]);

#endif
