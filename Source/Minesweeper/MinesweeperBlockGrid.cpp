// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MinesweeperBlockGrid.h"
#include "MinesweeperBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AMinesweeperBlockGrid::AMinesweeperBlockGrid() : IsInGameOverState(false)
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);


	TimerText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TimerText0"));
	TimerText->SetRelativeLocation(FVector(350, -300.f, 0.f));
	TimerText->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	TimerText->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));
	TimerText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Seconds: {0}"), FText::AsNumber(0)));
	TimerText->SetupAttachment(DummyRoot);


}

void AMinesweeperBlockGrid::CreateRandomMinesPositions()
{
	int candidate;
	bool isFindedNewPos = false;
	for (int i = 0; i < MinesCount; i++)
	{
		while (!isFindedNewPos)
		{
			// Search new random but not repead positions.
			isFindedNewPos = true;
			candidate = FMath::RandRange(0, SIZE_SECUENCE - 1);
			for (int j = 0; j < i; j++)
			{
				if (minesPositions[j] == candidate)
				{
					isFindedNewPos = false;
					break;
				}
			}
		}
		minesPositions[i] = candidate;
		isFindedNewPos = false;
	}
}

bool AMinesweeperBlockGrid::IsMinePosition(int position)
{
	bool isFinded = false;
	for (int i = 0; i < MinesCount; i++)
	{
		if (minesPositions[i] == position)
		{
			isFinded = true;
			break;
		}
	}
	return isFinded;
}

int AMinesweeperBlockGrid::NumberRow(int indexBlock)
{
	return indexBlock / ColCount;
}

int AMinesweeperBlockGrid::NumberCol(int indexBlock)
{
	return indexBlock % ColCount;
}

int AMinesweeperBlockGrid::CalcIndexBlock(int row, int col)
{
	return (ColCount * row) + col;
}

bool AMinesweeperBlockGrid::IsAdjMinePosition(int position)
{
	int row = NumberRow(position), rowMine;
	int col = NumberCol(position), colMine;

	for (int i = 0; i < MinesCount; i++)
	{
		rowMine = NumberRow(minesPositions[i]);
		colMine = NumberCol(minesPositions[i]);
		
		if (((row == rowMine - 1) && (col == colMine - 1)) || ((row == rowMine - 1) && (col == colMine))
				|| ((row == rowMine - 1) && (col == colMine + 1)) || ((row == rowMine) && (col == colMine - 1))
				|| ((row == rowMine) && (col == colMine + 1)) || ((row == rowMine + 1) && (col == colMine - 1))
				|| ((row == rowMine + 1) && (col == colMine)) || ((row == rowMine + 1) && (col == colMine + 1)))
			return true;
	}

	return false;
}

int AMinesweeperBlockGrid::NumberAdjMinePosition(int position)
{
	int row = NumberRow(position), rowMine;
	int col = NumberCol(position), colMine;
	int minesCounter = 0;

	for (int i = 0; i < MinesCount; i++)
	{
		rowMine = NumberRow(minesPositions[i]);
		colMine = NumberCol(minesPositions[i]);

		if (((row == rowMine - 1) && (col == colMine - 1)) || ((row == rowMine - 1) && (col == colMine))
			|| ((row == rowMine - 1) && (col == colMine + 1)) || ((row == rowMine) && (col == colMine - 1))
			|| ((row == rowMine) && (col == colMine + 1)) || ((row == rowMine + 1) && (col == colMine - 1))
			|| ((row == rowMine + 1) && (col == colMine)) || ((row == rowMine + 1) && (col == colMine + 1)))
		{
			minesCounter++;
		}
	}

	return minesCounter;
}

void AMinesweeperBlockGrid::CreateSequenceBlockValues()
{
	// Create NUMBER_MINES (10) mines in random position in array.
	CreateRandomMinesPositions();

	// Create 8 numbers arround mines (not write in a mine square).
	for (int BlockIndex = 0; BlockIndex < SIZE_SECUENCE; BlockIndex++)
	{
		if (IsMinePosition(BlockIndex))
			SecuenceBlockValue[BlockIndex] = -1;
		else if (IsAdjMinePosition(BlockIndex))
			SecuenceBlockValue[BlockIndex] = NumberAdjMinePosition(BlockIndex);
	}
}


void AMinesweeperBlockGrid::RepeatingFunction() {
	UE_LOG(LogTemp, Warning, TEXT("CountDown = %d"), CountDownSeconds);

	TimerText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Seconds: {0}"), FText::AsNumber(CountDownSeconds)));
	// Once we've called this function enough times, clear the Timer.
	if (--CountDownSeconds <= 0)
	{
		SetGameOverState();
		// MemberTimerHandle can now be reused for any other Timer.
	}

}

void AMinesweeperBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(timerHandle, this, &AMinesweeperBlockGrid::RepeatingFunction, 1.0f, true, 1.0f);

	// Set defaults
	BlockSpacing = 150.0f;
	SIZE_SECUENCE = RowCount * ColCount;

	SecuenceBlockValue.resize(SIZE_SECUENCE, 0);
	minesPositions.resize(MinesCount, 0);

	CreateSequenceBlockValues();

	// Number of blocks
	//const int32 NumBlocks = Size * Size;
	UE_LOG(LogTemp, Warning, TEXT("MinesCount = %d, SecuenceBlockValue.size = %d"), MinesCount, SecuenceBlockValue.size());
	UE_LOG(LogTemp, Warning, TEXT("ColCount = %d, RowCount = %d, SIZE_SECUENCE = %d"), ColCount, RowCount, SIZE_SECUENCE);

	// Loop to spawn each block
	for(int32 BlockIndex=0; BlockIndex < SIZE_SECUENCE; BlockIndex++)
	{
		const float XOffset = (BlockIndex % ColCount) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex / ColCount) * BlockSpacing; // Modulo gives remainder

		UE_LOG(LogTemp, Warning, TEXT("XOffset = %f, YOffset = %f"), XOffset, YOffset);

		// Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		AMinesweeperBlock* NewBlock = GetWorld()->SpawnActor<AMinesweeperBlock>(BlockLocation, FRotator(0,0,0));

		// Tell the block about its owner
		if (NewBlock != nullptr)
		{
			NewBlock->OwningGrid = this;
			//NewBlock->SetMineValue(FMath::RandRange(-1, 8));
			NewBlock->SetMineValue(SecuenceBlockValue[BlockIndex]);
		}
	}
}


void AMinesweeperBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

void AMinesweeperBlockGrid::SetGameOverState()
{
	TimerText->SetText(FText::FromString("Game Over!"));
	GetWorldTimerManager().ClearTimer(timerHandle);
	IsInGameOverState = true;
}

bool AMinesweeperBlockGrid::IsInGameOver() const
{
	return IsInGameOverState;
}

#undef LOCTEXT_NAMESPACE
