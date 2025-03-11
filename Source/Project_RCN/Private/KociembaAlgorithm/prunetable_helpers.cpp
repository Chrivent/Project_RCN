#include "KociembaAlgorithm/prunetable_helpers.h"

FString FPruneTableHelpers::JoinPath(const FString& Dir, const FString& Filename)
{
	return FPaths::Combine(Dir, Filename);
}

bool FPruneTableHelpers::CheckCachedTable(const FString& Name, void* Ptr, int32 Len, const FString& CacheDir)
{
	FString FilePath = JoinPath(CacheDir, Name);

	if (FPaths::FileExists(FilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("Found cache for %s. Loading..."), *Name);
		ReadFromFile(Ptr, Len, FilePath);
		UE_LOG(LogTemp, Log, TEXT("Done."));
		return true;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Cache table %s was not found. Recalculating."), *FilePath);
	return false;
}

void FPruneTableHelpers::ReadFromFile(void* Ptr, int32 Len, const FString& FilePath)
{
	TArray<uint8> FileData;
	if (FFileHelper::LoadFileToArray(FileData, *FilePath))
	{
		FMemory::Memcpy(Ptr, FileData.GetData(), FMath::Min(FileData.Num(), Len));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to read file: %s"), *FilePath);
	}
}

bool FPruneTableHelpers::MakeDir(const FString& CacheDir)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	return PlatformFile.CreateDirectoryTree(*CacheDir);
}

void FPruneTableHelpers::DumpToFile(void* Ptr, int32 Len, const FString& Name, const FString& CacheDir)
{
	if (MakeDir(CacheDir))
	{
		FString FilePath = JoinPath(CacheDir, Name);
		TArray<uint8> FileData;
		FileData.Append(static_cast<uint8*>(Ptr), Len);

		if (FFileHelper::SaveArrayToFile(FileData, *FilePath))
		{
			UE_LOG(LogTemp, Log, TEXT("Successfully wrote cache table: %s"), *FilePath);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to write cache table: %s"), *FilePath);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot create cache tables directory: %s"), *CacheDir);
	}
}
