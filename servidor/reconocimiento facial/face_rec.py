# coding=utf-8

import cv2
import os
import numpy as np
import sys
import urllib.request

dataPath = r'C:/Users/USUARIO DELL/Documents/IoT/servidor/reconocimiento facial/images' #Cambia a la ruta donde hayas almacenado Data
imagePaths = os.listdir(dataPath)
url = sys.argv[1]
#print('imagePaths=',imagePaths)

peopleList = [nameDir for nameDir in os.listdir(dataPath)]
#print(peopleList)

#face_recognizer = cv2.face.EigenFaceRecognizer_create()
#face_recognizer = cv2.face.FisherFaceRecognizer_create()
face_recognizer = cv2.face.LBPHFaceRecognizer_create()


# Leyendo el modelo
#face_recognizer.read('modeloEigenFace.xml')
#face_recognizer.read('modeloFisherFace.xml')
face_recognizer.read('modeloLBPHFace.xml')



#cap = cv2.VideoCapture(0,cv2.CAP_DSHOW)
#cap = cv2.VideoCapture('Video.mp4')
faceClassif = cv2.CascadeClassifier(cv2.data.haarcascades+'haarcascade_frontalface_default.xml')


"""
labels = []
facesData = []
label = 0
for nameDir in peopleList:
    personPath = dataPath + '/' + nameDir
    print("Leyendo las imagenes")
    for fileName in os.listdir(personPath):
        print('Rostros: ', nameDir + '/' + fileName)
        labels.append(label)
        facesData.append(cv2.imread(personPath+'/'+fileName,0))
        #image = cv2.imread(personPath+'/'+fileName,0)
        #cv2.imshow('image',image)
        #cv2.waitKey(10)
    label = label + 1

face_recognizer.train(facesData, np.array(labels))

"""

#ret,frame = cap.read()
#if ret == False: break

img_resp=urllib.request.urlopen(url)
imgnp=np.array(bytearray(img_resp.read()),dtype=np.uint8)
img=cv2.imdecode(imgnp,-1)

frame = cv2.resize(img, (0, 0), None, 0.25, 0.25)

gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
auxFrame = gray.copy()
faces = faceClassif.detectMultiScale(gray,1.3,5)

for (x,y,w,h) in faces:
    rostro = auxFrame[y:y+h,x:x+w]
    rostro = cv2.resize(rostro,(150,150),interpolation= cv2.INTER_CUBIC)
    result = face_recognizer.predict(rostro)
    #cv2.putText(frame,'{}'.format(result),(x,y-5),1,1.3,(255,255,0),1,cv2.LINE_AA)
    
    # EigenFaces
    #if result[1] < 5700:
    #    cv2.putText(frame,'{}'.format(imagePaths[result[0]]),(x,y-25),2,1.1,(0,255,0),1,cv2.LINE_AA)
    #    cv2.rectangle(frame, (x,y),(x+w,y+h),(0,255,0),2)
    #else:
    #    cv2.putText(frame,'Desconocido',(x,y-20),2,0.8,(0,0,255),1,cv2.LINE_AA)
    #    cv2.rectangle(frame, (x,y),(x+w,y+h),(0,0,255),2)
    
    # FisherFace
    #if result[1] < 500:
    #    cv2.putText(frame,'{}'.format(imagePaths[result[0]]),(x,y-25),2,1.1,(0,255,0),1,cv2.LINE_AA)
    #    cv2.rectangle(frame, (x,y),(x+w,y+h),(0,255,0),2)
    #else:
    #    cv2.putText(frame,'Desconocido',(x,y-20),2,0.8,(0,0,255),1,cv2.LINE_AA)
    #    cv2.rectangle(frame, (x,y),(x+w,y+h),(0,0,255),2)
    
    # LBPHFace
    if result[1] < 60:
        #cv2.putText(frame,'{}'.format(imagePaths[result[0]]),(x,y-25),2,1.1,(0,255,0),1,cv2.LINE_AA)
        #cv2.rectangle(frame, (x,y),(x+w,y+h),(0,255,0),2)
        print(imagePaths[result[0]])
    else:
        #cv2.putText(frame,'Desconocido',(x,y-20),2,0.8,(0,0,255),1,cv2.LINE_AA)
        #cv2.rectangle(frame, (x,y),(x+w,y+h),(0,0,255),2)
        print(-1)
        
    #cv2.imshow('frame',frame)
    #k = cv2.waitKey(1)
    #if k == 27:
    #    break
#cap.release()
#cv2.destroyAllWindows()

#face_recognizer.write('modeloEigenFace.xml')
#face_recognizer.write('modeloFisherFace.xml')
#face_recognizer.write('modeloLBPHFace.xml')
#print("Modelo almacenado...")
if(len(faces) == 0):
    print(-1)



sys.stdout.flush()