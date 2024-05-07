from skimage.feature import corner_harris, corner_peaks
from skimage.io import imread
from skimage.color import rgb2gray
import cv2
import numpy as np
import matplotlib.pyplot as plt


#I = imread('../data/Gopro/GPTempDownload1.JPG')

img_path = "/Users/kimstp/Dropbox/DIKU_Images/DIKU_underwater/Seq3/Air_calibration_Narrow/Seq1/"
I = rgb2gray(imread(img_path + "GPTempDownload 4.JPG"))

plt.figure()
plt.imshow(I, cmap='gray')


F = corner_harris(I, k=0.1, sigma=4)

plt.figure()
plt.imshow(F, cmap='gray')

corners = corner_peaks(F, min_distance=2)

print(corners)

plt.figure()
plt.imshow(I, cmap='gray')
plt.plot(corners[:,1], corners[:,0], '.')


plt.show()
