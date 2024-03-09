import argparse
import pathlib
import numpy as np
import matplotlib.pyplot as plt
from skimage.io import imread
from skimage.util import img_as_float
from skimage.exposure import histogram
import pandas as pd

def empty_dataframe():
    record = pd.DataFrame({
        "Filename stem": [],
        "Quantization org": [],
        "Quantization lin": [],
        "Quantization min": [],
        "Quantization max": [],
        "Dynamic range min org": [],
        "Dynamic range max org": [],
        "Dynamic range min lin": [],
        "Dynamic range max lin": [],
        "#Saturated pixels in R min org": [],
        "#Saturated pixels in G min org": [],
        "#Saturated pixels in B min org": [],
        "#Saturated pixels in R max org": [],
        "#Saturated pixels in G max org": [],
        "#Saturated pixels in B max org": [],
        "#Saturated pixels in R min lin": [],
        "#Saturated pixels in G min lin": [],
        "#Saturated pixels in B min lin": [],
        "#Saturated pixels in R max lin": [],
        "#Saturated pixels in G max lin": [],
        "#Saturated pixels in B max lin": [],
        "Resolution in #pixels": [],
        "%Saturated pixels org": [],
        "%Saturated pixels lin": [],
    })
    return record

def check_saturation(fname, img_org, img_lin, outpath=""):
    """Compate a pair of images and plot histograms of the two images as well as print some statistics."""
    print(" ")
    print(fname)
    print("Quantization limits: dtype = ", img_org.dtype, " bits = ", np.iinfo(img_org.dtype).bits)
    print("  min = ", np.iinfo(img_org.dtype).min, " max = ", np.iinfo(img_org.dtype).max)
    print("Dynamic range: ")
    print("  org dtype = ", img_org.dtype, " lin dtype = ", img_lin.dtype)
    print("  min org: ", img_org.min(), " max org: ", img_org.max())
    print("  min lin: ", img_lin.min(), " max lin: ", img_lin.max())

    #plt.figure()
    #plt.imshow(img_as_float(img_org))
    #plt.title('Original Image - ' + fname)

    #plt.figure()
    #plt.imshow(img_as_float(img_lin))
    #plt.title('Linear Image - ' + fname)

    hist, bin_centers = histogram(img_org, channel_axis=2)
    hist_lin, bin_centers_lin = histogram(img_lin, channel_axis=2)

    #hist_intensity, _ = histogram(np.mean(img_org, axis=2), nbins=2**16)
    #hist_intensity_lin, _ = histogram(np.mean(img_lin, axis=2), nbins=2**16)

    print("#saturated pixels per channel: ")
    print("  min org: R = ", hist[0, 0], " G = ", hist[1, 0], " B = ", hist[2, 0])
    print("  max org: R = ", hist[0, -1], " G = ", hist[1, -1], " B = ", hist[2, -1])
    print("  min lin: R = ", hist_lin[0, 0], " G = ", hist_lin[1, 0], " B = ", hist_lin[2, 0])
    print("  max lin: R = ", hist_lin[0, -1], " G = ", hist_lin[1, -1], " B = ", hist_lin[2, -1])

    plt.figure()
    plt.suptitle(fname)
    plt.subplot(1, 2, 1)
    plt.plot(bin_centers, hist[0, :], lw=2, color='r')
    plt.plot(bin_centers, hist[1, :], lw=2, color='g')
    plt.plot(bin_centers, hist[2, :], lw=2, color='b')
    #plt.plot(bin_centers, hist_intensity, lw=2, color='g')
    (ybottom, ytop)=plt.ylim()
    plt.xlabel('Intensity')
    plt.ylabel('Histogram')
    plt.title('Original Image Histogram')

    plt.subplot(1, 2, 2)
    plt.plot(bin_centers_lin, hist_lin[0, :], lw=2, color='r')
    plt.plot(bin_centers_lin, hist_lin[1, :], lw=2, color='g')
    plt.plot(bin_centers_lin, hist_lin[2, :], lw=2, color='b')
    plt.ylim(ybottom, ytop)
    plt.xlabel('Intensity')
    plt.yticks([])
    plt.title('Linear Image Histogram')
    plt.savefig(outpath + fname + 'histogram.pdf')


    # Create a record
    df = pd.DataFrame({
        "Filename stem": [fname],
        "Quantization org": [str(img_org.dtype)],
        "Quantization lin": [str(img_lin.dtype)],
        "Quantization min": [np.iinfo(img_org.dtype).min],
        "Quantization max": [np.iinfo(img_org.dtype).max],
        "Dynamic range min org": [img_org.min()],
        "Dynamic range max org": [img_org.max()],
        "Dynamic range min lin": [img_lin.min()],
        "Dynamic range max lin": [img_lin.max()],
        "#Saturated pixels in R min org": [hist[0, 0]],
        "#Saturated pixels in G min org": [hist[1, 0]],
        "#Saturated pixels in B min org": [hist[2, 0]],
        "#Saturated pixels in R max org": [hist[0, -1]],
        "#Saturated pixels in G max org": [hist[1, -1]],
        "#Saturated pixels in B max org": [hist[2, -1]],
        "#Saturated pixels in R min lin": [hist_lin[0, 0]],
        "#Saturated pixels in G min lin": [hist_lin[1, 0]],
        "#Saturated pixels in B min lin": [hist_lin[2, 0]],
        "#Saturated pixels in R max lin": [hist_lin[0, -1]],
        "#Saturated pixels in G max lin": [hist_lin[1, -1]],
        "#Saturated pixels in B max lin": [hist_lin[2, -1]],
        "Resolution in #pixels": [img_org.shape[0]*img_org.shape[1]],
        "%Saturated pixels org": [np.min(hist[:, -1]) / (img_org.shape[0]*img_org.shape[1]) * 100],
        "%Saturated pixels lin": [np.min(hist_lin[:, -1]) / (img_lin.shape[0]*img_lin.shape[1]) * 100]
    })
    return df


def main():
    parser = argparse.ArgumentParser(description='Check saturation of herbarium images')
    parser.add_argument('--inpath', type=str, help='Path to the input images', default="/Users/kimstp/Documents/NHMD/data/Herbarium/calibration/LinearCurveComparison/")
    parser.add_argument('--outpath', type=str, help='Path to save the output', default="./output/")
    parser.add_argument('fname', type=str, nargs='*', help='Filename of the image to be checked')
    args = parser.parse_args()

    shutterspeeds = ['115', '120', '130']

    master_table = empty_dataframe()

    for fname_stem in args.fname:
        for shutterspeed in shutterspeeds:
            img_org = imread(pathlib.Path(args.inpath, fname_stem + '_' + shutterspeed + '.tif'), plugin='tifffile')
            img_lin = imread(pathlib.Path(args.inpath, fname_stem + '_' + shutterspeed + 'edit.tif'), plugin='tifffile')
            df = check_saturation(fname_stem + '_' + shutterspeed, img_org, img_lin, outpath=args.outpath)

            # Add to master table
            master_table = pd.concat([master_table, df], axis=0, ignore_index=True)

    # Write Excel sheet to disk
    master_table.to_excel(pathlib.Path(args.outpath, "saturation_stats.xlsx").as_posix(), index=False)

    plt.show()


# Main script
if __name__ == "__main__":
    #fname_stem = 'NHMD_Herb01_9'
    main()


