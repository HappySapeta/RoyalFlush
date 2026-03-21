// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "RFClueDataAsset.h"
#include "Components/ActorComponent.h"
#include "RFJournalComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROYALFLUSH_API URFJournalComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCluesSubmittedDelegate);
	
public:

	// Sets default values for this component's properties
	URFJournalComponent();
	
	UFUNCTION(BlueprintCallable)
	int AddClue(const URFClueDataAsset* ClueObject);
	
	UFUNCTION(BlueprintCallable)
	void StageClue(const int ClueIndex);
	
	UFUNCTION(BlueprintCallable)
	void GetStagedClues(TArray<FRFClue>& Out_StagedClues);
	
	UFUNCTION(BlueprintCallable)
	void Accept();
	
	void BindOnCluesSubmitted(FOnCluesSubmittedDelegate Callback);
	
protected:
	
	UPROPERTY(BlueprintAssignable)
	FOnCluesSubmittedDelegate OnCluesSubmittedEvent;
	
private:
	
	UPROPERTY(VisibleAnywhere)
	TArray<FRFClue> Clues;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FRFClue> StagedClues;
	
};
