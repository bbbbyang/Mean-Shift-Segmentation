#### Mean Shift Segmentation
Author        : Bingyang Liu

[Mean Shift Segmentation][1] should have three steps:

- Filtering (Mode Searching)
- Merge Similar Regions (Mode clustering)
- Merge Small Regions

In the original paper, for step 2, when considered mode clustering, not only color similarity and
also coordinate position should be within the bandwidth correspondingly. Here, we will only consider
color factor to do segmentation.

Filtering process is the main part of the algorithm.

- Determined the Uniform kernel for filtering.
- Based on one point<sup>1</sup>, within the space bandwidth, calculate every point color distance to the center point.
- For all points, which color distance is within the color bandwidth, calculate the shift vector
for the center point and add together<sup>2</sup>. 
- Shift the center point. center = center + shift (include color).
- Repeat above steps and stop after 5 iterations or the center point doesn't move.
- Assigned color to the original point the color. This original point belong to this cluster.

1.Every point will be considered as 5-D point(R, G, B, x, y).

2.Shift value is the average value of all points within the bandwidth.

This code includes two functions.

- Filtering
- Segmentation

Segmentation function is the Mean Shift Segmentation first two process, which has two parts, one is Mean Shift filtering
and other one is flood filled algorithm (Region Growing).

![](https://raw.githubusercontent.com/bbbbyang/PictureRepository/e76a6e52ed0b61c4bbcc68df2f0c3d6898d4d8d2/Mean%20Shift/mandril_color256.jpg)
![](https://raw.githubusercontent.com/bbbbyang/PictureRepository/e76a6e52ed0b61c4bbcc68df2f0c3d6898d4d8d2/Mean%20Shift/16-16Filtering.jpg)
![](https://raw.githubusercontent.com/bbbbyang/PictureRepository/e76a6e52ed0b61c4bbcc68df2f0c3d6898d4d8d2/Mean%20Shift/16-16Segmentation.jpg)

&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;OriginalPic
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;Filtering
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;Segmentation

[Mean Shift: A Robust Approach Toward Feature Space Analysis][1]

[1]:https://courses.csail.mit.edu/6.869/handouts/PAMIMeanshift.pdf

