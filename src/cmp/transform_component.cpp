#include "cmp/transform_component.h"
#include "bullet_emitter_component.h"

C_Transform::C_Transform()
{
}

C_Transform::C_Transform(v2 pos)
{
	set_pos(pos);
}

Entity C_Transform::get_entity() const
{
	return entity;
}

const Transform &C_Transform::get_local_transform() const
{
	return transform;
}

Transform C_Transform::get_world_transform() const
{
	return get_parent_transform() * transform;
}

void C_Transform::set_transform(const Transform &t)
{
	invalidate_cached_parent_transform_for_children();
	transform = t;
}

void C_Transform::set_pos(const v2 pos)
{
	invalidate_cached_parent_transform_for_children();
	transform.pos = pos;
}

void C_Transform::set_rotation(const float radians)
{
	invalidate_cached_parent_transform_for_children();
	transform.angle = radians;
}

Transform C_Transform::get_parent_transform() const
{
	//	if (!is_cached)
	{
		is_cached = true;

		cached_parent_transform =
			parent ? parent->get_world_transform() : Transform {};
	}

	return cached_parent_transform;
}

void C_Transform::add_child(C_Transform *child)
{
	assert(child->parent == nullptr);
	child->set_parent(this);
	children.add(child);
}

void C_Transform::remove_child(C_Transform *child)
{
	assert(child->parent == this);
	int i;
	for (i = children.count() - 1; i >= 0; i--)
	{
		if (children[i] == child)
		{
			children.unordered_remove(i);
			child->clear_parent();
			return;
		}
	}

	assert(false && "Parent child relationship is broken.");
}

void C_Transform::set_parent(C_Transform *p)
{
	invalidate_cached_parent_transform();
	parent = p;
}

void C_Transform::clear_parent()
{
	set_parent(nullptr);
}

void C_Transform::invalidate_cached_parent_transform()
{
	is_cached = false;
	invalidate_cached_parent_transform_for_children();
}

void C_Transform::invalidate_cached_parent_transform_for_children()
{
	for (auto child : children)
	{
		child->invalidate_cached_parent_transform();
	}
}

C_Transform &C_Transform::operator=(const Transform &t)
{
	set_transform(t);
	return *this;
}

C_Transform &C_Transform::operator=(const v2 &p)
{
	set_pos(p);
	return *this;
}

C_Transform &C_Transform::operator+=(const v2 &p)
{
	set_pos(transform.pos + p);
	return *this;
}

C_Transform &C_Transform::operator-=(const v2 &p)
{
	set_pos(transform.pos - p);
	return *this;
}

void on_transform_construct(World &world, Entity e)
{
	world.get<C_Transform>(e).entity = e;
}

void on_transform_update(World &world, Entity e)
{
	world.get<C_Transform>(e).entity = e;
}

void on_transform_destroy(World &world, Entity e)
{
	auto &t = world.get<C_Transform>(e);

	if (t.parent)
	{
		t.parent->remove_child(&t);
	}

	for (const auto &child : t.children)
	{
		child->clear_parent();
	}
}

void register_transform_callbacks(World &world)
{
	world.on_construct<C_Transform>().connect<&on_transform_construct>();
	world.on_update<C_Transform>().connect<&on_transform_update>();
	world.on_destroy<C_Transform>().connect<&on_transform_destroy>();
}