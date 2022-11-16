import cv2
# import pyzbar
from pyzbar.pyzbar import decode
from pyzbar.pyzbar import ZBarSymbol

pic_path = "crystal_barcode.jpg"
# preprocessing using opencv
im = cv2.imread(pic_path, cv2.IMREAD_GRAYSCALE)
ret, bw_im = cv2.threshold(im, 127, 255, cv2.THRESH_BINARY)
# zbar
cv2.imshow("Barcode Scanner",bw_im)
cv2.waitKey()
barcodes = decode(bw_im, symbols=[ZBarSymbol.CODE93])
print(barcodes)