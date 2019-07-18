This project converts medical image format from kfb to more universal formats like tiff.
kfb format is provided by http://www.kfbio.cn.
For ImageOperationLib.dll, please contact http://www.kfbio.cn/contact.php

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

FAQs:
1, warned that some dll files are missed
install visual studio

2, SHException
the value of "num" in your command is too small

3, where to download convert tool
https://www.imagemagick.org/script/index.php

