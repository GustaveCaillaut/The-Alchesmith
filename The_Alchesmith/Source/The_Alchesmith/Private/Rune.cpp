// Fill out your copyright notice in the Description page of Project Settings.


#include "Rune.h"

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
	}

}

URune::~URune()
{
}
