#include "MultiplicativePRNG.h"

MultiplicativePRNG::MultiplicativePRNG(long long module, long long seed, int multiplier) : module(module), multiplier(multiplier), seed(seed)
{
	this->last = seed;
}

MultiplicativePRNG::MultiplicativePRNG(const MultiplicativePRNG* source) : module(source->module), multiplier(source->multiplier), seed(source->seed)
{
	this->last = source->last;
}

MultiplicativePRNG::~MultiplicativePRNG()
= default;

double MultiplicativePRNG::next() const
{
	last = (last * multiplier) % module;
	return (double)last / module;
}

double MultiplicativePRNG::next(double from, double to) const
{
	return from + (to - from) * next();
}

void MultiplicativePRNG::reset() const
{
	last = seed;
}

MultiplicativePRNG* MultiplicativePRNG::clone() const
{
	return new MultiplicativePRNG(this);
}
