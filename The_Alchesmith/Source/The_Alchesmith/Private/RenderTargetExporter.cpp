#include "RenderTargetExporter.h"
#include "ImageUtils.h"
#include "RenderUtils.h"

static int32 const N = 64; //Taille de l'image à l'export

// Petit utilitaire: luminance (0..255)
static FORCEINLINE float Luma(const FColor& C) {
    return 0.2126f * C.R + 0.7152f * C.G + 0.0722f * C.B;
}

// Seuils simples (à ajuster si besoin)
static constexpr float kInkLumCut = 220.f; // en dessous = contenu (traits sombres)
static constexpr int32 kCropMargin = 2;     // marge (pixels) autour de la bbox

TArray<float> URenderTargetExporter::ExportRenderTargetToBinary(UTextureRenderTarget2D* renderTarget) {
    if (renderTarget == NULL) {
        UE_LOG(LogTemp, Warning, TEXT("What the fuck did you do?!"))
            return TArray<float>();
    }

    FTextureRenderTargetResource* resource = renderTarget->GameThread_GetRenderTargetResource();
    TArray<FColor> pixels;
    resource->ReadPixels(pixels);

    int32 const sizex = renderTarget->SizeX;
    int32 const sizey = renderTarget->SizeY;

    // --- NOUVEAU: bbox auto des pixels sombres (fond blanc ignoré) ---
    int32 minX = sizex, minY = sizey, maxX = -1, maxY = -1;
    for (int32 y = 0; y < sizey; ++y) {
        const int32 row = y * sizex;
        for (int32 x = 0; x < sizex; ++x) {
            if (Luma(pixels[row + x]) <= kInkLumCut) { // pixel “encre”
                if (x < minX) minX = x;
                if (y < minY) minY = y;
                if (x > maxX) maxX = x;
                if (y > maxY) maxY = y;
            }
        }
    }
    // si rien détecté: on garde toute l'image
    if (maxX < 0 || maxY < 0) {
        minX = 0; minY = 0; maxX = sizex - 1; maxY = sizey - 1;
    }

    // marge simple + clamp
    minX = FMath::Max(0, minX - kCropMargin);
    minY = FMath::Max(0, minY - kCropMargin);
    maxX = FMath::Min(sizex - 1, maxX + kCropMargin);
    maxY = FMath::Min(sizey - 1, maxY + kCropMargin);

    const int32 srcW = maxX - minX + 1;
    const int32 srcH = maxY - minY + 1;

    // --- NOUVEAU: extraire la zone croppée (à partir du buffer déjà lu) ---
    TArray<FColor> cropped;
    cropped.SetNumUninitialized(srcW * srcH);
    for (int32 y = 0; y < srcH; ++y) {
        const int32 srcY = minY + y;
        FMemory::Memcpy(
            cropped.GetData() + y * srcW,
            pixels.GetData() + srcY * sizex + minX,
            sizeof(FColor) * srcW
        );
    }

    // --- Resize à partir de l’image croppée ---
    TArray<FColor> dstDownScaled;
    dstDownScaled.SetNumUninitialized(N * N);
    FImageUtils::ImageResize(srcW, srcH, cropped, N, N, dstDownScaled, /*bLinearSpace*/ false);

    // --- Binarisation identique (mais tu peux garder en niveaux de gris si tu veux) ---
    TArray<float> binaryImage;
    binaryImage.Reserve(N * N);
    for (const FColor& pixel : dstDownScaled) {
        binaryImage.Add((pixel.R + pixel.G + pixel.B < 250 * 3) ? 0.0f : 1.0f);
        // ou: float g = Luma(pixel)/255.f; binaryImage.Add(g <= 0.86f ? 1.f : 0.f);
    }

    return binaryImage;
}