#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	left_flipper_rect.x = 15;
	left_flipper_rect.y = 130;
	left_flipper_rect.w = 56;
	left_flipper_rect.h = 18;

	right_flipper_rect.x = 87;
	right_flipper_rect.y = 131;
	right_flipper_rect.w = 56;
	right_flipper_rect.h = 18;

	ball_rect.x = 184;
	ball_rect.y = 72;
	ball_rect.w = 15;
	ball_rect.h = 14;

	launcher_rect.x = 349;
	launcher_rect.y = 289;
	launcher_rect.w = 13;
	launcher_rect.h = 2;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	textures = App->textures->Load("pinball/pinball_elements_2.png");

	left_flipper = App->physics->CreateRectangle(100, 100, 40, 10, b2_dynamicBody);
	left_flipper_aux = App->physics->CreateCircle(80, 100, 10, b2_staticBody, true);
	App->physics->CreateRevoluteJoint(left_flipper, left_flipper_aux, { -40, 0 }, { 0, 0 }, 40, -30);

	right_flipper = App->physics->CreateRectangle(200, 200, 40, 10, b2_dynamicBody);
	right_flipper_aux = App->physics->CreateCircle(220, 200, 10, b2_staticBody, true);
	App->physics->CreateRevoluteJoint(right_flipper, right_flipper_aux, { 40, 0 }, { 0, 0 }, 30, -40);

	launcher = App->physics->CreateRectangle(397, 620, 19, 10, b2_dynamicBody);
	launcher->body->GetFixtureList()->SetRestitution(0.4f);
	launcher_aux = App->physics->CreateCircle(397, 680, 15, b2_staticBody, true);
	App->physics->CreateDistanceJoint(launcher, launcher_aux, 30.0f, 1.0f);

	ball = App->physics->CreateCircle(ball_starting_pos.x, ball_starting_pos.y, 6, b2_dynamicBody);
	ball->body->GetFixtureList()->SetDensity(1000.0f);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(textures);
	/* necessary ?-
	// Destroy left flipper
	App->physics->world->DestroyBody(left_flipper->b_attached);
	App->physics->world->DestroyBody(left_flipper->body);
	left_flipper = nullptr;

	App->physics->world->DestroyBody(right_flipper->b_attached);
	App->physics->world->DestroyBody(right_flipper->body);
	right_flipper = nullptr;
	*/
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	int x, y;
	ball->GetPosition(x, y);
	App->renderer->Blit(textures, x, y, &ball_rect, 1.0f, ball->GetRotation());

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		launcher->body->ApplyForceToCenter({0, 2000}, true);
	}	

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		left_flipper->body->ApplyAngularImpulse(DEGTORAD * -360, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		right_flipper->body->ApplyAngularImpulse(DEGTORAD * 360, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		left_flipper->body->ApplyAngularImpulse(DEGTORAD * 650, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		right_flipper->body->ApplyAngularImpulse(DEGTORAD * -650, true);
	}

	int xx, yy;
	launcher->GetPosition(xx, yy);
	App->renderer->Blit(textures, xx + 1, yy, &launcher_rect, 1.0f, launcher->GetRotation());
	
	return UPDATE_CONTINUE;
}