#include "EarClippingTriangulator.h"

bool EarClippingTriangulator::ClockwiseTest() {
	float sum = 0;

	int j = this->verticies->size() - 1;
	for (size_t i = 0; i < this->verticies->size(); i++)
	{
		sum += (verticies->at(i).x - verticies->at(j).x)* (verticies->at(i).y + verticies->at(j).y);
		j = i;
	}


	return sum > 0;
}

bool EarClippingTriangulator::isInside(vec2 const& p, vec2 const& t1, vec2 const& t2, vec2 const& t3)
{
	bool a = cross(t1, t2, p) > 0;
	bool b = cross(t2, t3, p) > 0;
	bool c = cross(t3, t1, p) > 0;
	return a && b && c;
}

bool EarClippingTriangulator::usingElements() const {
    return true;
}

bool EarClippingTriangulator::init(const VertexArray *vertices, IndexArray *indices, VertexArray *output)
{
    if (!vertices || !indices)
        return false;
	this->verticies = vertices;
	this->indicies = indices;
    reset();
	initialized = true;
    return true;
}

/// <summary>
/// Make one step
/// If there are no collisions - create triangle
/// otherwise - move to next points
/// </summary>
/// <returns>Return true, if new triangle is created</returns>
void EarClippingTriangulator::Step() {
	if (MakeStep())
		watchdog = index.size();
	else
		watchdog--;
	if (index.size() <= 2 || !watchdog)
	{
		_finished = true;
	}
}
bool EarClippingTriangulator::MakeStep()//private
{
 	bool inside = false;
	//test, if no point is in the triangle 
	for (size_t i = 0; i < verticies->size(); i++)
	{
		if (isInside(verticies->at(i), verticies->at(index[prevIndex]), verticies->at(index[actualIndex]), verticies->at(index[nextIndex])))
		{
			inside = true;
			break;
		}
	}

	float convex = cross(verticies->at(index[prevIndex]), verticies->at(index[actualIndex]), verticies->at(index[nextIndex]));
	if (!inside && 0 < convex)
	{

		indicies->push_back(index[prevIndex]);
		indicies->push_back(index[actualIndex]);
		indicies->push_back(index[nextIndex]);
		index.erase(index.begin()+ actualIndex);
		prevIndex = prevIndex % index.size();
		actualIndex = (prevIndex +1) % index.size();
		nextIndex = (actualIndex +1) % index.size();
		return true;
	}
	else
	{
		//Next points
		prevIndex = (prevIndex + 1) % index.size();
		actualIndex = (actualIndex + 1) % index.size();
		nextIndex = (nextIndex + 1) % index.size();
		return false;
	}
}


/// <summary>
/// Make steps until new triangle is created
/// </summary>
void EarClippingTriangulator::NextTriangle()
{
	watchdog = index.size();
	while (index.size() > 2 && !MakeStep() && watchdog)
	{
		watchdog--;
	}
	if (index.size() <= 2 || !watchdog)
	{
		_finished = true;
	}
}

/// <summary>
/// triangulate whole polygon
/// </summary>
void EarClippingTriangulator::triangulate()
{
	while (index.size() > 2 && watchdog) {
		NextTriangle();
	}
	_finished = true;
}

void EarClippingTriangulator::reset() {
    index.clear();
    actualIndex = 0;
    nextIndex = 0;
    prevIndex = 0;
	_finished = false;
}

void EarClippingTriangulator::begin() {
    index.clear();
    bool clockwise = ClockwiseTest();
    if (clockwise)
    {
        for (size_t i = 0; i < verticies->size(); i++)
        {
            index.push_back(verticies->size() - 1 - i);
        }
    }
    else
    {
        for (size_t i = 0; i < verticies->size(); i++)
        {
            index.push_back(i);
        }
    }
    actualIndex = 0;
    nextIndex = 1;
    prevIndex = verticies->size() - 1;
	watchdog = index.size();
	_finished = verticies->size()<=2;
}

void EarClippingTriangulator::GetActualIndices(uint & actualIndex, uint & nextIndex, uint & prevIndex)
{
	if (initialized && index.size())
	{
	actualIndex = index[this->actualIndex];
	nextIndex = index[this->nextIndex];
	prevIndex = index[this->prevIndex];
	}
}
bool EarClippingTriangulator::finished()
{
	return _finished;
}
