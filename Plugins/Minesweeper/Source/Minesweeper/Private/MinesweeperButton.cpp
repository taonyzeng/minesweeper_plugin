// © 2021, Matthew Barham. All rights reserved.

#include "MinesweeperButton.h"

#include "Minesweeper.h"
#include "MinesweeperStyle.h"
#include "SlateOptMacros.h"
#include "Components/Button.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMinesweeperButton::Construct(const FArguments& InArgs)
{
	Visibility = EVisibility::SelfHitTestInvisible;

	bIsClicked = false;

	bIsMine = InArgs._bIsMine;
	MinesweeperModule = InArgs._MinesweeperModule;
	Coordinates = InArgs._Coordinates;

	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(50.f)
		.HeightOverride(50.f)

		[
			SNew(SOverlay)

			+ SOverlay::Slot()
			[
				SAssignNew(Button, SButton)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.OnClicked_Raw(this, &SMinesweeperButton::HandleClick)
			]

			+ SOverlay::Slot()
			  .HAlign(HAlign_Center)
			  .VAlign(VAlign_Center)
			[
				SAssignNew(ButtonImage, SImage)
				.Visibility(EVisibility::Hidden)
			]
		]
	];

	if (bIsMine)
	{
		ButtonImage.Get()->SetImage(FMinesweeperStyle::Get().GetBrush("Minesweeper.MineBrush"));
	}
}

void SMinesweeperButton::SetMine(bool const bSetMine)
{
	bIsMine = bSetMine;
	ButtonImage.Get()->SetImage(FMinesweeperStyle::Get().GetBrush("Minesweeper.MineBrush"));
}

void SMinesweeperButton::RevealMine() const
{
	if (bIsMine)
	{
		ButtonImage.Get()->SetVisibility(EVisibility::HitTestInvisible);
	}
}

void SMinesweeperButton::RevealAdjacency() const
{
	if (MinesweeperModule)
	{
		MinesweeperModule->RevealAdjacentButtons(Coordinates);
	}
}

void SMinesweeperButton::SetClicked()
{
	Button->SetEnabled(false);
	bIsClicked = true;
}

void SMinesweeperButton::SetNeighborMineCount(int const Count) const
{
	if (Count < 1) return;

	if (Count == 1)
	{
		ButtonImage.Get()->SetImage(FMinesweeperStyle::Get().GetBrush("Minesweeper.OneBrush"));
		ButtonImage->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count == 2)
	{
		ButtonImage.Get()->SetImage(FMinesweeperStyle::Get().GetBrush("Minesweeper.TwoBrush"));
		ButtonImage->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count == 3)
	{
		ButtonImage.Get()->SetImage(FMinesweeperStyle::Get().GetBrush("Minesweeper.ThreeBrush"));
		ButtonImage->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count == 4)
	{
		ButtonImage.Get()->SetImage(FMinesweeperStyle::Get().GetBrush("Minesweeper.FourBrush"));
		ButtonImage->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count == 5)
	{
		ButtonImage.Get()->SetImage(FMinesweeperStyle::Get().GetBrush("Minesweeper.FiveBrush"));
		ButtonImage->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count == 6)
	{
		ButtonImage.Get()->SetImage(FMinesweeperStyle::Get().GetBrush("Minesweeper.SixBrush"));
		ButtonImage->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count == 7)
	{
		ButtonImage.Get()->SetImage(FMinesweeperStyle::Get().GetBrush("Minesweeper.SevenBrush"));
		ButtonImage->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count == 8)
	{
		ButtonImage.Get()->SetImage(FMinesweeperStyle::Get().GetBrush("Minesweeper.EightBrush"));
		ButtonImage->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count > 8)
	{
		ButtonImage.Get()->SetImage(FMinesweeperStyle::Get().GetBrush("Minesweeper.PlusBrush"));
		ButtonImage->SetVisibility(EVisibility::HitTestInvisible);
	}
}

bool SMinesweeperButton::IsAdjacent(FVector2D const InCoordinates)
{
	if (Coordinates.X == InCoordinates.X && (Coordinates.Y == InCoordinates.Y + 1 || Coordinates.Y == InCoordinates.Y -
			1) ||
		Coordinates.Y == InCoordinates.Y && (Coordinates.X == InCoordinates.X + 1 || Coordinates.X == InCoordinates.X -
			1) ||
		Coordinates.X == InCoordinates.X - 1 && Coordinates.Y == InCoordinates.Y - 1 ||
		Coordinates.X == InCoordinates.X - 1 && Coordinates.Y == InCoordinates.Y + 1 ||
		Coordinates.X == InCoordinates.X + 1 && Coordinates.Y == InCoordinates.Y + 1 ||
		Coordinates.X == InCoordinates.X + 1 && Coordinates.Y == InCoordinates.Y - 1
	)
	{
		return true;
	}
	return false;
}

FReply SMinesweeperButton::HandleClick()
{
	SetClicked();

	if (MinesweeperModule)
	{
		/** If this is the first turn, don't click a mine, move it somewhere else */
		if (MinesweeperModule->IsFirstTurn())
		{
			bIsMine = false;
			MinesweeperModule->MoveMineElsewhereOnFirstTurn();
		}
		
		if (bIsMine)
		{
			RevealMine();
			MinesweeperModule->MineClicked();
			return FReply::Handled();
		}

		auto const Count = MinesweeperModule->CountAdjacentMines(Coordinates);
		if (Count == 0)
		{
			RevealAdjacency();
		}
		else
		{
			SetNeighborMineCount(Count);
		}
	}

	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
