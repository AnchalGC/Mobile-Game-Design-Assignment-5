
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDInLevel.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AHUDInLevel : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditAnywhere, Category="InLevelHUD")
	UTexture2D* TxCrosshair;

	UPROPERTY(EditAnywhere, Category="InLevelHUD")
	uint8 CrosshairScale = 2;
	
};
