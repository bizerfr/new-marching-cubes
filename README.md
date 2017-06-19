# new-marching-cubes

算法基于文章 "何晖光， 田捷， 赵明昌， 杨骅． 基于分割的三维医学图像表面重建算法[J]． 软件学报， 2002， 02： 219-226"

需要安装VTK6.2

NewMarchingCubes.cxx是程序main函数入口







This code is based on the paper "A 3D Medical Imaging Surface Reconstruction Scheme Based on Segmentation".
VTK6.2 is required.
"NewMarchingCubes.cxx" contains the main function. 
It is a revised version of Marching cubes for medical imaging, and is encapsulated into a class vtkMarchingCubesSpace.
The memberfuction "SetInputData" of this class has two arguments, for the first argument, 0 represents the binary image set(used to extract the triangular surface), while 1 represents the original image set(used to compute normals).
