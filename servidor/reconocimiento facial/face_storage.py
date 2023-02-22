import cv2
import os
import imutils
import urllib.request
import numpy as np


personName = 'julio'
dataPath = 'C:/Users/USUARIO DELL/Downloads/ATTENDANCE/images'
url='http://192.168.43.103/cam-hi.jpg'

personPath = dataPath + "/" + personName

if not os.path.exists(personPath):
    print('Carpeta creada: ',personPath)
    os.makedirs(personPath)


faceClassif = cv2.CascadeClassifier(cv2.data.haarcascades+'haarcascade_frontalface_default.xml')
count = 0


cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)


while True:

    ret, frame = cap.read()
    if ret == False:
          break
    
    #img_resp=urllib.request.urlopen(url)
    #imgnp=np.array(bytearray(img_resp.read()),dtype=np.uint8)
    #img=cv2.imdecode(imgnp,-1)

    #frame = cv2.resize(img, (0, 0), None, 0.25, 0.25)

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    auxFrame = frame.copy()

    frame = cv2.resize(frame, (0, 0), None, 0.25, 0.25)

    
    faces = faceClassif.detectMultiScale(gray, 1.3, 5)

    for(x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
        rostro = auxFrame[y:y+h, x:x+w]
        rostro = cv2.resize(rostro, (150, 150), interpolation=cv2.INTER_CUBIC)
        cv2.imwrite(personPath + "/" + personName + "_{}.jpg".format(count), rostro)
        count = count + 1
        
    #cv2.imshow('Webcam', img)
    cv2.imshow("webcam", auxFrame)

    k = cv2.waitKey(1)
    if k == 27 or count >= 300:
        break

cv2.destroyAllWindows()
cv2.imread