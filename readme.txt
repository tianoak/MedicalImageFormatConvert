1,
ConsoleApplication1.exe "image.kfb" image num
parameter1: absolute file path or relative file path
parameter2: output filename, if num is 2, then the filenames are separately: image0.jpeg, image1.jpeg, image2.jpeg
parameter3: num is the number of partition, from 1 on, horizontally.

2,
convert +append image*.jpeg image.jpeg
parameter:+append means combining images horizontally

3,
convert image.jpeg -define tiff:tile-geometry=256x256 -compress LZW "ptif:image.tif"
parameter:image.jpeg is input image, LZW is compress algorithm,image.tif is output image



