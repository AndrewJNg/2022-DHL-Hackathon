import cv2 #include opencv library functions in python.


#Create an object to hold reference to camera video capturing.
vidcap = cv2. VideoCapture(0)

#check if connection with camera is successfully.
if vidcap. isOpened():
    ret, frame = vidcap. read() #capture a frame from live video.   