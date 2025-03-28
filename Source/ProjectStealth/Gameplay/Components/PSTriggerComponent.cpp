// Copyright (c) 2025 Filip Dunal. All Rights Reserved.


#include "ProjectStealth/Gameplay/Components/PSTriggerComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "ProjectStealth/Gameplay/ActionActors/PSActionActor.h"
#include "ProjectStealth/Gameplay/Characters/PSCharacterBase.h"

#define ECC_TriggerSource ECC_GameTraceChannel1
#define ECC_TriggerZone ECC_GameTraceChannel2

UPSTriggerComponent::UPSTriggerComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	TriggerType = EPSTriggerType::Cone;
	TriggerConeSettings = FPSTriggerConeSettings();
	bCanEverAffectNavigation = false;

	SetCollisionObjectType(ECC_TriggerZone);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_TriggerSource, ECR_Overlap);
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

	GenerateConeMesh(false);
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

	return FilterClass.ContainsByPredicate([ClassToCheck](const TSubclassOf<APSCharacterBase>& CharClass)
	{
		return ClassToCheck->IsChildOf(CharClass);
	});
}


APSActionActor* UPSTriggerComponent::GetActionActorChecked() const
{
	APSActionActor* ActionActor = Cast<APSActionActor>(GetOwner());
	return IsValid(ActionActor) ? ActionActor : nullptr;
}


void UPSTriggerComponent::GenerateCubeMesh()
{
	TArray<FVector> Vertices = {
		FVector(-50.f, -50.f, -50.f), // 0
		FVector(50.f, -50.f, -50.f),  // 1
		FVector(50.f, 50.f, -50.f),   // 2
		FVector(-50.f, 50.f, -50.f),  // 3
		FVector(-50.f, -50.f, 50.f),  // 4
		FVector(50.f, -50.f, 50.f),   // 5
		FVector(50.f, 50.f, 50.f),    // 6
		FVector(-50.f, 50.f, 50.f)    // 7
	};

	TArray<int32> Triangles = {
		0, 2, 1, 0, 3, 2, // Front face
		4, 5, 6, 4, 6, 7, // Back face
		0, 1, 5, 0, 5, 4, // Bottom face
		2, 3, 7, 2, 7, 6, // Top face
		0, 4, 7, 0, 7, 3, // Left face
		1, 2, 6, 1, 6, 5  // Right face
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
