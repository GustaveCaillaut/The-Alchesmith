// Fill out your copyright notice in the Description page of Project Settings.


#include "Rune.h"

static const int32 N = 7;
static const int32 FusionMatrix[N * N] = {
	0, 1, 2, 3, 4, 5, 6, // Row None
	0, 0, 4, 0, 0, 0, 0, // Row Fire
	0, 4, 0, 0, 0, 0, 0, // Row Air
	0, 0, 0, 5, 6, 0, 0, // Row Earth
	0, 0, 0, 6, 0, 0, 0, // Row Lightning
	0, 0, 0, 0, 0, 0, 0, // Row Mountain
	0, 0, 0, 0, 0, 0, 0, // Row Magnetism
};
int32 URune::GetFusionID(ERuneID j) {
	int32 old_id = static_cast<int32>(this->id);
	int32 new_id = static_cast<int32>(j);
	return FusionMatrix[old_id * N + new_id];
};

void URune::Init(ERuneID idToSet) {

	bool temp[4] = { true, false, false, true };

	switch (idToSet) {

	case ERuneID::FIRE:
		this->id = idToSet;
		this->drawColor = FColor::Red;
		this->drawing.Append(temp, 4);

		break;

	case ERuneID::AIR:
		this->id = idToSet;
		this->drawColor = FColor(128, 128, 128, 255); //XXX
		this->drawing.Append(temp, 4);
		break;


	case ERuneID::EARTH:
		this->id = idToSet;
		this->drawColor = FColor::Cyan; //XXX
		this->drawing.Append(temp, 4);

		break;


	case ERuneID::LIGHTNING:
		this->id = idToSet;
		this->drawColor = FColor::Yellow;
		this->drawing.Append(temp, 4);

		break;

	case ERuneID::MOUNTAIN:
		this->id = idToSet;
		this->drawColor = FColor::Yellow;
		this->drawing.Append(temp, 4);

		break;

	case ERuneID::MAGNETISM:
		this->id = idToSet;
		this->drawColor = FColor::Yellow;
		this->drawing.Append(temp, 4);

		break;
	}

}

URune::~URune()
{
}
