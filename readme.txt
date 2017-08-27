1，
ConsoleApplication1.exe "image.kfb" image num
参数1：绝对路径或相对路径均可。
参数2：输出文件名，如果num为2，则文件名分别为image0.jpeg, image1.jpeg, image2.jpeg
参数3：num为分割的个数，从1开始试，是水平分割的。

2,
convert +append image*.jpeg image.jpeg
参数：+append 表示水平拼接

3，
convert image.jpeg -define tiff:tile-geometry=256x256 -compress LZW "ptif:image.tif"
参数:image.jpeg为输入图片，LZW为压缩算法，image.tif为输出图片



