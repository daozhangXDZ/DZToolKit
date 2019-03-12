#include "ObjectFunLib.h"

FString FTrimmedKeyMap::ToString()
{
	return FString();
}


UObjectFunLib::UObjectFunLib(const  FObjectInitializer & objinit)
	:Super(objinit)
{

}

FTrimmedKeyMapNameType UObjectFunLib::NativeGetStructPropertyNameList(UScriptStruct * pStruct, void * StructPtr, bool IsBlueprintStruct)
{
	TSharedPtr<FTrimmedKeyMapNameType> vProptyShare = MakeShareable<FTrimmedKeyMapNameType>(new FTrimmedKeyMapNameType());
	SetTrimmedKeyMapForStruct(vProptyShare, pStruct);
	return *vProptyShare;
}

void UObjectFunLib::SetTrimmedKeyMapForStruct(TSharedPtr<FTrimmedKeyMapNameType>& InMap, UStruct * pStruct)
{
	UField* FieldPtr = pStruct->Children;
	InMap->mTypeName = pStruct->GetName();
	while (FieldPtr != nullptr)
	{
		if (FieldPtr->GetClass() != nullptr)
		{
			InMap->mPropertyNameMapType.Add(FieldPtr->GetName(), FieldPtr->GetClass()->GetName());
		}
		else
		{
			InMap->mPropertyNameMapType.Add(FieldPtr->GetName(),"");
		}
		/*UStructProperty* SubStruct = Cast<UStructProperty>(FieldPtr);
		if (SubStruct != nullptr)
		{
			return;
		}
		UArrayProperty* ArrayProp = Cast<UArrayProperty>(FieldPtr);
		if (ArrayProp != nullptr)
		{
			return;
		}
		UMapProperty* MapProperty = Cast<UMapProperty>(FieldPtr);
		if (MapProperty != nullptr)
		{
			return;
		}*/

		FieldPtr = FieldPtr->Next;
	}
}


