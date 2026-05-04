// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "RFClueDataAsset.h"
#include "Components/ActorComponent.h"
#include "RoyalFlush/UI/RFJournalWidget.h"
#include "RFJournalComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROYALFLUSH_API URFJournalComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCluesSubmittedDelegate);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClueStagedUnstageDelegate, FRFClue, Clue);
	
public:

	// Sets default values for this component's properties
	URFJournalComponent();
	
	UFUNCTION(BlueprintCallable)
	int GetNumCluesCollected() const;
	
	UFUNCTION(BlueprintCallable)
	void AddClue(FRFClue ClueData);
	
	UFUNCTION(BlueprintCallable)
	void StageClue(FRFClue ClueData);
	
	UFUNCTION(BlueprintCallable)
	void UnStageClue(FRFClue ClueData);
	
	UFUNCTION(BlueprintCallable)
	TArray<FRFClue> GetUnstagedClues() const;
	
	UFUNCTION(BlueprintCallable)
	TArray<FRFClue> GetStagedClues();
	
	UFUNCTION(BlueprintCallable)
	void Accept();
	
	void BindOnCluesSubmitted(FOnCluesSubmittedDelegate Callback);

public:
	
	UPROPERTY(BlueprintAssignable)
	FOnClueStagedUnstageDelegate OnClueStagedEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnClueStagedUnstageDelegate OnClueUnStagedEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnCluesSubmittedDelegate OnCluesSubmittedEvent;

protected:
	
	UPROPERTY(EditDefaultsOnly)
	int MaxNumStagedClues = 6;
	
private:
	
	UPROPERTY(VisibleAnywhere)
	TArray<FRFClue> Clues;
	
	int NumStagedClues = 0;
};
