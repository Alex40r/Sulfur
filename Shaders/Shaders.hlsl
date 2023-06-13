[[vk::binding(0,0)]]
cbuffer UBO
{
	int frame;
	int width;
	int height;
	float fov;

	float4x4 view;
	float4x4 view_inverted;
	float4x4 projection;

	int voxelDepth;
};

#define VOXEL_CHUNK_DEPTH 4
#define VOXEL_CHUNK_SIZE (1 << VOXEL_CHUNK_DEPTH)
#define VOXEL_CHUNK_WIDTH (VOXEL_CHUNK_SIZE * VOXEL_CHUNK_SIZE * VOXEL_CHUNK_SIZE)
#define VOXEL_REFERENCE_BIT 0x80000000U
#define VOXEL_ROOT 0
#define VOXEL_MAX_DEPTH 4

struct Chunk
{
	uint bitmap[VOXEL_CHUNK_WIDTH / 32];
	uint data[VOXEL_CHUNK_WIDTH];
};

[[vk::binding(1,0)]]
StructuredBuffer<Chunk> chunks;

[[vk::binding(2,0)]]
StructuredBuffer<float> stats;

/* ---- ---- ---- ---- */

static const float EPSILON = 0.0001;

[shader("vertex")]
float4 Vertex(int vertex : SV_VertexID) : SV_POSITION
{
	switch (vertex)
	{
		case 0:
			return float4(-1, -1, 0, 1);
		case 1:
			return float4(1, -1, 0, 1);
		case 2:
			return float4(-1, 1, 0, 1);
		case 3:
			return float4(1, 1, 0, 1);
		default:
			return float4(0, 0, 0, 1);
	}
}


float3 GetRayDirection(float2 pos)
{
	float2 xy = pos.xy - float2(width, height) / 2;
	xy.y *= -1;
	return normalize(float3(xy, height / tan(radians(fov * 2) / 2.0)));
}

static const float STATS_WIDTH = 800.0;
static const float STATS_HEIGHT = 400.0;
static const float STATS_BORDER = 4.0;

float4 RuntimeStats(float4 position, out bool visible)
{
	visible = false;

	if (position.x < -STATS_BORDER || position.x > STATS_WIDTH + STATS_BORDER || position.y < -STATS_BORDER || position.y > STATS_HEIGHT + STATS_BORDER)
		return 0;

	visible = true;

	if (position.x < 0 || position.x > STATS_WIDTH || position.y < 0 || position.y > STATS_HEIGHT)
		return 0.1;

	float x = STATS_WIDTH - position.x;
	float y = STATS_HEIGHT - position.y;

	float min = 0;
	float max = 6;

	float stat = stats[x];

	float sy = (stat - min) / (max - min) * STATS_HEIGHT;

	if (abs(sy - y) <= 0.5)
		return lerp(float4(0, 1, 0, 0), float4(1, 0, 0, 0),
			(sy / STATS_HEIGHT));

	if (sy < 0)
		return float4(0, 0.02, 0, 0);

	if (sy > STATS_HEIGHT)
		return float4(0.02, 0, 0, 0);

	float r = round(((y / STATS_HEIGHT) * (max - min) + min) / 0.5) * 0.5;
	float sr = (r - min) / (max - min) * STATS_HEIGHT;

	if (abs(sr - y) <= 0.5)
		return 0.02;

	return 0;
}

#define MAX_RUNS 1024

[shader("pixel")]
float4 Pixel(float4 position : SV_POSITION) : SV_TARGET
{
	bool visible = false;
	float4 color = RuntimeStats(position, visible);

	if (visible)
		return color;

	float3 raydir = mul((float3x3) view_inverted, GetRayDirection(position.xy));
	float3 origin = mul(view_inverted, float4(0, 0, 0, 1)).xyz;

	int depth = 0;

	float3 delta = abs(1.0 / raydir);
	float3 ray_sign = sign(raydir);

	uint chunk_ids[VOXEL_MAX_DEPTH];
	chunk_ids[0] = VOXEL_ROOT;

	float3 in_chunk_offsets[VOXEL_MAX_DEPTH];
	in_chunk_offsets[0] = origin;

	float3 in_chunk_positions[VOXEL_MAX_DEPTH];
	in_chunk_positions[0] = floor(origin);

	float3 part2 = ((ray_sign * 0.5) + 0.5) * delta;
	float3 part1 = ray_sign * delta;

	bool skip = false;

	for (int runs = 0; runs < MAX_RUNS; runs++)
	{
		float3 side_distance = mad(in_chunk_positions[depth] - in_chunk_offsets[depth], part1, part2);
		bool3 mask = false;
		Chunk chunk = chunks.Load(chunk_ids[depth]);

		float3 p = in_chunk_positions[depth];

		[unroll]
		for (; runs < MAX_RUNS; runs++)
		{
			uint cell_id = dot(p, float3(1, VOXEL_CHUNK_SIZE, VOXEL_CHUNK_SIZE * VOXEL_CHUNK_SIZE));

			bool a = !skip && (any(p < 0) || any(p >= VOXEL_CHUNK_SIZE));

			if (a && depth == 0)
				return float4(0.02, 0.02, 0.02, 1);

			depth -= a;
			skip = a || skip;

			if (a)
				break;

			uint data = chunk.data[cell_id];

			float d = length(mask * (side_distance - delta));

			bool hit = !skip && data != 0;

			if (hit && !(data & VOXEL_REFERENCE_BIT))
				return float4(data % 256 / 1024.0, 0, 0, 1);

			float3 off = in_chunk_offsets[depth];

			depth += hit;


			chunk_ids[depth] = (data & ~VOXEL_REFERENCE_BIT) * hit + chunk_ids[depth] * (!hit);

			in_chunk_offsets[depth] =
			lerp(off, clamp(
					mad(raydir, d, off - p)
					* VOXEL_CHUNK_SIZE,
					0.0, VOXEL_CHUNK_SIZE - EPSILON), hit);

			in_chunk_positions[depth] = floor(lerp(p, in_chunk_offsets[depth], hit));

			if (hit)
				break;

			skip = false;
			mask = side_distance <= min(side_distance.yzx, side_distance.zxy);
			side_distance += mask * delta;
			p += mask * ray_sign;
			in_chunk_positions[depth] = p;
		}
	}

	return float4(0.01, 0.01, 0.1, 1);

}