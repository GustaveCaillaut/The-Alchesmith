// Fill out your copyright notice in the Description page of Project Settings.


#include "RenderTargetExporter.h"
#include "ImageUtils.h"
#include "RenderUtils.h"


static int32 const N = 64; //Taille de l'image à l'export


TArray<float> URenderTargetExporter::ExportRenderTargetToBitmap(UTextureRenderTarget2D* renderTarget) {
	if (renderTarget == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("What the fuck did you do?!"))
			return TArray<float>();
	}

	FTextureRenderTargetResource* resource = renderTarget->GameThread_GetRenderTargetResource();
	TArray<FColor> pixels;

	resource->ReadPixels(pixels);

	int32 const sizex = renderTarget->SizeX;
	int32 const sizey = renderTarget->SizeY;

	int32 const ratioX = sizex / N;
	int32 const ratioY = sizey / N;

	
;

	/*Async(EAsyncExecution::Thread, [pixels = MoveTemp(pixels), sizex, sizey]() -> TArray<float> {

		TArray<float> binaryImage;
		binaryImage.Reserve(N * N);


		TArray<FColor> dstDownScaled;
		dstDownScaled.SetNumUninitialized(N * N);

		//TODO
		FImageUtils::ImageResize(sizex, sizey, pixels, N, N, dstDownScaled, false);

		for (FColor pixel : dstDownScaled) {
			if (pixel.R + pixel.G + pixel.B < 250 * 3) {
				binaryImage.Add(1.0f);
			}
			else {
				binaryImage.Add(0.0f);
			}
		}
		return binaryImage;
	});*/

	return TArray<float>();
}
