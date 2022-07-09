#pragma once

#include "ErrorDef.h"

class ModulationBase;
class ModulationIf {
public:
	enum class Type {
		Chorus,
		Flanger,
		Phaser
	};
	ModulationIf();
	~ModulationIf();
private:
	ModulationBase* mMod = nullptr;
};