
#include "KNOCKturneObjectPool.h"
#include "Actor/PooledObject.h"

UKNOCKturneObjectPool::UKNOCKturneObjectPool()
{

}

void UKNOCKturneObjectPool::BeginPlay()
{
	Super::BeginPlay();

	if (PooledObjectSubclass != nullptr) {
		UWorld* const World = GetWorld();

		if (World != nullptr) {
			for (int count = 0; count < PoolSize; count++) {
				APooledObject* PoolableActor = World->SpawnActor<APooledObject>(PooledObjectSubclass, FVector().ZeroVector, FRotator().ZeroRotator);

				if (PoolableActor != nullptr) {
					PoolableActor->SetActive(false);
					PoolableActor->SetPoolIndex(count);
					PoolableActor->OnPooledObjectDespawn.AddDynamic(this, &UKNOCKturneObjectPool::OnPooledObjectDespawn);
					PoolableActor->OnPooledObjectTouchDespawn.AddDynamic(this, &UKNOCKturneObjectPool::OnPooledObjectTouchDespawn);
					ObjectPool.Add(PoolableActor);
				}
			}
		}
	}
}

APooledObject* UKNOCKturneObjectPool::SpawnPooledObject() {
	if (FirstIndex == LastIndex) {
		if (ObjectPool[FirstIndex]->IsActive()) {
			NTLOG(Warning, TEXT("Object pool is full"));
			return NULL;
		}
	}

	LastIndex = (LastIndex + 1) % PoolSize;
	auto locX = FMath::FRandRange(350.0f, 1150.0f);
	auto locY = FMath::FRandRange(370.0f, 1170.0f);
	ObjectPool[LastIndex]->TeleportTo(FVector(locX, locY, -20), FRotator::ZeroRotator);
	ObjectPool[LastIndex]->SetActive(true);
	ObjectPool[LastIndex]->SetActorEnableCollision(true);
	return ObjectPool[LastIndex];
}

/*
APooledObject* UKNOCKturneObjectPool::SpawnPooledObject() {
	for (APooledObject* PoolableActor : ObjectPool) {
		if (PoolableActor != nullptr && !PoolableActor->IsActive()) {
			PoolableActor->TeleportTo(FVector(0, 0, 0), FRotator(0, 0, 0));
			PoolableActor->SetLifeSpan(true);
			SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex());

			NTLOG(Warning, TEXT("Object spawned"));
			return PoolableActor;
		}
	}

	if (SpawnedPoolIndexes.Num() > 0) {
		int PooledObjectIndex = SpawnedPoolIndexes[0];
		SpawnedPoolIndexes.Remove(PooledObjectIndex);
		APooledObject* PoolableActor = ObjectPool[PooledObjectIndex];

		if (PoolableActor != nullptr) {
			PoolableActor->SetActive(false);

			PoolableActor->TeleportTo(FVector(0, 0, 0), FRotator(0, 0, 0));
			PoolableActor->SetLifeSpan(PooledObjectLifeSpan);
			PoolableActor->SetActive(true);
			SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex());

			return PoolableActor;
		}
	}
	return nullptr;
}
*/

void UKNOCKturneObjectPool::DeactivateAllSpawnedObject() {
	for (int index = 0; index < PoolSize; index++) {
		ObjectPool[index]->SetActive(false);
		ObjectPool[index]->SetActorLocation(FVector::Zero());
		ObjectPool[index]->SetActorEnableCollision(false);
	}
}

void UKNOCKturneObjectPool::OnPooledObjectDespawn(APooledObject* PoolActor) {
	ObjectPool[FirstIndex]->SetActive(false);
	ObjectPool[FirstIndex]->TeleportTo(FVector(0, 0, 0), FRotator(0, 0, 0));
	FirstIndex = (FirstIndex + 1) % PoolSize;
}

void UKNOCKturneObjectPool::OnPooledObjectTouchDespawn(APooledObject* PoolActor) {
	PoolActor->SetActive(false);
	PoolActor->SetActorEnableCollision(false);
}