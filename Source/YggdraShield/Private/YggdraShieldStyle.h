// Copyright Mateo Egey

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

/**  */
class FYggdraShieldStyle : public FSlateStyleSet
{
public:
	static void Register();
	static void Unregister();
	static void Shutdown();

	/** reloads textures used by slate renderer */
	static void ReloadTextures();

	/** @return The Slate style set for the Shooter game */
	static const FYggdraShieldStyle& Get();

	static void ReinitializeStyle();

private:
	FYggdraShieldStyle();
	
	void InitIcons();
	
	static TSharedPtr<FYggdraShieldStyle> MageHubStyle;
};