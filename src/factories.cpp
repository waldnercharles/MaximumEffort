#include "factories.h"
#include "cmp/boomerang_movement_component.h"
#include "cmp/enemy_component.h"
#include "cmp/hitbox_component.h"
#include "cmp/hurtbox_component.h"
#include "cmp/input_component.h"
#include "cmp/movement_component.h"
#include "cmp/physics_component.h"
#include "cmp/player_component.h"
#include "cmp/projectile_component.h"
#include "cmp/spawner_component.h"
#include "cmp/sprite_component.h"
#include "cmp/target_component.h"
#include "cmp/weapon_component.h"

using namespace Cute;

entt::entity make_enemy_eyeball(entt::registry &reg, v2 pos)
{
	const entt::entity e = reg.create();
	reg.emplace<EnemyComponent>(e);

	auto &movement = reg.emplace<MovementComponent>(e);
	movement.pos = pos;
	movement.speed = V2(20, 20);

	auto &physics = reg.emplace<PhysicsComponent>(e);
	physics.aabb = make_aabb(V2(0, 0), 16, 16);

	auto &target = reg.emplace<TargetComponent>(e);
	target.entity = reg.view<PlayerComponent>().front();
	target.face_target = true;

	auto &hitbox = reg.emplace<HitboxComponent>(e);
	hitbox.circle = make_circle(V2(8, 8), 8);

	auto &hurtbox = reg.emplace<HurtboxComponent>(e);
	hurtbox.circle = make_circle(V2(8, 8), 8);

	auto &sprite = reg.emplace<SpriteComponent>(e, make_sprite("eyeball.ase"));
	sprite.layer = 50;

	return e;
}

entt::entity make_player(entt::registry &reg)
{
	const entt::entity e = reg.create();
	reg.emplace<PlayerComponent>(e);

	auto &m = reg.emplace<MovementComponent>(e);
	m.speed = V2(45, 45);

	reg.emplace<InputComponent>(e);

	auto &sprite = reg.emplace<SpriteComponent>(
		e,
		make_sprite("character.ase")
	);

	sprite.layer = 100;

	return e;
}

entt::entity make_enemy_spawner(
	entt::registry &reg,
	entt::entity parent,
	float rate,
	EnemyType spawn_type
)
{
	const entt::entity e = reg.create();
	auto &s = reg.emplace<SpawnerComponent>(e);
	s.rate = rate;
	s.entity_type = spawn_type;

	return e;
}

entt::entity make_weapon_boomerang(entt::registry &reg, entt::entity parent)
{
	const entt::entity e = reg.create();
	auto &w = reg.emplace<WeaponComponent>(e);
	w.parent = parent;
	w.rate = 0.5f;
	w.weapon_type = WEAPON_BOOMERANG;

	return e;
}