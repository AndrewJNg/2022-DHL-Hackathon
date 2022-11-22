
# https://youtu.be/0N3OzEA9A8I

import cv2
from pyzbar import pyzbar


'''url = "http://192.168.43.1:8080/video"    # uncomment when using phone camera through wifi
cap = cv2.VideoCapture(url)'''  
cap = cv2.VideoCapture(0)                   # use in built camera instead
# cap.set(cv2.CAP_PROP_AUTOFOCUS, 0) # turn the autofocus off

lastbarcode =0
while True:
   
# setup display specs 
    ret, frame = cap.read()
    # frame = cv2.resize(frame, (0,0),fx = 0.50,fy=0.50)
    frame = cv2.resize(frame, (0,0),fx = 1.5,fy=1.5)
    
# show instruction to close
    cv2.putText(frame,"Q to exit",(10,10),cv2.FONT_HERSHEY_SIMPLEX,0.5,(0,255,100),1)
    # cv2.putText(frame,"Press q to close camera",(10,10),cv2.FONT_HERSHEY_SIMPLEX,0.5,(0,255,100),1)


# barcode processing
    barcodes = pyzbar.decode(frame)
    # print(barcode)
    for barcode in barcodes:
        (x,y,w,h) = barcode.rect
        cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0),1)
        barcodeData = barcode.data.decode('utf=8')
        barcodeType = barcode.type
        text = "Barcode: {}\n Type: {}".format(barcodeData,barcodeType)
        cv2.putText(frame,text,(x,y-10),cv2.FONT_HERSHEY_SIMPLEX,0.5,(0,0,255),1)

        # barcode filter
        isEqualLastBarcode = lastbarcode != barcodeData
        contain_symbols = '?' in barcodeData 
        if (isEqualLastBarcode  ):
            print(barcodeData)
            lastbarcode = barcodeData
        
        print(barcodeData)   
        print("  ")   
        # cv2.putText(frame,text,(x,y-10),cv2.FONT_HERSHEY_SIMPLEX,0.5,(0,0,255),1)

# display
    cv2.imshow("Barcode Scanner",frame)

    
    # exit when q is pressed
    # cv2.waitKey(1)
    if cv2.waitKey(1) == ord('q'):
        break

if text:
    print(text)
cap.release()
cv2.destroyAllWindows


































    





































