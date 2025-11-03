// Fill out your copyright notice in the Description page of Project Settings.


#include "Rune.h"

static const int32 N = 13;
static const int32 FusionMatrix[N * N] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, // Row None 0
	0, 7, 4, 9, 0, 0,  0, 0, 0, 0, 12,  0,  0, // Row Fire 1
	0, 4, 0, 8, 0, 0,  0, 0, 0, 0, 3,  0,  0, // Row Air 2
	0, 9, 8, 5, 6, 0,  0, 0, 0, 0, 2,  0,  0, // Row Earth 3
	0, 0, 0, 6, 0, 0,  0, 0, 0, 0, 0,  0,  0, // Row Lightning 4
	0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0,  0,  0, // Row Mountain 5
	0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0,  0,  0, // Row Magnetism 6
	0, 0, 0, 0, 0, 0,  0, 0, 10, 0, 0, 0,  0, // Row Light 7
	0, 0, 0, 0, 0, 0,  0, 10, 0, 0, 0, 0,  0, // Row Darkness 8
	0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0,  0, // Row Steel 9
	0, 12, 3, 2, 0, 0, 0, 0, 0, 0, 0,  0,  1, // Row Inversion 10
	0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0,  0, // Row Gravity 11
	0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 1,  0,  0, // Row Sea 12
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

	case ERuneID::LIGHT:
		this->id = idToSet;
		this->drawColor = FColor::Yellow;
		this->drawing.Append(temp, 4);

		break;

	case ERuneID::DARKNESS:
		this->id = idToSet;
		this->drawColor = FColor::Yellow;
		this->drawing.Append(temp, 4);

		break;

	case ERuneID::STEEL:
		this->id = idToSet;
		this->drawColor = FColor::Yellow;
		this->drawing.Append(temp, 4);

		break;

	case ERuneID::INVERSION:
		this->id = idToSet;
		this->drawColor = FColor::Yellow;
		this->drawing.Append(temp, 4);

		break;

	case ERuneID::GRAVITY:
		this->id = idToSet;
		this->drawColor = FColor::Yellow;
		this->drawing.Append(temp, 4);

		break;

	case ERuneID::SEA:
		this->id = idToSet;
		this->drawColor = FColor::Yellow;
		this->drawing.Append(temp, 4);

		break;
	}

}

URune::~URune()
{
}
