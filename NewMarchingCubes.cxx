#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkMarchingCubes.h>
#include <vtkVoxelModeller.h>
#include <vtkSphereSource.h>
#include <vtkImageData.h>
#include "vtkPNGReader.h"
#include <vtkSmoothPolyDataFilter.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkStripper.h>
#include "vtkMarchingCubesSpace.h"



int main(int argc, char *argv[])
{
	vtkSmartPointer<vtkImageData> OriginalVolume =vtkSmartPointer<vtkImageData>::New();
	vtkSmartPointer<vtkImageData> BinaryVolume =vtkSmartPointer<vtkImageData>::New();

	double isoValue=127;


	// 新建文件读取对象，常见的有vtkBMPReader、vtkDICOMImageReader、vtkJPEGReader等
	vtkSmartPointer<vtkPNGReader> BinaryPNGReader =vtkSmartPointer<vtkPNGReader>::New();  
	// 不同的reader需要设置的参数是不同的 因此本例仅适合jpegreader
	BinaryPNGReader->SetFilePrefix("G:/GraduationDesignNeeded/ExperimentalData/Interpolation/BinaryImage16/P01/"); // 要打开的路径
	BinaryPNGReader->SetFilePattern("%s%03d.png");// 图片文件名格式，此处为 001.png 015.png ...
	BinaryPNGReader->SetDataByteOrderToLittleEndian();
	BinaryPNGReader->SetDataSpacing(1, 1, 1);  // 设置三维数据场中像素间距，
	BinaryPNGReader->SetFileNameSliceSpacing(1); 
	BinaryPNGReader->SetDataExtent(0, 109, 0, 109, 1, 30);   //多设置一层，可使顶层有造一个面出来，又能延拓一个连通区域
	// 这里因为在P01文件夹里面有0.png ~ 15.png，所以设置为 1，29
	// 每张图片的长宽为110 * 110 因此设置为0，109
	BinaryPNGReader->Update();  
	BinaryVolume->DeepCopy(BinaryPNGReader->GetOutput());


	// 新建文件读取对象，常见的有vtkBMPReader、vtkDICOMImageReader、vtkJPEGReader等
	vtkSmartPointer<vtkPNGReader> OriginalPNGReader =vtkSmartPointer<vtkPNGReader>::New();  
	// 不同的reader需要设置的参数是不同的 因此本例仅适合jpegreader
	OriginalPNGReader->SetFilePrefix("G:/GraduationDesignNeeded/ExperimentalData/Interpolation/OriginalImage/P01/"); // 要打开的路径
	OriginalPNGReader->SetFilePattern("%s%03d.png");// 图片文件名格式，此处为 001.png 015.png ...
	OriginalPNGReader->SetDataByteOrderToLittleEndian();
	OriginalPNGReader->SetDataSpacing(1, 1, 1);  // 设置三维数据场中像素间距，
	OriginalPNGReader->SetFileNameSliceSpacing(1); 
	OriginalPNGReader->SetDataExtent(0, 109, 0, 109, 1, 30); //多设置一层，可使顶层有造一个面出来，又能延拓一个连通区域
	// 这里因为在P01文件夹里面有0.png ~ 57.png，所以设置为 1，29
	// 每张图片的长宽为110 * 110 因此设置为0，109
	OriginalPNGReader->Update(); 

	OriginalVolume->DeepCopy(OriginalPNGReader->GetOutput());

	


	vtkSmartPointer<vtkMarchingCubesSpace> surface = vtkSmartPointer<vtkMarchingCubesSpace>::New();

#if VTK_MAJOR_VERSION <= 5
  surface->SetInput(BinaryVolume);   //5以前版本不支持
#else
  surface->SetInputData(0,BinaryVolume);    //0号端口作为二值化图像输入
  surface->SetInputData(1,OriginalVolume);  //1号端口作为原始图像输入端口,如果0号和1号端口都输入同一个数据，法向量计算不变
#endif
  surface->ComputeNormalsOn();
  //surface->ComputeNormalsOff();  //如果用off法向量计算默认为输入二值化图像的法向量计算，默认为on
  surface->SetValue(0, isoValue);


  vtkSmartPointer<vtkStripper> stripper = 
      vtkSmartPointer<vtkStripper>::New();
  stripper->SetInputConnection(surface->GetOutputPort());  //将生成的三角片连接成三角带 
  stripper->Update();


  vtkSmartPointer<vtkSmoothPolyDataFilter> smoothFilter =
	  vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
  smoothFilter->SetInputConnection(stripper->GetOutputPort());
  smoothFilter->SetNumberOfIterations(5);
  smoothFilter->Update();


  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(smoothFilter->GetOutputPort());
  mapper->ScalarVisibilityOff();

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);   actor->GetProperty()->SetColor(1,0,0);


  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
   renderer->SetBackground(1, 1, 1);  

  vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);

  renderWindow->Render();
  interactor->Start();

  return EXIT_SUCCESS;
}
