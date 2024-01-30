/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#include "APIInventoryClient.h"

bool UAPIInventoryClient::SteamSimpleAddPromoItem(int32 itemDef, TArray<FString> readProperties, FItemResultsCallback callback)
{
	STEAM_CLIENT_INSTANCE(false);
	FItemResultRequest request = FItemResultRequest();
	request.properties = readProperties;
	request.callback = callback;
	TArray<SteamItemDef_t> steamItemDefs;

	SteamInventoryResult_t handle;
	bool response = SteamInventory()->AddPromoItem(&handle, SteamItemDef_t(itemDef));
	if (response)
	{
		SteamGameInstance->InventoryRequests.Add(handle, request);
	}
	return response;
}

bool UAPIInventoryClient::SteamSimpleAddPromoItems(TArray<int32> itemDefs, TArray<FString> readProperties, FItemResultsCallback callback)
{
	STEAM_CLIENT_INSTANCE(false);
	FItemResultRequest request = FItemResultRequest();
	request.properties = readProperties;
	request.callback = callback;

	TArray<SteamItemDef_t> steamItemDefs;
	for (const int32& itemDef : itemDefs)
	{
		steamItemDefs.Add(static_cast<SteamItemDef_t>(itemDef));
	}

	SteamInventoryResult_t handle;
	bool response = SteamInventory()->AddPromoItems(&handle, steamItemDefs.GetData(), steamItemDefs.Num());
	if (response)
	{
		SteamGameInstance->InventoryRequests.Add(handle, request);
	}
	return response;
}

bool UAPIInventoryClient::SteamSimpleConsumeItem(int64 itemInstanceId, int32 quantity, TArray<FString> readProperties, FItemResultsCallback callback)
{
	STEAM_CLIENT_INSTANCE(false);
	FItemResultRequest request = FItemResultRequest();
	request.properties = readProperties;
	request.callback = callback;

	SteamItemInstanceID_t steamItemInstanceId = SteamItemInstanceID_t(itemInstanceId);
	SteamInventoryResult_t handle;
	bool response = SteamInventory()->ConsumeItem(&handle, steamItemInstanceId, static_cast<uint32>(quantity));
	if (response)
	{
		SteamGameInstance->InventoryRequests.Add(handle, request);
	}
	return response;
}

bool UAPIInventoryClient::SteamSimpleDeserializeResult(int64 userId, TArray<uint8> buffer, TArray<FString> readProperties, FItemResultsCallback callback)
{
	STEAM_CLIENT_INSTANCE(false);
	FItemResultRequest request = FItemResultRequest();
	request.properties = readProperties;
	request.callback = callback;
	request.userId = INT64_TO_STEAMID(userId);

	SteamInventoryResult_t handle;
	bool response = SteamInventory()->DeserializeResult(&handle, buffer.GetData(), buffer.Num());
	if (response)
	{
		SteamGameInstance->InventoryRequests.Add(handle, request);
	}
	return response;
}

bool UAPIInventoryClient::SteamSimpleExchangeItems(int32 itemDef, TArray<FItemCount> recipe, TArray<FString> readProperties, FItemResultsCallback callback)
{
	STEAM_CLIENT_INSTANCE(false);
	FItemResultRequest request = FItemResultRequest();
	request.properties = readProperties;
	request.callback = callback;

	TArray<SteamItemDef_t> generateItems;
	TArray<uint32> generateQuantities;
	TArray<SteamItemInstanceID_t> destroyItems;
	TArray<uint32> destroyQuantities;

	for (const FItemCount& item : recipe)
	{
		if (item.instanceId > 0)
		{
			generateItems.Add(static_cast<SteamItemDef_t>(item.instanceId));
			generateQuantities.Add(static_cast<uint32>(item.quantity));
		}
		else if (item.instanceId < 0)
		{
			destroyItems.Add(static_cast<SteamItemInstanceID_t>(-item.instanceId));
			destroyQuantities.Add(static_cast<uint32>(item.quantity));
		}
	}

	SteamInventoryResult_t handle;

	bool response = SteamInventory()->ExchangeItems(&handle,
		generateItems.GetData(), generateQuantities.GetData(), generateItems.Num(),
		destroyItems.GetData(), destroyQuantities.GetData(), destroyItems.Num());
	if (response)
	{
		SteamGameInstance->InventoryRequests.Add(handle, request);
	}
	return response;
}

bool UAPIInventoryClient::SteamSimpleGenerateItems(TArray<FItemCount> items, TArray<FString> readProperties, FItemResultsCallback callback)
{
	STEAM_CLIENT_INSTANCE(false);
	FItemResultRequest request = FItemResultRequest();
	request.properties = readProperties;
	request.callback = callback;

	TArray<SteamItemDef_t> itemDefs;
	TArray<uint32> quantities;

	for (const FItemCount& item : items)
	{
		itemDefs.Add(static_cast<SteamItemDef_t>(item.instanceId));
		quantities.Add(static_cast<uint32>(item.quantity));
	}

	SteamInventoryResult_t handle;
	bool response = SteamInventory()->GenerateItems(&handle, itemDefs.GetData(), quantities.GetData(), itemDefs.Num());
	if (response)
	{
		SteamGameInstance->InventoryRequests.Add(handle, request);
	}
	return response;
}

bool UAPIInventoryClient::SteamSimpleGetAllItems(TArray<FString> readProperties, FItemResultsCallback callback)
{
	STEAM_CLIENT_INSTANCE(false);
	FItemResultRequest request = FItemResultRequest();
	request.properties = readProperties;
	request.callback = callback;
	SteamInventoryResult_t handle;
	bool response = SteamInventory()->GetAllItems(&handle);
	if (response)
	{
		SteamGameInstance->InventoryRequests.Add(handle, request);
	}
	return response;
}

bool UAPIInventoryClient::SteamSimpleGrantPromoItems(TArray<FString> readProperties, FItemResultsCallback callback)
{
	STEAM_CLIENT_INSTANCE(false);
	FItemResultRequest request = FItemResultRequest();
	request.properties = readProperties;
	request.callback = callback;
	SteamInventoryResult_t handle;
	bool response = SteamInventory()->GrantPromoItems(&handle);
	if (response)
	{
		SteamGameInstance->InventoryRequests.Add(handle, request);
	}
	return response;
}

bool UAPIInventoryClient::SteamSimpleTransferItemQuantity(int64 sourceInstanceId, int32 quantity, int64 destinationInstanceId, TArray<FString> readProperties, FItemResultsCallback callback)
{
	STEAM_CLIENT_INSTANCE(false);
	FItemResultRequest request = FItemResultRequest();
	request.properties = readProperties;
	request.callback = callback;
	SteamInventoryResult_t result;
	SteamItemInstanceID_t sInstance = static_cast<SteamItemInstanceID_t>(sourceInstanceId);
	SteamItemInstanceID_t dInstance = static_cast<SteamItemInstanceID_t>(destinationInstanceId);
	bool response = SteamInventory()->TransferItemQuantity(&result, sInstance, static_cast<uint32>(quantity), dInstance);
	if (response)
	{
		SteamGameInstance->InventoryRequests.Add(result, request);
	}
	return response;
}

bool UAPIInventoryClient::SteamSimpleTriggerItemDrop(int32 itemDef, TArray<FString> readProperties, FItemResultsCallback callback)
{
	STEAM_CLIENT_INSTANCE(false);
	FItemResultRequest request = FItemResultRequest();
	request.properties = readProperties;
	request.callback = callback;
	SteamInventoryResult_t handle;
	bool response = SteamInventory()->TriggerItemDrop(&handle, SteamItemDef_t(itemDef));
	if (response)
	{
		SteamGameInstance->InventoryRequests.Add(handle, request);
	}
	return response;
}

FInventoryResult UAPIInventoryClient::SteamAddPromoItem(int32 itemDef)
{
	FInventoryResult result = FInventoryResult();
	STEAM_CLIENT(result);

	TArray<SteamItemDef_t> steamItemDefs;
	steamItemDefs.Add(static_cast<SteamItemDef_t>(itemDef));

	SteamInventoryResult_t handle;
	result.success = SteamInventory()->AddPromoItem(&handle, SteamItemDef_t(itemDef));
	result.resultHandle = handle;

	return result;
}

FInventoryResult UAPIInventoryClient::SteamAddPromoItems(TArray<int32> itemDefs)
{
	FInventoryResult result = FInventoryResult();
	STEAM_CLIENT(result);

	TArray<SteamItemDef_t> steamItemDefs;
	for (const int32& itemDef : itemDefs)
	{
		steamItemDefs.Add(static_cast<SteamItemDef_t>(itemDef));
	}

	SteamInventoryResult_t handle;
	result.success = SteamInventory()->AddPromoItems(&handle, steamItemDefs.GetData(), steamItemDefs.Num());
	result.resultHandle = handle;

	return result;
}

bool UAPIInventoryClient::SteamCheckResultSteamID(int32 resultHandle, int64 steamIDExpected)
{
	STEAM_CLIENT(false);

	CSteamID cId = INT64_TO_STEAMID(steamIDExpected);
	return SteamInventory()->CheckResultSteamID(resultHandle, cId);
}

FInventoryResult UAPIInventoryClient::SteamConsumeItem(int64 itemInstanceId, int32 quantity)
{
	FInventoryResult result = FInventoryResult();
	STEAM_CLIENT(result);

	SteamItemInstanceID_t steamItemInstanceId = SteamItemInstanceID_t(itemInstanceId);
	SteamInventoryResult_t handle;
	result.success = SteamInventory()->ConsumeItem(&handle, steamItemInstanceId, static_cast<uint32>(quantity));
	result.resultHandle = handle;

	return result;
}

FInventoryResult UAPIInventoryClient::SteamDeserializeResult(TArray<uint8> buffer)
{
	FInventoryResult result = FInventoryResult();
	STEAM_CLIENT(result);
	
	if (buffer.Num() == 0)
		return result;

	SteamInventoryResult_t handle;
	result.success = SteamInventory()->DeserializeResult(&handle, buffer.GetData(), buffer.Num());
	result.resultHandle = handle;
	
	return result;
}

void UAPIInventoryClient::SteamDestroyResult(int32 resultHandle)
{
	SteamInventory()->DestroyResult(SteamInventoryResult_t(resultHandle));
}

FInventoryResult UAPIInventoryClient::SteamExchangeItems(int32 itemDef, TArray<FItemCount> recipe)
{
	FInventoryResult result = FInventoryResult();
	STEAM_CLIENT(result);

	TArray<SteamItemDef_t> generateItems;
	TArray<uint32> generateQuantities;
	TArray<SteamItemInstanceID_t> destroyItems;
	TArray<uint32> destroyQuantities;

	for (const FItemCount& item : recipe)
	{
		if (item.instanceId > 0)
		{
			generateItems.Add(static_cast<SteamItemDef_t>(item.instanceId));
			generateQuantities.Add(static_cast<uint32>(item.quantity));
		}
		else if (item.instanceId < 0)
		{
			destroyItems.Add(static_cast<SteamItemInstanceID_t>(-item.instanceId));
			destroyQuantities.Add(static_cast<uint32>(item.quantity));
		}
	}

	SteamInventoryResult_t handle;
	
	result.success = SteamInventory()->ExchangeItems(&handle,
		generateItems.GetData(), generateQuantities.GetData(), generateItems.Num(),
		destroyItems.GetData(), destroyQuantities.GetData(), destroyItems.Num());
	result.resultHandle = handle;
	
	return result;
}

FInventoryResult UAPIInventoryClient::SteamGenerateItems(TArray<FItemCount> items)
{
	FInventoryResult result = FInventoryResult();
	STEAM_CLIENT(result);

	TArray<SteamItemDef_t> itemDefs;
	TArray<uint32> quantities;

	for (const FItemCount& item : items)
	{
		itemDefs.Add(static_cast<SteamItemDef_t>(item.instanceId));
		quantities.Add(static_cast<uint32>(item.quantity));
	}

	SteamInventoryResult_t handle;
	result.success = SteamInventory()->GenerateItems(&handle, itemDefs.GetData(), quantities.GetData(), itemDefs.Num());
	result.resultHandle = handle;

	return result;
}

FInventoryResult UAPIInventoryClient::SteamGetAllItems()
{
	FInventoryResult result = FInventoryResult();
	STEAM_CLIENT(result);

	SteamInventoryResult_t handle;
	result.success = SteamInventory()->GetAllItems(&handle);
	result.resultHandle = handle;

	return result;
}

TArray<FString> UAPIInventoryClient::SteamGetItemDefinitionProperties(int32 itemDef)
{
	STEAM_CLIENT(TArray<FString>());

	uint32 bufferSize = 0;
	bool bGetSizeSuccess = SteamInventory()->GetItemDefinitionProperty(SteamItemDef_t(itemDef), NULL, NULL, &bufferSize);

	if (!bGetSizeSuccess || bufferSize == 0)
		return TArray<FString>();

	char* pchValueBuffer = new char[bufferSize];
	bool bGetPropertySuccess = SteamInventory()->GetItemDefinitionProperty(SteamItemDef_t(itemDef), NULL, pchValueBuffer, &bufferSize);

	FString ItemProperty;
	TArray<FString> ResultArray;
	if (bGetPropertySuccess)
	{
		ItemProperty = FString(pchValueBuffer);
		ItemProperty.ParseIntoArray(ResultArray, TEXT(","), true);
	}

	delete[] pchValueBuffer;

	return ResultArray;
}

FString UAPIInventoryClient::SteamGetItemDefinitionProperty(int32 itemDef, FString property)
{
	STEAM_CLIENT(FString());

	uint32 bufferSize = 0;
	bool bGetSizeSuccess = SteamInventory()->GetItemDefinitionProperty(SteamItemDef_t(itemDef), StringCast<ANSICHAR>(*property).Get(), nullptr, &bufferSize);

	if (!bGetSizeSuccess || bufferSize == 0)
	{
		// Handle error or empty property
		return FString();
	}

	char* pchValueBuffer = new char[bufferSize];

	// Call Steamworks SDK to get the item property
	bool bGetPropertySuccess = SteamInventory()->GetItemDefinitionProperty(SteamItemDef_t(itemDef), StringCast<ANSICHAR>(*property).Get(), pchValueBuffer, &bufferSize);

	FString ItemProperty;
	if (bGetPropertySuccess)
	{
		ItemProperty = FString(pchValueBuffer);
	}

	// Free the allocated memory
	delete[] pchValueBuffer;

	return ItemProperty;
}

FInventoryResult UAPIInventoryClient::SteamGetItemsByID(TArray<int64> instanceIds)
{
	FInventoryResult result = FInventoryResult();
	STEAM_CLIENT(result);

	TArray<SteamItemInstanceID_t> steamInstanceIds;
	for (const int64& instanceId : instanceIds)
	{
		SteamItemInstanceID_t steamInstanceId = static_cast<SteamItemInstanceID_t>(instanceId);
		steamInstanceIds.Add(steamInstanceId);
	}

	SteamInventoryResult_t handle;
	result.success = SteamInventory()->GetItemsByID(&handle, steamInstanceIds.GetData(), steamInstanceIds.Num());
	result.resultHandle = static_cast<int32>(handle);
	return result;
}

FItemPrice UAPIInventoryClient::SteamGetItemPrice(int32 itemDef)
{
	STEAM_CLIENT(FItemPrice());
	FItemPrice price = FItemPrice();
	uint64 nCur;
	uint64 nBase;
	SteamInventory()->GetItemPrice(SteamItemDef_t(itemDef), &nCur, &nBase);
	price.basePrice = static_cast<int64>(nBase);
	price.currentPrice = static_cast<int64>(nCur);
	return price;
}

TArray<FItemWithPrice> UAPIInventoryClient::SteamGetItemsWithPrices()
{
	STEAM_CLIENT(TArray<FItemWithPrice>());
	uint32 count = SteamInventory()->GetNumItemsWithPrices();
	SteamItemDef_t* itemDefs = new SteamItemDef_t[count];
	uint64* currentPrices = new uint64[count];
	uint64* basePrices = new uint64[count];

	if (SteamInventory()->GetItemsWithPrices(itemDefs, currentPrices, basePrices, count))
	{
		TArray<FItemWithPrice> result;
		for (uint32 i = 0; i < count; i++)
		{
			FItemWithPrice nItem = FItemWithPrice();
			nItem.itemDef = itemDefs[i];
			nItem.currentPrice = static_cast<int64>(currentPrices[i]);
			nItem.basePrice = static_cast<int64>(basePrices[i]);
			result.Add(nItem);
		}

		delete[] itemDefs;
		delete[] currentPrices;
		delete[] basePrices;

		return result;
	}
	else
	{
		delete[] itemDefs;
		delete[] currentPrices;
		delete[] basePrices;

		return TArray<FItemWithPrice>();
	}
}

int32 UAPIInventoryClient::SteamGetNumItemsWithPrices()
{
	STEAM_CLIENT(0);
	return static_cast<int32>(SteamInventory()->GetNumItemsWithPrices());
}

FString UAPIInventoryClient::SteamGetResultItemProperty(int32 resultHandle, int32 index, FString propertyName)
{
	STEAM_CLIENT(FString());
	
	uint32 bufferSize = 0;
	uint32 unItemIndex = static_cast<uint32>(index);
	bool bGetSizeSuccess = SteamInventory()->GetResultItemProperty(resultHandle, unItemIndex, StringCast<ANSICHAR>(*propertyName).Get(), nullptr, &bufferSize);

	if (!bGetSizeSuccess || bufferSize == 0)
	{
		// Handle error or empty property
		return FString();
	}

	char* pchValueBuffer = new char[bufferSize];

	// Call Steamworks SDK to get the item property
	bool bGetPropertySuccess = SteamInventory()->GetResultItemProperty(resultHandle, unItemIndex, StringCast<ANSICHAR>(*propertyName).Get(), pchValueBuffer, &bufferSize);

	FString ItemProperty;
	if (bGetPropertySuccess)
	{
		ItemProperty = FString(pchValueBuffer);
	}

	// Free the allocated memory
	delete[] pchValueBuffer;

	return ItemProperty;
}

TArray<FItemDetail> UAPIInventoryClient::SteamGetResultItems(int32 resultHandle)
{
	TArray<FItemDetail> ItemDetailsArray;
	STEAM_CLIENT(ItemDetailsArray);

	SteamItemDetails_t* pOutItemsArray = nullptr;
	uint32 punOutItemsArraySize = 0;
	bool bSuccess = SteamInventory()->GetResultItems(resultHandle, pOutItemsArray, &punOutItemsArraySize);

	if (!bSuccess || punOutItemsArraySize == 0)
	{
		// Handle error or empty array
		return TArray<FItemDetail>();
	}

	pOutItemsArray = new SteamItemDetails_t[punOutItemsArraySize];
	bSuccess = SteamInventory()->GetResultItems(resultHandle, pOutItemsArray, &punOutItemsArraySize);

	if (bSuccess && punOutItemsArraySize > 0)
	{
		for (uint32 i = 0; i < punOutItemsArraySize; ++i)
		{
			FItemDetail ItemDetail;
			ItemDetail.instanceId = static_cast<int64>(pOutItemsArray[i].m_itemId);
			ItemDetail.definitionId = static_cast<int32>(pOutItemsArray[i].m_iDefinition);
			ItemDetail.quantity = static_cast<int32>(pOutItemsArray[i].m_unQuantity);
			ItemDetail.flags = static_cast<int32>(pOutItemsArray[i].m_unFlags);

			ItemDetailsArray.Add(ItemDetail);
		}
	}

	delete[] pOutItemsArray;

	return ItemDetailsArray;
}

UEResult UAPIInventoryClient::SteamGetResultStatus(int32 resultHandle)
{
	STEAM_CLIENT(UEResult::EPC_IOFailure);

	EResult result = SteamInventory()->GetResultStatus(SteamInventoryResult_t(resultHandle));
	return static_cast<UEResult>(result);
}

FDateTime UAPIInventoryClient::SteamGetResultTimestamp(int32 resultHandle)
{
	STEAM_CLIENT(FDateTime());
	return FDateTime().FromUnixTimestamp(static_cast<int64>(SteamInventory()->GetResultTimestamp(SteamInventoryResult_t(resultHandle))));
}

FInventoryResult UAPIInventoryClient::SteamGrantPromoItems()
{
	STEAM_CLIENT(FInventoryResult());
	FInventoryResult result = FInventoryResult();
	SteamInventoryResult_t handle;
	bool ret = SteamInventory()->GrantPromoItems(&handle);
	result.resultHandle = static_cast<int32>(handle);
	result.success = ret;
	return result;
}

bool UAPIInventoryClient::SteamLoadItemDefinitions()
{
	STEAM_CLIENT(false);
	return SteamInventory()->LoadItemDefinitions();
}

void UAPIInventoryClient::SteamRequestEligiblePromoItemDefinitionsIDs(FSteamInventoryEligiblePromoItemDefIDsCallback callback)
{
	STEAM_CLIENT(;);
	SteamInventoryEligiblePromoItemDefIDsLinker* linker = new SteamInventoryEligiblePromoItemDefIDsLinker(callback);
}

void UAPIInventoryClient::SteamRequestPrices(FRequestPricesCallback callback)
{
	STEAM_CLIENT(;);
	SteamInventoryRequestPricesResultLinker* linker = new SteamInventoryRequestPricesResultLinker(callback);
}

TArray<uint8> UAPIInventoryClient::SteamSerializeResult(int32 resultHandle)
{
	STEAM_CLIENT(TArray<uint8>());

	SteamInventoryResult_t rHandle = SteamInventoryResult_t(resultHandle);
	uint32 size;
	if (SteamInventory()->SerializeResult(rHandle, NULL, &size))
	{
		TArray<uint8> buffer;
		buffer.AddUninitialized(static_cast<int32>(size));
		void* outBuffer = buffer.GetData();

		if (SteamInventory()->SerializeResult(rHandle, outBuffer, &size))
			return buffer;
		else
			return TArray<uint8>();
	}
	else
		return TArray<uint8>();
}

void UAPIInventoryClient::SteamStartPurchase(TArray<FItemDefCount> items, FSteamInventoryStartPurchaseResultCallback callback)
{
	STEAM_CLIENT(;);
	SteamInventoryStartPurchaseResultLinker* linker = new SteamInventoryStartPurchaseResultLinker(items, callback);
}

FInventoryResult UAPIInventoryClient::SteamTransferItemQuantity(int64 sourceInstanceId, int32 quantity, int64 destinationInstanceId)
{
	STEAM_CLIENT(FInventoryResult());
	SteamInventoryResult_t result;
	SteamItemInstanceID_t sInstance = static_cast<SteamItemInstanceID_t>(sourceInstanceId);
	SteamItemInstanceID_t dInstance = static_cast<SteamItemInstanceID_t>(destinationInstanceId);
	bool ret = SteamInventory()->TransferItemQuantity(&result, sInstance, static_cast<uint32>(quantity), dInstance);

	FInventoryResult value = FInventoryResult();
	value.resultHandle = static_cast<int32>(result);
	value.success = ret;
	return value;
}

FInventoryResult UAPIInventoryClient::SteamTriggerItemDrop(int32 itemDef)
{
	STEAM_CLIENT(FInventoryResult());
	SteamInventoryResult_t result;
	bool ret = SteamInventory()->TriggerItemDrop(&result, SteamItemDef_t(itemDef));

	FInventoryResult value = FInventoryResult();
	value.resultHandle = static_cast<int32>(result);
	value.success = ret;
	return value;
}

int64 UAPIInventoryClient::SteamStartUpdateProperties()
{
	SteamInventoryUpdateHandle_t updateHandle = SteamInventory()->StartUpdateProperties();
	return static_cast<int64>(updateHandle);
}

FInventoryResult UAPIInventoryClient::SteamSubmitUpdateProperties(int64 updateHandle)
{
	STEAM_CLIENT(FInventoryResult());
	SteamInventoryUpdateHandle_t handle = static_cast<SteamInventoryUpdateHandle_t>(updateHandle);
	SteamInventoryResult_t result;
	bool ret = SteamInventory()->SubmitUpdateProperties(handle, &result);

	FInventoryResult value = FInventoryResult();
	value.resultHandle = static_cast<int32>(result);
	value.success = ret;
	return value;
}

bool UAPIInventoryClient::SteamRemoveProperty(int64 updateHandle, int64 instanceId, FString propertyName)
{
	STEAM_CLIENT(false);
	SteamInventoryUpdateHandle_t handle = static_cast<SteamInventoryUpdateHandle_t>(updateHandle);
	SteamItemInstanceID_t instance = static_cast<SteamItemInstanceID_t>(instanceId);
	return SteamInventory()->RemoveProperty(handle, instance, StringCast<ANSICHAR>(*propertyName).Get());
}

bool UAPIInventoryClient::SteamSetPropertyString(int64 updateHandle, int64 instanceId, FString propertyName, FString value)
{
	STEAM_CLIENT(false);
	SteamInventoryUpdateHandle_t handle = static_cast<SteamInventoryUpdateHandle_t>(updateHandle);
	SteamItemInstanceID_t instance = static_cast<SteamItemInstanceID_t>(instanceId);
	return SteamInventory()->SetProperty(handle, instance, StringCast<ANSICHAR>(*propertyName).Get(), StringCast<ANSICHAR>(*value).Get());
}

bool UAPIInventoryClient::SteamSetPropertyBool(int64 updateHandle, int64 instanceId, FString propertyName, bool value)
{
	STEAM_CLIENT(false);
	SteamInventoryUpdateHandle_t handle = static_cast<SteamInventoryUpdateHandle_t>(updateHandle);
	SteamItemInstanceID_t instance = static_cast<SteamItemInstanceID_t>(instanceId);
	return SteamInventory()->SetProperty(handle, instance, StringCast<ANSICHAR>(*propertyName).Get(), value);
}

bool UAPIInventoryClient::SteamSetPropertyInt(int64 updateHandle, int64 instanceId, FString propertyName, int64 value)
{
	STEAM_CLIENT(false);
	SteamInventoryUpdateHandle_t handle = static_cast<SteamInventoryUpdateHandle_t>(updateHandle);
	SteamItemInstanceID_t instance = static_cast<SteamItemInstanceID_t>(instanceId);
	return SteamInventory()->SetProperty(handle, instance, StringCast<ANSICHAR>(*propertyName).Get(), value);
}

bool UAPIInventoryClient::SteamSetPropertyFloat(int64 updateHandle, int64 instanceId, FString propertyName, float value)
{
	STEAM_CLIENT(false);
	SteamInventoryUpdateHandle_t handle = static_cast<SteamInventoryUpdateHandle_t>(updateHandle);
	SteamItemInstanceID_t instance = static_cast<SteamItemInstanceID_t>(instanceId);
	return SteamInventory()->SetProperty(handle, instance, StringCast<ANSICHAR>(*propertyName).Get(), value);
}