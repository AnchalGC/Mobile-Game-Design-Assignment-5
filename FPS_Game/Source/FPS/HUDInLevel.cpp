
#include "HUDInLevel.h"

#include "Engine/Canvas.h"

void AHUDInLevel::DrawHUD()
{
	check(TxCrosshair != nullptr);

	FVector2D Center;
	Canvas->GetCenter(Center.X, Center.Y);
	
	FVector2D CrosshairTileLoc = FVector2D(
		Center.X - TxCrosshair->GetSurfaceWidth() * CrosshairScale * 0.5f,
		Center.Y - TxCrosshair->GetSurfaceHeight() * CrosshairScale * 0.5f
	);
	FCanvasTileItem CrosshairTile(CrosshairTileLoc, TxCrosshair->GetResource(), FLinearColor::White);
	CrosshairTile.BlendMode = SE_BLEND_Translucent;
	CrosshairTile.Size *= CrosshairScale;
	Canvas->DrawItem(CrosshairTile);
}
