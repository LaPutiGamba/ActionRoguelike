#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARWorldUserWidget.generated.h"

UCLASS(Abstract)
class ACTIONROGUELIKE_API UARWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USizeBox> ParentSizeBox;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<AActor> AttachedActor;
};
