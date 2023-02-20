#ifndef __detour_h__
#define __detour_h__

#include "lua_kit.h"

#include "DetourCommon.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"

namespace ldetour {

    /// These are just sample areas to use consistent values across the samples.
    /// The use should specify these base on his needs.
    enum SamplePolyAreas {
        SAMPLE_POLYAREA_GROUND,
        SAMPLE_POLYAREA_WATER,
        SAMPLE_POLYAREA_ROAD,
        SAMPLE_POLYAREA_DOOR,
        SAMPLE_POLYAREA_GRASS,
        SAMPLE_POLYAREA_JUMP,
    };

    enum SamplePolyFlags {
        SAMPLE_POLYFLAGS_WALK = 0x01, // Ability to walk (ground, grass, road)
        SAMPLE_POLYFLAGS_SWIM = 0x02, // Ability to swim (water).
        SAMPLE_POLYFLAGS_DOOR = 0x04, // Ability to move through doors.
        SAMPLE_POLYFLAGS_JUMP = 0x08, // Ability to jump.
        SAMPLE_POLYFLAGS_DISABLED = 0x10, // Disabled polygon
        SAMPLE_POLYFLAGS_ALL = 0xffff // All abilities.
    };

    struct nav_set_header {
        int magic;
        int version;
        int num_tiles;
        dtNavMeshParams params;
    };

    struct nav_tile_header {
        dtTileRef tile_ref;
        int data_size;
    };

    typedef float nav_point[3]; // [x, y, z]

    class nav_query {
    public:
        ~nav_query();

        int create(dtNavMesh* mesh, const int max_nodes, float scaled);
    
        // 返回导航图上的随机一个点
        // [out]   pos         The random location.
        int random_point(lua_State* L);

        // 查找两点间的路径，如果不可达，则返回最接近终点的路径。
        // [in]    startPos    Path start position. [(x, y, z)]
        // [in]    endPos      Path end position. [(x, y, z)]
        // [out]   path        Points describing the straight path. [(x, y, z) * pathCount].
        int find_path(lua_State* L, int32_t sx, int32_t sy, int32_t sz, int32_t ex, int32_t ey, int32_t ez);
        
        // 射线检查
        // [in]    startPos    Path start position. [(x, y, z)]
        // [in]    endPos      Path end position. [(x, y, z)]
        // [out]   pos         Intersecting point. [(x, y, z)].
        int raycast(lua_State* L, int32_t sx, int32_t sy, int32_t sz, int32_t ex, int32_t ey, int32_t ez);

    private:
        int32_t pformat(float v);

    private:
        int max_polys;          // polys的长度
        int max_points;         // points的长度
        dtPolyRef* polys;       // 寻路过程中使用的临时缓存
        nav_point* points;      // 寻路过程中的路点缓存
        dtQueryFilter filter;
        dtNavMeshQuery* nvquery;
        float qscale = 1.0f; 
    };

    class nav_mesh
    {
    public:
        ~nav_mesh();

        int get_max_tiles();

        int create(const char* buf, size_t sz);

        nav_query* create_query(const int max_nodes, float scale);

    private:
        dtNavMesh* nvmesh = nullptr;
    };

}

#endif // __detour_h__