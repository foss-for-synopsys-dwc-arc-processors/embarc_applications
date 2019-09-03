import numpy as np
import math

# This replicates clf.decision_function(X)
def decision_function(params, sv, nv, a, b, x_test):
    # calculate the kernels
    # k = kernel(params, sv, x_test)
    k = np.dot(sv[0], x_test.T)
    for i in range(1, len(sv)):
        k_tmp = np.dot(sv[i], x_test.T)
        k = np.vstack((k, k_tmp))

    # define the start and end index for support vectors for each class
    start = [sum(nv[:i]) for i in range(len(nv))]
    end = [start[i] + nv[i] for i in range(len(nv))]

    # calculate: sum(a_p * k(x_p, x)) between every 2 classes8
    c = [ sum(a[i][p] * k[p] for p in range(start[j], end[j])) +
          sum(a[j-1][p] * k[p] for p in range(start[i], end[i]))
                for i in range(len(nv)) for j in range(i+1,len(nv))]

    # add the intercept
    return [sum(x) for x in zip(c, b)]

# This replicates clf.predict(X)
def predict(params, sv, nv, a, b, cs, x_test):

    decision = decision_function(params, sv, nv, a, b, x_test)
    votes = []
    for s in range(len(x_test)):
        votes.append([(i if decision[p][s] > 0 else j) for p, (i, j) in enumerate((i, j) for i in range(len(cs)) for j in range(i+1, len(cs)))])

    cs = []
    for s in range(len(x_test)):
        cs.append(max(set(votes[s]), key=votes[s].count))

    return cs

#X_tmp = np.loadtxt('final.txt')
#X = X_tmp[:, :2]
X = np.loadtxt('final.txt')
Y = np.loadtxt('final_target.txt')

from sklearn.model_selection import train_test_split
x_train, x_test, y_train, y_test = train_test_split(X, Y, test_size=0.3,  random_state=87)

from sklearn.svm import SVC
clf = SVC(kernel='linear')
clf.fit(x_train, y_train)

# Get parameters from model
params = clf.get_params()
sv = clf.support_vectors_
nv = clf.n_support_
a = clf.dual_coef_
b = clf.intercept_
cs = clf.classes_

test = clf.decision_function(x_test)
y_predict = clf.predict(x_test)
print(y_predict)
# Use the functions to predict
predict_algorithm = predict(params, sv, nv, a, b, cs, x_test)
print(predict_algorithm)
print('OK')