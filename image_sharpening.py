# load the required packages
import cv2
import numpy as np

from pyzbar import pyzbar

# load the image into system memory
# image = cv2.imread('blur_barcode.jpg', flags=cv2.IMREAD_COLOR)
# image = cv2.imread('clear_barcode.jpg', flags=cv2.IMREAD_COLOR)
image = cv2.imread('crystal_barcode.jpg', flags=cv2.IMREAD_COLOR)
# image = cv2.imread('barcode.png', flags=cv2.IMREAD_COLOR)
# image = cv2.imread('10_A-0100-Z.jpg', flags=cv2.IMREAD_COLOR)

# image = cv2.resize(image, (0,0),fx = 0.30,fy=0.30)
# display the image to the screen
cv2.imshow('AV CV- Winter Wonder', image)
cv2.waitKey()
# cv2.destroyAllWindows()

kernel = np.array([[0, -1, 0],
                   [-1, 5,-1],
                   [0, -1, 0]])
image_sharp = cv2.filter2D(src=image, ddepth=-1, kernel=kernel)

# image_sharp = cv2.resize(image_sharp, (0,0),fx = 0.10,fy=0.10)
cv2.imshow('AV CV- Winter Wonder Sharpened', image_sharp)
cv2.waitKey()
cv2.destroyAllWindows()


def search_barcode(frame):  

    # barcode processing
    barcodes = pyzbar.decode(frame)
    
    for barcode in barcodes:
        # (x,y,w,h) = barcode.rect
        # cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0),1)
        barcodeData = barcode.data.decode('utf=8')
        # barcodeType = barcode.type

        return(barcodeData)

print(search_barcode(image))
print(search_barcode(image_sharp))