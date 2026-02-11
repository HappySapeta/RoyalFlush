#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DlgSystem/DlgDialogueParticipant.h"
#include "DialogueComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROYALFLUSH_API UDialogueComponent : public UActorComponent, public IDlgDialogueParticipant
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueComponent();
 
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
