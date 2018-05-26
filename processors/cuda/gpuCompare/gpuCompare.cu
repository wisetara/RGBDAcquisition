/*
 * Copyright 1993-2015 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

/* A CUDA program that demonstrates how to compute a stereo disparity map using
 *   SIMD SAD (Sum of Absolute Difference) intrinsics
 */

// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// includes, kernels
#include <cuda_runtime.h>
#include "sdkHelper.h"
#include "gpuCompare_kernel.cuh"


// includes, project
#include <helper_functions.h>  // helper for shared that are common to CUDA Samples
#include <helper_cuda.h>       // helper for checking cuda initialization and error checking
#include <helper_string.h>     // helper functions for string parsing


static const char *sSDKsample = "[stereoDisparity]\0";

int iDivUp(int a, int b)
{
    return ((a % b) != 0) ? (a / b + 1) : (a / b);
}


////////////////////////////////////////////////////////////////////////////////
// declaration, forward
void runTest(int argc, char **argv);

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int
main(int argc, char **argv)
{
    runTest(argc, argv);
}

int writeResult(char * file1,char * file2,unsigned int * odata, unsigned int w , unsigned int h )
{
    unsigned int cpuCheckSum = 0;
    for (unsigned int i=0 ; i<w *h ; i++)
    {
        cpuCheckSum += odata[i];
    }
    printf("CPU Checksum = %u, ", cpuCheckSum);

    // write out the resulting disparity image.
    unsigned char *dispOut = (unsigned char *)malloc(w*h);
    char fnameOut[512];
    snprintf(fnameOut,512,"output_CPU_%s_%s.pgm",file1,file2);
    for (unsigned int i=0; i<w*h; i++) { dispOut[i] = (int)odata[i]; }
    for (unsigned int i=0; i<w*h; i++) { dispOut[i] = (int)odata[i]; }

    printf("CPU image: <%s>\n", fnameOut);
    sdkSavePGM(fnameOut, dispOut, w, h);

    free(dispOut);
}



int doCPUonly(int argc, char **argv)
{
    // Load image data
    //allocate mem for the images on host side
    //initialize pointers to NULL to request lib call to allocate as needed
    // PPM images are loaded into 4 byte/pixel memory (RGBX)
    unsigned char *h_img0 = NULL;
    unsigned char *h_img1 = NULL;
    unsigned char *h_img2 = NULL;
    unsigned char *h_img3 = NULL;
    unsigned int w, h;
    char *fname0 = sdkFindFilePath("stereo.im0.640x533.ppm", argv[0]);
    char *fname1 = sdkFindFilePath("stereo.im1.640x533.ppm", argv[0]);
    char *fname2 = sdkFindFilePath("stereo.im2.640x533.ppm", argv[0]);
    char *fname3 = sdkFindFilePath("stereo.im3.640x533.ppm", argv[0]);




    printf("Loaded <%s> as image 0\n", fname0);
    if (!sdkLoadPPM4ub(fname0, &h_img0, &w, &h))    { fprintf(stderr, "Failed to load <%s>\n", fname0); }

    printf("Loaded <%s> as image 1\n", fname1);
    if (!sdkLoadPPM4ub(fname1, &h_img1, &w, &h))    { fprintf(stderr, "Failed to load <%s>\n", fname1); }

    printf("Loaded <%s> as image 2\n", fname1);
    if (!sdkLoadPPM4ub(fname2, &h_img2, &w, &h))    { fprintf(stderr, "Failed to load <%s>\n", fname2); }

    printf("Loaded <%s> as image 2\n", fname3);
    if (!sdkLoadPPM4ub(fname3, &h_img3, &w, &h))    { fprintf(stderr, "Failed to load <%s>\n", fname3); }

    unsigned int numData = w*h;
    unsigned int memSize = sizeof(int) * numData;

    //allocate mem for the result on host side
    unsigned int *h_odata = (unsigned int *)malloc(memSize);

    //initialize the memory

    printf("Computing CPU reference...\n");
    for (unsigned int i = 0; i < numData; i++) h_odata[i] = 0;
    compareImagesCPU((unsigned int *)h_img0, (unsigned int *)h_img1, (unsigned int *)h_odata, w, h);
    writeResult("im0","im1",h_odata,w,h);


    printf("Computing CPU reference...\n");
    for (unsigned int i = 0; i < numData; i++) h_odata[i] = 0;
    compareImagesCPU((unsigned int *)h_img0, (unsigned int *)h_img2, (unsigned int *)h_odata, w, h);
    writeResult("im0","im2",h_odata,w,h);


    printf("Computing CPU reference...\n");
    for (unsigned int i = 0; i < numData; i++) h_odata[i] = 0;
    compareImagesCPU((unsigned int *)h_img0, (unsigned int *)h_img3, (unsigned int *)h_odata, w, h);
    writeResult("im0","im3",h_odata,w,h);



 return 0;
}




////////////////////////////////////////////////////////////////////////////////
//! CUDA Sample for calculating depth maps
////////////////////////////////////////////////////////////////////////////////
void
runTest(int argc, char **argv)
{
    doCPUonly(argc,argv);
    return;



    cudaDeviceProp deviceProp;
    deviceProp.major = 0;
    deviceProp.minor = 0;
    int dev = 0;

    // This will pick the best possible CUDA capable device
    dev = findCudaDevice(argc, (const char **)argv);

    checkCudaErrors(cudaGetDeviceProperties(&deviceProp, dev));

    // Statistics about the GPU device
    printf("> GPU device has %d Multi-Processors, SM %d.%d compute capabilities\n\n",
           deviceProp.multiProcessorCount, deviceProp.major, deviceProp.minor);

    int version = (deviceProp.major * 0x10 + deviceProp.minor);

    if (version < 0x20)
    {
        printf("%s: requires a minimum CUDA compute 2.0 capability\n", sSDKsample);
        exit(EXIT_SUCCESS);
    }

    StopWatchInterface *timer;
    sdkCreateTimer(&timer);

    // Search parameters
    int minDisp = -16;
    int maxDisp = 0;

    // Load image data
    //allocate mem for the images on host side
    //initialize pointers to NULL to request lib call to allocate as needed
    // PPM images are loaded into 4 byte/pixel memory (RGBX)
    unsigned char *h_img0 = NULL;
    unsigned char *h_img1 = NULL;
    unsigned int w, h;
    char *fname0 = sdkFindFilePath("stereo.im0.640x533.ppm", argv[0]);
    char *fname1 = sdkFindFilePath("stereo.im1.640x533.ppm", argv[0]);

    printf("Loaded <%s> as image 0\n", fname0);

    if (!sdkLoadPPM4ub(fname0, &h_img0, &w, &h))
    {
        fprintf(stderr, "Failed to load <%s>\n", fname0);
    }

    printf("Loaded <%s> as image 1\n", fname1);

    if (!sdkLoadPPM4ub(fname1, &h_img1, &w, &h))
    {
        fprintf(stderr, "Failed to load <%s>\n", fname1);
    }

    dim3 numThreads = dim3(blockSize_x, blockSize_y, 1);
    dim3 numBlocks = dim3(iDivUp(w, numThreads.x), iDivUp(h, numThreads.y));
    unsigned int numData = w*h;
    unsigned int memSize = sizeof(int) * numData;

    //allocate mem for the result on host side
    unsigned int *h_odata = (unsigned int *)malloc(memSize);

    //initialize the memory
    for (unsigned int i = 0; i < numData; i++)
        h_odata[i] = 0;

    // allocate device memory for result
    unsigned int *d_odata, *d_img0, *d_img1;

    checkCudaErrors(cudaMalloc((void **) &d_odata, memSize));
    checkCudaErrors(cudaMalloc((void **) &d_img0, memSize));
    checkCudaErrors(cudaMalloc((void **) &d_img1, memSize));

    // copy host memory to device to initialize to zeros
    checkCudaErrors(cudaMemcpy(d_img0,  h_img0, memSize, cudaMemcpyHostToDevice));
    checkCudaErrors(cudaMemcpy(d_img1,  h_img1, memSize, cudaMemcpyHostToDevice));
    checkCudaErrors(cudaMemcpy(d_odata, h_odata, memSize, cudaMemcpyHostToDevice));

    size_t offset = 0;
    cudaChannelFormatDesc ca_desc0 = cudaCreateChannelDesc<unsigned int>();
    cudaChannelFormatDesc ca_desc1 = cudaCreateChannelDesc<unsigned int>();

    tex2Dleft.addressMode[0] = cudaAddressModeClamp;
    tex2Dleft.addressMode[1] = cudaAddressModeClamp;
    tex2Dleft.filterMode     = cudaFilterModePoint;
    tex2Dleft.normalized     = false;
    tex2Dright.addressMode[0] = cudaAddressModeClamp;
    tex2Dright.addressMode[1] = cudaAddressModeClamp;
    tex2Dright.filterMode     = cudaFilterModePoint;
    tex2Dright.normalized     = false;
    checkCudaErrors(cudaBindTexture2D(&offset, tex2Dleft,  d_img0, ca_desc0, w, h, w*4));
    assert(offset == 0);

    checkCudaErrors(cudaBindTexture2D(&offset, tex2Dright, d_img1, ca_desc1, w, h, w*4));
    assert(offset == 0);

    // First run the warmup kernel (which we'll use to get the GPU in the correct max power state
    compareImagesKernel<<<numBlocks, numThreads>>>(d_img0, d_img1, d_odata, w, h, minDisp, maxDisp);
    cudaDeviceSynchronize();

    // Allocate CUDA events that we'll use for timing
    cudaEvent_t start, stop;
    checkCudaErrors(cudaEventCreate(&start));
    checkCudaErrors(cudaEventCreate(&stop));

    printf("Launching CUDA compareImagesKernel()\n");

    // Record the start event
    checkCudaErrors(cudaEventRecord(start, NULL));

    // launch the stereoDisparity kernel
    compareImagesKernel<<<numBlocks, numThreads>>>(d_img0, d_img1, d_odata, w, h, minDisp, maxDisp);

    // Record the stop event
    checkCudaErrors(cudaEventRecord(stop, NULL));

    // Wait for the stop event to complete
    checkCudaErrors(cudaEventSynchronize(stop));

    // Check to make sure the kernel didn't fail
    getLastCudaError("Kernel execution failed");

    float msecTotal = 0.0f;
    checkCudaErrors(cudaEventElapsedTime(&msecTotal, start, stop));

    //Copy result from device to host for verification
    checkCudaErrors(cudaMemcpy(h_odata, d_odata, memSize, cudaMemcpyDeviceToHost));

    printf("Input Size  [%dx%d], ", w, h);
    printf("Kernel size [%dx%d], ", (2*RAD+1), (2*RAD+1));
    printf("Disparities [%d:%d]\n", minDisp, maxDisp);

    printf("GPU processing time : %.4f (ms)\n", msecTotal);
    printf("Pixel throughput    : %.3f Mpixels/sec\n", ((float)(w *h*1000.f)/msecTotal)/1000000);

    // calculate sum of resultant GPU image
    unsigned int checkSum = 0;

    for (unsigned int i=0 ; i<w *h ; i++)
    {
        checkSum += h_odata[i];
    }

    printf("GPU Checksum = %u, ", checkSum);

    // write out the resulting disparity image.
    unsigned char *dispOut = (unsigned char *)malloc(numData);
    int mult = 20;
    const char *fnameOut = "output_GPU.pgm";

    for (unsigned int i=0; i<numData; i++)
    {
        dispOut[i] = (int)h_odata[i]*mult;
    }

    printf("GPU image: <%s>\n", fnameOut);
    sdkSavePGM(fnameOut, dispOut, w, h);

    //compute reference solution
    printf("Computing CPU reference...\n");
    compareImagesCPU((unsigned int *)h_img0, (unsigned int *)h_img1, (unsigned int *)h_odata, w, h);
    unsigned int cpuCheckSum = 0;

    for (unsigned int i=0 ; i<w *h ; i++)
    {
        cpuCheckSum += h_odata[i];
    }

    printf("CPU Checksum = %u, ", cpuCheckSum);
    const char *cpuFnameOut = "output_CPU.pgm";

    for (unsigned int i=0; i<numData; i++)
    {
        dispOut[i] = (int)h_odata[i]*mult;
    }

    printf("CPU image: <%s>\n", cpuFnameOut);
    sdkSavePGM(cpuFnameOut, dispOut, w, h);

    // cleanup memory
    checkCudaErrors(cudaFree(d_odata));
    checkCudaErrors(cudaFree(d_img0));
    checkCudaErrors(cudaFree(d_img1));

    if (h_odata != NULL) free(h_odata);

    if (h_img0 != NULL) free(h_img0);

    if (h_img1 != NULL) free(h_img1);

    if (dispOut != NULL) free(dispOut);

    sdkDeleteTimer(&timer);

    exit((checkSum == cpuCheckSum) ? EXIT_SUCCESS : EXIT_FAILURE);
}
