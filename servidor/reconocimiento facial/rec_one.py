# coding=utf-8

import cv2
import os
import numpy as np
import sys
import urllib.request

dataPath = r'C:/Users/USUARIO DELL/Documents/IoT/servidor/reconocimiento facial/images' #Cambia a la ruta donde hayas almacenado Data
imagePaths = os.listdir(dataPath)
url = sys.argv[1]

#url = "https://firebasestorage.googleapis.com/v0/b/timbre-inteligente-b20fd.appspot.com/o/faces%2Fphoto2022-11-28T19%3A16%3A25Z.jpg?alt=media&token=63d666f5-9a89-462c-a8a6-73c904107089"

peopleList = [nameDir for nameDir in os.listdir(dataPath)]


face_recognizer = cv2.face.LBPHFaceRecognizer_create()

face_recognizer.read('modeloLBPHFace.xml')

faceClassif = cv2.CascadeClassifier(cv2.data.haarcascades+'haarcascade_frontalface_default.xml')

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

    # LBPHFace
    if result[1] < 90:
        print(imagePaths[result[0]])
    else:
        print(-1)
        
if(len(faces) == 0):
    print(-1)



sys.stdout.flush()