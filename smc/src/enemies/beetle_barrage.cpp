#include "beetle_barrage.hpp"
#include "../core/errors.hpp"
#include "../core/xml_attributes.hpp"
#include "../core/game_core.hpp"
#include "../level/level_player.hpp"

using namespace SMC;

cBeetleBarrage::cBeetleBarrage(cSprite_Manager* p_sprite_manager)
	: cEnemy(p_sprite_manager)
{
	Init();
}

cBeetleBarrage::cBeetleBarrage(XmlAttributes& attributes, cSprite_Manager* p_sprite_manager)
	: cEnemy(p_sprite_manager)
{
	Init();

	// position
	Set_Pos(attributes.fetch<float>("posx", 0), attributes.fetch<float>("posy", 0), true);
}

cBeetleBarrage::~cBeetleBarrage()
{
	//
}

void cBeetleBarrage::Init()
{
	m_type = TYPE_BEETLE_BARRAGE;
	m_pos_z = 0.093f;
	m_gravity_max = 24.0f;
	m_name = "Beetle Barrage";

	Add_Image(pVideo->Get_Surface(utf8_to_path("enemy/beetle_barrage/1.png")));
	Add_Image(pVideo->Get_Surface(utf8_to_path("enemy/beetle_barrage/2.png")));
	Add_Image(pVideo->Get_Surface(utf8_to_path("enemy/beetle_barrage/3.png")));

	// Add an animation, but don’t yet play it. We will only
	// play the animation when spitting out the beetles.
	Set_Animation_Image_Range(0, 3);
	Set_Time_All(180, true);
	Reset_Animation();
	Set_Image_Num(0);
	Set_Animation(false);

	//Set_Moving_State(STA_STAY);
	Set_Moving_State(STA_RUN);
	Set_Direction(DIR_UP);

	// TODO: Own die sound
	m_kill_sound = "enemy/eato/die.ogg";
	m_kill_points = 100;
}

cBeetleBarrage* cBeetleBarrage::Copy() const
{
	cBeetleBarrage* p_bb = new cBeetleBarrage(m_sprite_manager);
	p_bb->Set_Pos(m_start_pos_x, m_start_pos_y);
	p_bb->Set_Direction(m_start_direction);
	return p_bb;
}

std::string cBeetleBarrage::Get_XML_Type_Name()
{
	return "beetle_barrage";
}

xmlpp::Element* cBeetleBarrage::Save_To_XML_Node(xmlpp::Element* p_element)
{
	xmlpp::Element* p_node = cEnemy::Save_To_XML_Node(p_element);

	Add_Property(p_node, "direction", Get_Direction_Name(m_start_direction));

	return p_node;
}

void cBeetleBarrage::DownGrade(bool force /* = false */)
{
	if (force) // Falling/lava death
		Set_Rotation_Z(180.0f);

	Set_Dead(true);
	m_massive_type = MASS_PASSIVE;
	m_velx = 0.0f;
	m_vely = 0.0f;
}

void cBeetleBarrage::Update_Dying()
{
	m_counter += pFramerate->m_speed_factor;

	// Forced death
	if (Is_Float_Equal(m_rot_z, 180.0f)) {
		// A little bit upwards first
		if (m_counter < 5.0f)
			Move(0.0f, 5.0f);
		// if not below the grund: fall
		else if (m_col_rect.m_y < pActive_Camera->m_limit_rect.m_y)
			Move(0.0f, 20.0f);
		// if below disable
		else {
			m_rot_z = 0.0f;
			Set_Active(false);
		}
	}
	// Normal death
	else {
		float speed = pFramerate->m_speed_factor * 0.05f;
		Add_Scale_X(-speed * 0.5f);
		Add_Scale_Y(-speed);

		if (m_scale_y < 0.01f) {
			Set_Scale(1.0f);
			Set_Active(false);
		}
	}
}

void cBeetleBarrage::Update()
{
	cEnemy::Update();

	if (!m_valid_update || !Is_In_Range())
		return;

	Update_Velocity();
	Update_Animation();
	Update_Gravity();
}

bool cBeetleBarrage::Is_Update_Valid()
{
	if (m_dead || m_freeze_counter)
		return false;
	else
		return true;
}

Col_Valid_Type cBeetleBarrage::Validate_Collision(cSprite* p_obj)
{
	Col_Valid_Type basic_valid = Validate_Collision_Ghost(p_obj);

	// found valid collision
	if (basic_valid != COL_VTYPE_NOT_POSSIBLE)
		return basic_valid;

	if (p_obj->m_massive_type == MASS_MASSIVE) {
		switch(p_obj->m_type) {
		case TYPE_FLYON:
		case TYPE_ROKKO: // fallthru
		case TYPE_GEE:   // fallthru
			return COL_VTYPE_NOT_VALID;
		default:
			return COL_VTYPE_BLOCKING;
		}
	}
	else if (p_obj->m_massive_type == MASS_HALFMASSIVE) {
		// If moving downwards and the object is on bottom
		if (m_vely >= 0.0f && Is_On_Top(p_obj))
			return COL_VTYPE_BLOCKING;
	}
	else if (p_obj->m_massive_type == MASS_PASSIVE) {
		if (p_obj->m_type == TYPE_ENEMY_STOPPER)
			return COL_VTYPE_BLOCKING;
	}

	return COL_VTYPE_NOT_VALID;
}

void cBeetleBarrage::Handle_Collision_Player(cObjectCollision* p_collision)
{
	// invalid
	if (p_collision->m_direction == DIR_UNDEFINED)
		return;

	// only if not invincable
	if (pLevel_Player->m_invincible <= 0.0f) {
		// If player is big and does not come from the bottom
		if (pLevel_Player->m_maryo_type != MARYO_SMALL && p_collision->m_direction != DIR_BOTTOM)
			pLevel_Player->Action_Jump(true);

		pLevel_Player->DownGrade_Player();
	}
}

void cBeetleBarrage::Handle_Collision_Enemy(cObjectCollision* p_collision)
{
	Send_Collision(p_collision);
}

void cBeetleBarrage::Handle_Collision_Massive(cObjectCollision* p_collision)
{
	if (m_state == STA_OBJ_LINKED)
		return;

	// TODO
}

void cBeetleBarrage::Set_Moving_State(Moving_state new_state)
{
	if (new_state == m_state)
		return;

	if (new_state == STA_STAY)
		Set_Animation(false);
	else if (new_state == STA_RUN)
		Set_Animation(true);
	else if (new_state == STA_FALL)
		{ /* Ignore */ }
	else
		throw(InvalidMovingStateError(new_state));

	Reset_Animation();
	Set_Image_Num(0);
	m_state = new_state;
}
