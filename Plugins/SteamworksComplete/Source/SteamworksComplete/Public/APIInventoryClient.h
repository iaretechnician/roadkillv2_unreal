/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <SteamGameInstance.h>
#include "APIInventoryClient.generated.h"

UCLASS()
class STEAMWORKSCOMPLETE_API UAPIInventoryClient : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSimpleAddPromoItem(int32 itemDef, TArray<FString> readProperties, FItemResultsCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSimpleAddPromoItems(TArray<int32> itemDefs, TArray<FString> readProperties, FItemResultsCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSimpleConsumeItem(int64 itemInstanceId, int32 quantity, TArray<FString> readProperties, FItemResultsCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSimpleDeserializeResult(int64 userId, TArray<uint8> buffer, TArray<FString> readProperties, FItemResultsCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSimpleExchangeItems(int32 itemDef, TArray<FItemCount> recipe, TArray<FString> readProperties, FItemResultsCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSimpleGenerateItems(TArray<FItemCount> items, TArray<FString> readProperties, FItemResultsCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSimpleGetAllItems(TArray<FString> readProperties, FItemResultsCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSimpleGrantPromoItems(TArray<FString> readProperties, FItemResultsCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSimpleTransferItemQuantity(int64 sourceInstanceId, int32 quantity, int64 destinationInstanceId, TArray<FString> readProperties, FItemResultsCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSimpleTriggerItemDrop(int32 itemDef, TArray<FString> readProperties, FItemResultsCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FInventoryResult SteamAddPromoItem(int32 itemDef);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FInventoryResult SteamAddPromoItems(TArray<int32> itemDefs);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamCheckResultSteamID(int32 resultHandle, int64 steamIDExpected);
	
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FInventoryResult SteamConsumeItem(int64 itemInstanceId, int32 quantity);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FInventoryResult SteamDeserializeResult(TArray<uint8> buffer);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamDestroyResult(int32 resultHandle);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FInventoryResult SteamExchangeItems(int32 itemDef, TArray<FItemCount> recipe);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FInventoryResult SteamGenerateItems(TArray<FItemCount> items);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FInventoryResult SteamGetAllItems();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static TArray<FString> SteamGetItemDefinitionProperties(int32 itemDef);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetItemDefinitionProperty(int32 itemDef, FString property);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FInventoryResult SteamGetItemsByID(TArray<int64> instanceIds);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FItemPrice SteamGetItemPrice(int32 itemDef);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static TArray<FItemWithPrice> SteamGetItemsWithPrices();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetNumItemsWithPrices();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetResultItemProperty(int32 resultHandle, int32 index, FString propertyName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static TArray<FItemDetail> SteamGetResultItems(int32 resultHandle);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static UEResult SteamGetResultStatus(int32 resultHandle);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FDateTime SteamGetResultTimestamp(int32 resultHandle);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FInventoryResult SteamGrantPromoItems();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamLoadItemDefinitions();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamRequestEligiblePromoItemDefinitionsIDs(FSteamInventoryEligiblePromoItemDefIDsCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamRequestPrices(FRequestPricesCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static TArray<uint8> SteamSerializeResult(int32 resultHandle);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamStartPurchase(TArray<FItemDefCount> items, FSteamInventoryStartPurchaseResultCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FInventoryResult SteamTransferItemQuantity(int64 sourceInstanceId, int32 quantity, int64 destinationInstanceId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FInventoryResult SteamTriggerItemDrop(int32 itemDef);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamStartUpdateProperties();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FInventoryResult SteamSubmitUpdateProperties(int64 updateHandle);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamRemoveProperty(int64 updateHandle, int64 instanceId, FString propertyName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetPropertyString(int64 updateHandle, int64 instanceId, FString propertyName, FString value);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetPropertyBool(int64 updateHandle, int64 instanceId, FString propertyName, bool value);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetPropertyInt(int64 updateHandle, int64 instanceId, FString propertyName, int64 value);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetPropertyFloat(int64 updateHandle, int64 instanceId, FString propertyName, float value);
};
