// Copyright Mateo Egey

#include "YggdraShieldStyle.h"

#include "SlateMaterialBrush.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidgetBlueprint.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

TSharedPtr<FYggdraShieldStyle> FYggdraShieldStyle::MageHubStyle = nullptr;

void FYggdraShieldStyle::Register()
{
	FSlateStyleRegistry::RegisterSlateStyle(Get());
}

void FYggdraShieldStyle::Unregister()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(Get());
}

void FYggdraShieldStyle::Shutdown()
{
	Unregister();
	MageHubStyle.Reset();
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon32x32(32.0f, 32.0f);
const FVector2D Icon128x128(128.0f, 128.0f);


FYggdraShieldStyle::FYggdraShieldStyle() : FSlateStyleSet(TEXT("YggdraShieldStyle"))
{
	SetContentRoot(FPaths::ProjectPluginsDir() / TEXT("YggdraShield/Content/Slate"));

	InitIcons();
}

void FYggdraShieldStyle::InitIcons()
{
	Set("YggdraShield.Lock", new IMAGE_BRUSH("YggdraShieldLock_Small", Icon32x32));
	Set("YggdraShield.Unlock", new IMAGE_BRUSH("YggdraShieldUnlock_Small", Icon32x32));
}

void FYggdraShieldStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const FYggdraShieldStyle& FYggdraShieldStyle::Get()
{
	if(!MageHubStyle.IsValid())
	{
		MageHubStyle = MakeShareable(new FYggdraShieldStyle());
	}

	return *MageHubStyle;
}

void FYggdraShieldStyle::ReinitializeStyle()
{
	Unregister();
	MageHubStyle.Reset();
	Register();	
}
