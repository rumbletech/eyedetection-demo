import cv2
import os
import time 


haar_face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_alt.xml')
haar_eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')

cap = cv2.VideoCapture(0)
while True:
    ret,img = cap.read()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = haar_face_cascade.detectMultiScale(gray, 1.1, 5)

    for(x,y,z,h) in faces:
        cv2.rectangle(img,(x,y),(x+z , y+h),(255,0,0),2)
        roi_gray = gray[y:y+h , x:x+z]
        roi_color = img[y:y+h , x:x+z]
        eyes = haar_eye_cascade.detectMultiScale(roi_gray)
        if(eyes == ()):
            print("Problem")
            os.system("simple_vcom_tx.exe A")
                        
        else:
            print("No problem")
            os.system("simple_vcom_tx.exe B")
            for (ex, ey, ez, eh) in eyes:
                cv2.rectangle(roi_color, (ex, ey), (ex + ez, ey + eh), (0, 255, 0), 2)



    cv2.imshow('img',img)
    k = cv2.waitKey(30)
    if k == 27:
        break

cap.release()
cv2.destroyAllWindows()
