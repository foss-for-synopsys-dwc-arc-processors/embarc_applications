/*******************************************************************************
*                                                                              *
*   PrimeSense NiTE 2.0 - User Viewer Sample                                   *
*   Copyright (C) 2012 PrimeSense Ltd.                                         *
*                                                                              *
*******************************************************************************/

#include "Viewer.h"
#include <iostream>
//#pragma comment(lib,"libftdi1.dll.a")
//#pragma comment(lib,"libusb-1.0.dll.a")
int main(int argc, char** argv)
{
	openni::Status rc = openni::STATUS_OK;

  std::cout << "dfl dr"  << std::endl;
	SampleViewer sampleViewer("User Viewer");

	rc = sampleViewer.Init(argc, argv);
	if (rc != openni::STATUS_OK)
	{
		return 1;
	}
	sampleViewer.Run();
}
