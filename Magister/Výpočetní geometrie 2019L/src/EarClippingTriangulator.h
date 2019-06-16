
#include "Triangulator.h"

class EarClippingTriangulator: public Triangulator {
	const VertexArray *verticies = NULL;
	IndexArray *indicies = NULL;
	std::vector <uint> index;
	uint actualIndex;
	uint nextIndex;
	uint prevIndex;
	bool initialized = false;
	int watchdog;
	bool _finished = true;
public:
    bool usingElements() const;
    bool init(const VertexArray *vertices, IndexArray *indices, VertexArray *output);
    void triangulate();
    void reset();
    
    void begin();
    
	void Step();
	void NextTriangle();
	void GetActualIndices(uint &actualIndex,
                          uint &nextIndex,
                          uint &prevIndex);
	bool finished();
private:
	bool MakeStep();
	bool ClockwiseTest();
	bool isInside(vec2 const& p, vec2 const& t1, vec2 const& t2, vec2 const& t3);
};
