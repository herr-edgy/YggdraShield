// Copyright Mateo Egey

#include "YggdraShield.h"

#include "ContentBrowserMenuContexts.h"
#include "ToolMenus.h"
#include "ToolMenuSection.h"
#include "YggdraShieldStyle.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/AssetRegistryTagsContext.h"

#define LOCTEXT_NAMESPACE "FYggdraShieldModule"

void FYggdraShieldModule::StartupModule()
{
	FYggdraShieldStyle::Register();

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FYggdraShieldModule::RegisterMenus));
}

void FYggdraShieldModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FYggdraShieldModule::RegisterMenus()
{
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("ContentBrowser.AssetContextMenu.AssetActionsSubMenu");
	FToolMenuSection& Section = Menu->FindOrAddSection("AssetContextMoveActions");
	FToolMenuEntry& Entry = Section.AddDynamicEntry("AssetManagerEditorViewCommands", FNewToolMenuSectionDelegate::CreateLambda([](FToolMenuSection& InSection)
	{
		UContentBrowserAssetContextMenuContext* Context = InSection.FindContext<UContentBrowserAssetContextMenuContext>();
		if (Context)
		{
			FToolUIActionChoice LockAction(FExecuteAction::CreateLambda([Context]()
			{
				FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
				
				for(FAssetData SelectedAsset : Context->SelectedAssets)
				{
					if(SelectedAsset.GetPackage()->HasAnyPackageFlags(PKG_DisallowExport) == false)
					{
						SelectedAsset.GetPackage()->MarkPackageDirty();
						SelectedAsset.GetPackage()->SetPackageFlags(PKG_DisallowExport);
						AssetRegistryModule.Get().AssetUpdateTags(SelectedAsset.GetAsset(), EAssetRegistryTagsCaller::FullUpdate);
					}
				}
			}));

			FToolUIActionChoice UnlockAction(FExecuteAction::CreateLambda([Context]()
			{
				FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
				
				for(FAssetData SelectedAsset : Context->SelectedAssets)
				{
					if(SelectedAsset.GetPackage()->HasAnyPackageFlags(PKG_DisallowExport))
					{
						SelectedAsset.GetPackage()->MarkPackageDirty();
						SelectedAsset.GetPackage()->ClearPackageFlags(PKG_DisallowExport);
						AssetRegistryModule.Get().AssetUpdateTags(SelectedAsset.GetAsset(), EAssetRegistryTagsCaller::FullUpdate);
					}
				}
			}));
			// Show only the relevant next-state action(s) for the current selection. A single asset yields exactly
			// one entry; a mixed selection (some locked, some not) may legitimately offer both.
			bool bAnyUnlocked = false;
			bool bAnyLocked = false;
			for(const FAssetData& SelectedAsset : Context->SelectedAssets)
			{
				if(UPackage* Package = SelectedAsset.GetPackage())
				{
					if(Package->HasAnyPackageFlags(PKG_DisallowExport))
					{
						bAnyLocked = true;
					}
					else
					{
						bAnyUnlocked = true;
					}
				}
			}

			if(bAnyUnlocked)
			{
				InSection.AddEntry(FToolMenuEntry::InitMenuEntry(FName("Lock"), FText::FromString("Lock Export"), FText::FromString("Lock this asset. Can't export if locked."), FSlateIcon(FYggdraShieldStyle::Get().GetStyleSetName(), "YggdraShield.Lock"), LockAction));
			}

			if(bAnyLocked)
			{
				InSection.AddEntry(FToolMenuEntry::InitMenuEntry(FName("Unlock"), FText::FromString("Unlock Export"), FText::FromString("Unlock this asset. Can export if unlocked."), FSlateIcon(FYggdraShieldStyle::Get().GetStyleSetName(), "YggdraShield.Unlock"), UnlockAction));
			}
		}
	}));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FYggdraShieldModule, YggdraShield)