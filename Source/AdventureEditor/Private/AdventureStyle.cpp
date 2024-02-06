// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureStyle.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

TSharedPtr< FSlateStyleSet > FAdventureStyle::AdventureStyleInstance = NULL;

void FAdventureStyle::Initialize()
{
	if ( !AdventureStyleInstance.IsValid() )
	{
		AdventureStyleInstance = Create();
	}
}

void FAdventureStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*AdventureStyleInstance.Get());
	ensure( AdventureStyleInstance.IsUnique() );
	AdventureStyleInstance.Reset();
}

FName FAdventureStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("UMGStyle"));
	return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define IMAGE_BRUSH_SVG( RelativePath, ... ) FSlateVectorImageBrush(Style->RootToContentDir(RelativePath, TEXT(".svg")), __VA_ARGS__)

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon64x64(64.0f, 64.0f);

TSharedRef< FSlateStyleSet > FAdventureStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("AdventureStyle"));
	Style->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
	Style->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));
	
	#if ENGINE_MAJOR_VERSION == 5

	Style->Set(FName(TEXT("ClassThumbnail.AdventureAssetBlueprint")), new IMAGE_BRUSH_SVG("Starship/AssetIcons/SlateBrushAsset_64", Icon64x64));
	Style->Set(FName(TEXT("ClassIcon.AdventureAssetBlueprint")), new IMAGE_BRUSH_SVG("Starship/AssetIcons/SlateBrushAsset_16", Icon16x16));

	Style->Set(FName(TEXT("ClassThumbnail.AdventureAsset")), new IMAGE_BRUSH_SVG("Starship/AssetIcons/SlateBrushAsset_64", Icon64x64));
	Style->Set(FName(TEXT("ClassIcon.AdventureAsset")), new IMAGE_BRUSH_SVG("Starship/AssetIcons/SlateBrushAsset_16", Icon16x16));

	Style->Set(FName(TEXT("ClassThumbnail.AdventureTriggerBoxVolume")), new IMAGE_BRUSH_SVG("Starship/AssetIcons/TriggerBox_64", Icon64x64));
	Style->Set(FName(TEXT("ClassIcon.AdventureTriggerBoxVolume")), new IMAGE_BRUSH_SVG("Starship/AssetIcons/TriggerBox_16", Icon16x16));

	#endif

	#if ENGINE_MAJOR_VERSION == 4

	Style->Set(FName(TEXT("ClassThumbnail.AdventureTriggerBoxVolume")), new IMAGE_BRUSH("Icons/AssetIcons/TriggerBox_64x", Icon64x64));
	Style->Set(FName(TEXT("ClassIcon.AdventureTriggerBoxVolume")), new IMAGE_BRUSH("Icons/AssetIcons/TriggerBox_16x", Icon16x16));

	Style->Set(FName(TEXT("ClassThumbnail.AdventureTriggerSphereVolume")), new IMAGE_BRUSH("Icons/AssetIcons/TriggerSphere_64x", Icon64x64));
	Style->Set(FName(TEXT("ClassIcon.AdventureTriggerSphereVolume")), new IMAGE_BRUSH("Icons/AssetIcons/TriggerSphere_16x", Icon16x16));

	Style->Set(FName(TEXT("ClassThumbnail.AdventureAssetBlueprint")), new IMAGE_BRUSH("Icons/AssetIcons/SlateBrushAsset_64x", Icon64x64));
	Style->Set(FName(TEXT("ClassIcon.AdventureAssetBlueprint")), new IMAGE_BRUSH("Icons/AssetIcons/SlateBrushAsset_16x", Icon16x16));

	Style->Set(FName(TEXT("ClassThumbnail.AdventureAsset")), new IMAGE_BRUSH("Icons/AssetIcons/SlateBrushAsset_64x", Icon64x64));
	Style->Set(FName(TEXT("ClassIcon.AdventureAsset")), new IMAGE_BRUSH("Icons/AssetIcons/SlateBrushAsset_16x", Icon16x16));

	#endif
	
	
	FSlateStyleRegistry::RegisterSlateStyle(*Style);
	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH

void FAdventureStyle::ReloadTextures()
{
	FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
}

const ISlateStyle& FAdventureStyle::Get()
{
	return *AdventureStyleInstance;
}