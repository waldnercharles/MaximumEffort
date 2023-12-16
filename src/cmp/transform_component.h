#pragma once
#include "common.h"

#include <cute.h>
#include <entt/entt.hpp>

struct Transform
{
	v2 pos;
	float angle;
};

inline Transform operator*(const Transform &a, const Transform &b)
{
	return {a.pos + b.pos, a.angle + b.angle};
}

void on_transform_construct(World &world, Entity e);
void on_transform_update(World &world, Entity e);
void on_transform_destroy(World &world, Entity e);

void register_transform_callbacks(World &world);

struct C_Transform
{
	C_Transform();
	C_Transform(v2 pos);

	Entity get_entity() const;

	const Transform &get_local_transform() const;
	Transform get_world_transform() const;
	Transform get_parent_transform() const;

	void set_transform(const Transform &t);
	void set_pos(const v2 pos);
	void set_rotation(const float radians);

	void add_child(C_Transform *child);
	void remove_child(C_Transform *child);

	C_Transform &operator=(const Transform &);
	C_Transform &operator=(const v2 &);
	C_Transform &operator+=(const v2 &pos);
	C_Transform &operator-=(const v2 &pos);

private:
	Entity entity = entt::null;

	C_Transform *parent = nullptr;
	Array<C_Transform *> children = {};

	Transform transform = {};

	mutable bool is_cached = false;
	mutable Transform cached_parent_transform = {};

	void set_parent(C_Transform *p);
	void clear_parent();

	void invalidate_cached_parent_transform();
	void invalidate_cached_parent_transform_for_children();

	friend void on_transform_construct(World &world, Entity e);
	friend void on_transform_update(World &world, Entity e);
	friend void on_transform_destroy(World &world, Entity e);
};

// Ensure TransformComponents are not relocated in memory. (Allows stable pointer)
template <>
struct entt::component_traits<C_Transform>
{
	using type = C_Transform;
	static constexpr bool in_place_delete = true;
	static constexpr std::size_t page_size = ENTT_PACKED_PAGE;
};
