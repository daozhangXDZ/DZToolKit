#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ObjectFunLib.generated.h"

USTRUCT(BlueprintType)
struct FTrimmedKeyMap
{
	GENERATED_BODY()
	FString LongKey;
	//ตÝน้

	TMap<FString, TSharedPtr<FTrimmedKeyMap>> SubMap;
	FString ToString();
};

USTRUCT(BlueprintType)
struct FTrimmedKeyMapNameType
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FString mTypeName;
	UPROPERTY(BlueprintReadWrite)
	TMap<FString,FString> mPropertyNameMapType;
};

UCLASS(BlueprintType)
class UObjectFunLib : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:

	UFUNCTION(BlueprintCallable, CustomThunk,meta=(DisplayName="GetStructPropertyNameList",CustomStructureParam="AnyStruct"),Category = "DZToolkit|ObjectFunLib|Property")
	static FTrimmedKeyMapNameType GetStructPropertyNameList(UProperty* AnyStruct);
	DECLARE_FUNCTION(execGetStructPropertyNameList)
	{
		//Get properties and pointers from stack (nb, it's reverse order, right to left!)
		Stack.StepCompiledIn<UStructProperty>(NULL);
		UStructProperty* StructProp = ExactCast<UStructProperty>(Stack.MostRecentProperty);
		void* StructPtr = Stack.MostRecentPropertyAddress;

		P_FINISH;

		P_NATIVE_BEGIN;
		*(FTrimmedKeyMapNameType*)RESULT_PARAM = UObjectFunLib::NativeGetStructPropertyNameList(StructProp->Struct, StructPtr,true);
		P_NATIVE_END;
	}

	static FTrimmedKeyMapNameType NativeGetStructPropertyNameList(UScriptStruct* pStruct, void* StructPtr,bool IsBlueprintStruct);
	static void SetTrimmedKeyMapForStruct(TSharedPtr<FTrimmedKeyMapNameType>& InMap, UStruct* pStruct);
};