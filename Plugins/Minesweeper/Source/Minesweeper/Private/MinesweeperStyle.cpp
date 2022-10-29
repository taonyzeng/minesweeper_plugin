// © 2021, Matthew Barham. All rights reserved.

#include "MinesweeperStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr< FSlateStyleSet > FMinesweeperStyle::StyleInstance = NULL;

void FMinesweeperStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();

		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMinesweeperStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMinesweeperStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MinesweeperStyle"));
	return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

#define MINE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define ONE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TWO_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define THREE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define PLUS_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define FLAG_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon32x32(32.0f, 32.0f);

TSharedRef< FSlateStyleSet > FMinesweeperStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MinesweeperStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("Minesweeper")->GetBaseDir() / TEXT("Resources"));

	Style->Set("Minesweeper.OpenPluginWindow", new IMAGE_BRUSH(TEXT("mine"), Icon32x32));
	
	Style->Set("Minesweeper.MineBrush", new MINE_BRUSH(TEXT("mine"), Icon32x32));
	Style->Set("Minesweeper.OneBrush", new MINE_BRUSH(TEXT("one"), Icon32x32));
	Style->Set("Minesweeper.TwoBrush", new MINE_BRUSH(TEXT("two"), Icon32x32));
	Style->Set("Minesweeper.ThreeBrush", new MINE_BRUSH(TEXT("three"), Icon32x32));
	Style->Set("Minesweeper.FourBrush", new MINE_BRUSH(TEXT("four"), Icon32x32));
	Style->Set("Minesweeper.FiveBrush", new MINE_BRUSH(TEXT("five"), Icon32x32));
	Style->Set("Minesweeper.SixBrush", new MINE_BRUSH(TEXT("six"), Icon32x32));
	Style->Set("Minesweeper.SevenBrush", new MINE_BRUSH(TEXT("seven"), Icon32x32));
	Style->Set("Minesweeper.EightBrush", new MINE_BRUSH(TEXT("eight"), Icon32x32));
	Style->Set("Minesweeper.PlusBrush", new MINE_BRUSH(TEXT("plus"), Icon32x32));
	Style->Set("Minesweeper.FlagBrush", new MINE_BRUSH(TEXT("flag"), Icon32x32));

	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FMinesweeperStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMinesweeperStyle::Get()
{
	return *StyleInstance;
}
