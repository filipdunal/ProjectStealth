// Copyright (c) 2025 Filip Dunal. All Rights Reserved.


#include "ProjectStealth/Gameplay/Components/PSTriggerComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "ProjectStealth/Gameplay/ActionActors/PSActionActor.h"
#include "ProjectStealth/Gameplay/Characters/PSCharacterBase.h"
#include "ProjectStealth/Settings/PSGuardSettings.h"
#include "ProjectStealth/Settings/PSTriggerSettings.h"

#define ECC_TriggerZone ECC_GameTraceChannel2


UPSTriggerComponent::UPSTriggerComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	TriggerType = EPSTriggerType::Cone;
	bCanEverAffectNavigation = false;

	SetCollisionObjectType(ECC_TriggerZone);
}


void UPSTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	if(TriggerType == EPSTriggerType::Cone)
	{
		GetWorld()->GetTimerManager().SetTimer(UpdateConeTriggerHandle, this, &UPSTriggerComponent::UpdateConeTrigger, 0.01f, true, FMath::RandRange(0.0f, 0.1f));
	}

	OnTriggerBegin.AddUniqueDynamic(this, &UPSTriggerComponent::RegisterTriggerSource);
	OnTriggerEnd.AddUniqueDynamic(this, &UPSTriggerComponent::UnregisterTriggerSource);
}


void UPSTriggerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(UpdateConeTriggerHandle);
	}

	Super::EndPlay(EndPlayReason);
}


void UPSTriggerComponent::OnRegister()
{
	Super::OnRegister();

	switch(TriggerType)
	{
	case EPSTriggerType::Cone:
		GenerateConeMesh(false);
		break;

	case EPSTriggerType::Box:
		GenerateBoxMesh();
		break;
	}

	UMaterialInterface* MaterialToUse = OverrideTriggerMaterial;
	if (!bUseOverrideTriggerMaterial)
	{
		if (const UPSTriggerSettings* TriggerSettings = UPSTriggerSettings::StaticClass()->GetDefaultObject<UPSTriggerSettings>())
		{
			MaterialToUse = TriggerSettings->DefaultMaterial.LoadSynchronous();
		}
	}
	UProceduralMeshComponent::SetMaterial(0, MaterialToUse);
}


void UPSTriggerComponent::PostInitProperties()
{
	Super::PostInitProperties();
}


bool UPSTriggerComponent::CanTrigger(const AActor* OtherActor) const
{
	if(!IsValid(OtherActor))
	{
		return false;
	}

	if(!OtherActor->Implements<UPSTriggerSource>())
	{
		return false;
	}

	return CheckClassFilter(OtherActor->GetClass());
}


bool UPSTriggerComponent::CheckClassFilter(UClass* ClassToCheck) const
{
	if (FilterClass.IsEmpty())
	{
		return true;
	}

	return FilterClass.ContainsByPredicate([ClassToCheck](const TSubclassOf<UObject>& CharClass)
	{
		return ClassToCheck->IsChildOf(CharClass);
	});
}


void UPSTriggerComponent::CallTriggerBegin(TScriptInterface<IPSTriggerSource> TriggerSource)
{
	OnTriggerBegin.Broadcast(TriggerSource, this);
}


void UPSTriggerComponent::CallTriggerEnd(TScriptInterface<IPSTriggerSource> TriggerSource)
{
	OnTriggerEnd.Broadcast(TriggerSource, this);
}


APSActionActor* UPSTriggerComponent::GetActionActorChecked() const
{
	APSActionActor* ActionActor = Cast<APSActionActor>(GetOwner());
	return IsValid(ActionActor) ? ActionActor : nullptr;
}


void UPSTriggerComponent::GenerateBoxMesh()
{
	const FVector HalfSize = TriggerBoxSettings.HalfSize;

	TArray<FVector> Vertices = {
		FVector(-HalfSize.X, -HalfSize.Y, -HalfSize.Z), // 0
		FVector(HalfSize.X, -HalfSize.Y, -HalfSize.Z),  // 1
		FVector(HalfSize.X, HalfSize.Y, -HalfSize.Z),   // 2
		FVector(-HalfSize.X, HalfSize.Y, -HalfSize.Z),  // 3
		FVector(-HalfSize.X, -HalfSize.Y, HalfSize.Z),  // 4
		FVector(HalfSize.X, -HalfSize.Y, HalfSize.Z),   // 5
		FVector(HalfSize.X, HalfSize.Y, HalfSize.Z),    // 6
		FVector(-HalfSize.X, HalfSize.Y, HalfSize.Z)    // 7
	};

	TArray<int32> Triangles = {
		0, 1, 2,  0, 2, 3,  // Front face
		4, 6, 5,  4, 7, 6,  // Back face
		0, 5, 1,  0, 4, 5,  // Bottom face
		2, 6, 7,  2, 7, 3,  // Top face
		0, 3, 7,  0, 7, 4,  // Left face
		1, 5, 6,  1, 6, 2   // Right face
	};

	TArray<FVector> Normals = {
		FVector(0.f, 0.f, -1.f), // Front face
		FVector(0.f, 0.f, 1.f),  // Back face
		FVector(0.f, -1.f, 0.f), // Bottom face
		FVector(0.f, 1.f, 0.f),  // Top face
		FVector(-1.f, 0.f, 0.f), // Left face
		FVector(1.f, 0.f, 0.f)   // Right face
	};

	TArray<FVector2D> UVs = {
		FVector2D(0.f, 0.f), FVector2D(1.f, 0.f), FVector2D(1.f, 1.f), FVector2D(0.f, 1.f)
	};

	TArray<FProcMeshTangent> Tangents;
	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		Tangents.Add(FProcMeshTangent(1.f, 0.f, 0.f));
	}

	CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), Tangents, true);
}


void UPSTriggerComponent::GenerateConeMesh(bool bUpdateOnly)
{
	const float ViewAngle = TriggerConeSettings.Angle;
	const int32 CircleSegments = 720;
	const float OuterRadius = TriggerConeSettings.Radius;

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;

	Vertices.Add(FVector(0, 0, 0)); // Center vertex
	Normals.Add(FVector(0, 0, 1));
	UVs.Add(FVector2D(0.5f, 0.5f));
	VertexColors.Add(FColor::White);

	const float StartingAngle = -ViewAngle / 2.0f;
	const float EndingAngle = StartingAngle + ViewAngle;

	int32 ConeSegments = 0;
	bool bLastLineHit = false;
	for (int32 i = 0; i <= CircleSegments; i++)
	{
		float Angle = StartingAngle + (360.0f * i) / CircleSegments;

		//UE_LOG(LogTemp, Display, TEXT("ProcMesh: Current angle: %f"), Angle);
		bool bIsLastSegment = false;

		if(Angle > EndingAngle)
		{
			Angle = FMath::Min(Angle, EndingAngle);
			bIsLastSegment = true;
		}

		float Rad = Angle * PI / 180.0f;
		float X = OuterRadius * FMath::Cos(Rad);
		float Y = OuterRadius * FMath::Sin(Rad);

		FVector TargetPoint = FVector(X, Y, 0);

		FHitResult HitResult;
		FVector StartTrace = GetComponentLocation();
		FVector EndTrace = UKismetMathLibrary::TransformLocation(GetComponentTransform(), TargetPoint);
		GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldDynamic);
		if(HitResult.bBlockingHit)
		{
			TargetPoint *= (HitResult.Distance + 10.0f) / OuterRadius;
		}

		Vertices.Add(TargetPoint);

		Normals.Add(FVector(0, 0, 1));
		UVs.Add(FVector2D((X / OuterRadius + 1) * 0.5f, (Y / OuterRadius + 1) * 0.5f));
		VertexColors.Add(FColor::White);

		ConeSegments++;

		if(bIsLastSegment)
		{
			break;
		}
	}

	for (int32 i = 1; i < ConeSegments - 1; i++)
	{
		Triangles.Add(0);
		Triangles.Add(i+ 1);
		Triangles.Add(i);
	}

	if(bUpdateOnly)
	{
		UpdateMeshSection_LinearColor(0, Vertices, Normals, UVs, UVs, UVs, UVs, VertexColors, Tangents);
	}
	else
	{
		CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
	}
}


void UPSTriggerComponent::UpdateConeTrigger()
{
	GenerateConeMesh(true);
}


void UPSTriggerComponent::RegisterTriggerSource(TScriptInterface<IPSTriggerSource> TriggerSource, UPSTriggerComponent* TriggerComponent)
{
	TriggerSources.Add(TriggerSource);
}


void UPSTriggerComponent::UnregisterTriggerSource(TScriptInterface<IPSTriggerSource> TriggerSource, UPSTriggerComponent* TriggerComponent)
{
	TriggerSources.Remove(TriggerSource);
}
