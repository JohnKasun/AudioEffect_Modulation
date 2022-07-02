#include "Chorus.h"

Chorus::Chorus()
{
}

Chorus::~Chorus()
{
}

Error_t Chorus::setDepth(float newDepth)
{
	return Error_t();
}

Error_t Chorus::setSpeed(float newSpeed)
{
	return Error_t();
}

Error_t Chorus::setShape(Chorus::Shape newShape)
{
	return Error_t();
}

float Chorus::getDepth() const
{
	return 0.0f;
}

float Chorus::getSpeed() const
{
	return 0.0f;
}

Chorus::Shape Chorus::getShape() const
{
	return Chorus::Shape();
}
