/*=================================================================
 * Kernel that calculates smooth normals.
 *
 * Author: xverny00
 *=================================================================*/

__kernel void smoothNormals(      __global double*        normals,
	                        const __global double*        verts,
	                        const __global unsigned int* indices,
	                        const          unsigned int  iterations,
	                        const          unsigned int  idxSize,
	                        const          unsigned int  vtxSize)
{
	unsigned int global_id = (unsigned int) get_global_id(0);
	double3 result = (double3) (0.0f, 0.0f, 0.0f);

	/* Foreach vertex */
	if (global_id < vtxSize)
	{
		/* Search each index */
		for (int i = 0; i < idxSize; i++)
		{
			/* Read index values */
			unsigned int a = indices[i * 3 + 0];
			unsigned int b = indices[i * 3 + 1];
			unsigned int c = indices[i * 3 + 2];
			/* If index contains vertex */
			if (a == global_id || b == global_id || c == global_id)
			{
				/* Load vertices */
				double3 vertA = (double3) (verts[a * 3], verts[a * 3 + 1], verts[a * 3 + 2]);
				double3 vertB = (double3) (verts[b * 3], verts[b * 3 + 1], verts[b * 3 + 2]);
				double3 vertC = (double3) (verts[c * 3], verts[c * 3 + 1], verts[c * 3 + 2]);
				/* Calculate, normalize and sum triangle normal */
				double3 vec1  = vertC - vertB;
				double3 vec2  = vertA - vertB;
				double angle;
				if (a == global_id)
				{
					angle = dot(vertB - vertA, vertC - vertA);
					angle = acos(angle / (length(vertB - vertA) * length(vertC - vertA)));
				}
				if (b == global_id)
				{
					angle = dot(vertA - vertB, vertC - vertB);
					angle = acos(angle / (length(vertA - vertB) * length(vertC - vertB)));
				}
				if (c == global_id)
				{
					angle = dot(vertA - vertC, vertB - vertC);
					angle = acos(angle / (length(vertA - vertC) * length(vertB - vertC)));
				}
				result +=  normalize(cross(vec1, vec2)) * angle;
			}
		}
	}

	/* Synchronize threads before writing */
	//barrier(CLK_GLOBAL_MEM_FENCE);

	/* Save the normal */
	if (global_id < vtxSize)
	{
		result = normalize(result);
		normals[global_id * 3 + 0] = result.x;
		normals[global_id * 3 + 1] = result.y;
		normals[global_id * 3 + 2] = result.z;
	}

	/* Synchronize threads */
	//barrier(CLK_GLOBAL_MEM_FENCE);

	/* Iterate */
	for (int iter = 1; iter < iterations; iter++)
	{
		barrier(CLK_GLOBAL_MEM_FENCE);
		result = (double3) (0.0f, 0.0f, 0.0f);

		/* Foreach vertex */
		if (global_id < vtxSize)
		{
			/* Search each index */
			for (int i = 0; i < idxSize; i++)
			{
				/* Read index values */
				unsigned int a = indices[i * 3 + 0];
				unsigned int b = indices[i * 3 + 1];
				unsigned int c = indices[i * 3 + 2];
				/* If index contains vertex */
				if (a == global_id || b == global_id || c == global_id)
				{
					/* Load vertices */
					double3 vertA   = (double3) (verts[a * 3], verts[a * 3 + 1], verts[a * 3 + 2]);
					double3 vertB   = (double3) (verts[b * 3], verts[b * 3 + 1], verts[b * 3 + 2]);
					double3 vertC   = (double3) (verts[c * 3], verts[c * 3 + 1], verts[c * 3 + 2]);
					double3 normalA = (double3) (normals[a * 3], normals[a * 3 + 1], normals[a * 3 + 2]);
					double3 normalB = (double3) (normals[b * 3], normals[b * 3 + 1], normals[b * 3 + 2]);
					double3 normalC = (double3) (normals[c * 3], normals[c * 3 + 1], normals[c * 3 + 2]);
					double angle;
					if (a == global_id)
					{
						angle = dot(vertB - vertA, vertC - vertA);
						angle = acos(angle / (length(vertB - vertA) * length(vertC - vertA)));
					}
					if (b == global_id)
					{
						angle = dot(vertA - vertB, vertC - vertB);
						angle = acos(angle / (length(vertA - vertB) * length(vertC - vertB)));
					}
					if (c == global_id)
					{
						angle = dot(vertA - vertC, vertB - vertC);
						angle = acos(angle / (length(vertA - vertC) * length(vertB - vertC)));
					}
					result += normalize(normalA + normalB + normalC) * angle;
				}
			}
		}

		/* Synchronize threads before writing */
		//barrier(CLK_GLOBAL_MEM_FENCE);

		/* Save the normal */
		if (global_id < vtxSize)
		{
			result = normalize(result);
			normals[global_id * 3 + 0] = result.x;
			normals[global_id * 3 + 1] = result.y;
			normals[global_id * 3 + 2] = result.z;
		}
		//barrier(CLK_GLOBAL_MEM_FENCE);
	}
}
