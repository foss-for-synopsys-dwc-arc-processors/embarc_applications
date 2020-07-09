from keras.layers import Conv2D, MaxPooling2D, Reshape, Activation, BatchNormalization, Dense, Input,Flatten,Embedding,Lambda,AveragePooling2D,Add,concatenate
from keras.models import Model
from keras.layers import Dropout,LeakyReLU
from keras import regularizers
from keras import initializers
from keras import backend as K
from scipy.spatial.distance import cosine 
from keras.layers.merge import dot

def Center_mFCN3_relu_max2(input_shape,classes=2, bn_axis=-1,FV_size = 256, isCenterloss=False):
    
    input_tensor = Input(shape=input_shape)

    x = Conv2D(48, (3, 3), strides=(1, 1), padding='same',kernel_regularizer=regularizers.l2(0.0005))(input_tensor)
    x = Activation('relu')(x)
    x = Conv2D(48, (3, 3), strides=(1, 1), padding='same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = MaxPooling2D((2, 2), strides=(2, 2))(x)

    x = Conv2D(96, (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = Conv2D(96, (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)

    x = Conv2D(192, (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = Conv2D(192, (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = MaxPooling2D((2, 2), strides=(2, 2))(x)

    x = Conv2D(128, (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = Conv2D(128, (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = MaxPooling2D((2, 2), strides=(2, 2))(x)

    x = Conv2D(128, (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = MaxPooling2D((2, 2), strides=(2, 2))(x)
    x = Conv2D(128, (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = MaxPooling2D((3, 3), strides=(3, 3))(x)
 
    x = Conv2D(256, (3, 3), strides=(1, 1), padding='same',name='new_FV')(x)
##############################
   #-------FC
    x = Flatten()(x)
    FV = Dense(FV_size,name='center_FV')(x)
    x = Dropout(0.7)(FV)
    x = Dense(classes,name='new_dense')(x)
    x = Activation('softmax', name='new_predictions')(x)


    if isCenterloss:
     #   print('Center loss')
        input_target = Input(shape=(1,)) # single value ground truth labels as inputs
        centers = Embedding(classes,FV_size)(input_target)
        l2_loss = Lambda(lambda x: K.sum(K.square(x[0]-x[1][:,0]),1,keepdims=True),name = 'l2_loss_2')([FV,centers])
        print('FV.shape = ',FV.shape)
        print('center shape = ',centers.shape)
        print('x.shape = ',x.shape)
        print('model / l2_loss.shape = ', l2_loss.shape)
        print('model / input_target.shape = ', input_target.shape)
        model = Model(inputs = [input_tensor, input_target], outputs = [x,l2_loss], name = 'model_hercules_center')        
    else:
        model = Model(inputs = input_tensor, outputs = x, name = 'model_hercules_softmax')

    return model


def Center_mFCN3_relu_max2_WID_fz1_2(input_shape,classes=2, bn_axis=-1,FV_size = 256, isCenterloss=True,WIDSR =0.5 ):
    input_tensor = Input(shape=input_shape)

    x = Conv2D(48, (3, 3), strides=(1, 1), padding='same',kernel_regularizer=regularizers.l2(0.0005))(input_tensor)
    x = Activation('relu')(x)
    x = Conv2D(48, (3, 3), strides=(1, 1), padding='same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = MaxPooling2D((2, 2), strides=(2, 2))(x)

    x = Conv2D(int(96*WIDSR), (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = Conv2D(int(96*WIDSR), (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)

    x = Conv2D(int(192*WIDSR), (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = Conv2D(int(192*WIDSR), (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = MaxPooling2D((2, 2), strides=(2, 2))(x)

    x = Conv2D(int(128*WIDSR), (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = Conv2D(int(128*WIDSR), (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = MaxPooling2D((2, 2), strides=(2, 2))(x)

    x = Conv2D(int(128*WIDSR), (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = MaxPooling2D((2, 2), strides=(2, 2))(x)
    x = Conv2D(int(128*WIDSR), (3, 3), strides=(1, 1), padding= 'same',kernel_regularizer=regularizers.l2(0.0005))(x)
    x = Activation('relu')(x)
    x = MaxPooling2D((3, 3), strides=(3, 3))(x)
 
    x = Conv2D(256, (3, 3), strides=(1, 1), padding='same',name='new_FV')(x)
##############################
   #-------FC
    x = Flatten()(x)
    FV = Dense(FV_size,name='center_FV')(x)
    x = Dropout(0.7)(FV)
    x = Dense(classes,name='new_dense')(x)
    x = Activation('softmax', name='new_predictions')(x)


    if isCenterloss:
     #   print('Center loss')
        input_target = Input(shape=(1,)) # single value ground truth labels as inputs
        centers = Embedding(classes,FV_size)(input_target)
        l2_loss = Lambda(lambda x: K.sum(K.square(x[0]-x[1][:,0]),1,keepdims=True),name = 'l2_loss')([FV,centers])
        print('FV.shape = ',FV.shape)
        print('center shape = ',centers.shape)
        print('x.shape = ',x.shape)
        print('model / l2_loss.shape = ', l2_loss.shape)
        print('model / input_target.shape = ', input_target.shape)
        model = Model(inputs = [input_tensor, input_target], outputs = [x,l2_loss], name = 'model_hercules_center')        
    else:
        model = Model(inputs = input_tensor, outputs = x, name = 'model_hercules_softmax')

    return model


