/**
*	@file	Entity.cpp
*	@brief	敵の制御用
*	@author	Takuya Yokoyama
*/


#include "Enemy.h"
#include "../../Engine/GameEngine.h"
#include "../../GameState.h"
#include "../../../Res/Audio/testProject_acf.h"
#include "../../../Res/Audio/CueSheet_0.h"
#include "../EnemyLaunchController.h"
#include "../GameMainScene.h"
#include "../../Engine/GLFWEW.h"
#include "../../../Res/Resource.h"

#include "Effect.h"
#include "Item.h"
#include "Bullet.h"
#include "Player.h"
#include <memory>

namespace  Application{

	void Toroid::Initialize() {
		entity->CastShadow(true);
		entity->CastStencil(true);	
		entity->StencilColor(glm::vec4(1, 0, 1, 1));
		entity->Scale(glm::vec3(1.5));

		entity->Collision(GameState::collisionDataList[GameState::EntityGroupId_Enemy]);

		return;
	}

	void Toroid::Update(float deltaTime) {

		timer += deltaTime;

		//弾管理システムの更新
		if (bulletGenerator) {
			//bulletGenerator->Color(bulletColor);
			bulletGenerator->Update(deltaTime);
		}

		//移動管理システムの更新
		if (moveController) {
			moveController->Update(*entity, deltaTime);
		}

		// 円盤を回転させる.
		float rot = glm::angle(entity->Rotation());
		rot += glm::radians(180.0f) * deltaTime;
		if (rot > glm::pi<float>() * 2.0f) {
			rot -= glm::pi<float>() * 2.0f;
		}
		entity->Rotation(glm::angleAxis(rot, glm::vec3(0, 1, 0)));

		//画面外判定処理
		if (isDestroyByScreenOut) {
			const glm::vec3 pos = entity->Position();
			if (std::abs(pos.x) > GameState::screenHalfW * 1.2 || std::abs(pos.z) > GameState::screenHalfH * 1.2) {

				std::cout << "enemy of outrange" << std::endl;
				GameEngine::Instance().RemoveEntity(entity);
				return;
			}
		}
	}

	void Toroid::Damage(float p) {

		if (--hp <= 0) {

			GameEngine& game = GameEngine::Instance();

			if (--hp <= 0) {

				//爆発エフェクト
				if (Entity::Entity* p = game.AddEntity(GameState::EntityGroupId_Others, entity->Position(), "Blast", Resource::tex_toroid, std::make_shared<Application::Blast>())) {
					const std::uniform_real_distribution<float> rotRange(0.0f, glm::pi<float>() * 2);
					p->Rotation(glm::quat(glm::vec3(0, rotRange(game.Rand()), 0)));
					p->Color(glm::vec4(1.0f, 0.75f, 0.5f, 1.0f));
					game.UserVariable("score") += 100;
				}

				game.PlayAudio(0, CRI_CUESHEET_0_EXPLOSIVE);

				if (isItemDrop) {
					//アイテムドロップ処理

					int itemID = rand() % 2;

					//アイテム
					std::string texName = itemID ? Resource::tex_itemboxSpeed : Resource::tex_itemboxBullet;

					if (Entity::Entity* p = game.AddEntity(GameState::EntityGroupId_Item, entity->Position(), "ItemBox", texName.c_str(), std::make_shared<Item>(itemID))) {
						p->Collision(GameState::collisionDataList[GameState::EntityGroupId_Item]);
					}
				}

				entity->Destroy();
			}
			else {
				game.PlayAudio(1, CRI_CUESHEET_0_DAMAGE);
			}
		}
	}

	void Toroid::CollisionEnter(Entity::Entity& e) {
		e.EntityData()->Damage(1);

	}

	void Toroid::Target(Entity::Entity* t) {

		playerEntity = t;

		if (bulletGenerator) {
			bulletGenerator->Target(t);
		}
	}

	void BossEnemy::Initialize() {

		GameEngine& game = GameEngine::Instance();

		entity->Scale(glm::vec3(11));
		entity->Collision({ { -15,-20,-10},{15,20,10} });

		//タレット用エンティティの生成処理
		for (int i = 0; i < 8; i++) {

			auto t = std::make_shared<Toroid>();
			t->DestroyByScreenOut(false);

			if (auto p = game.AddEntity(GameState::EntityGroupId_Others, glm::vec3(-60,0,0),
				"", Resource::tex_toroid, Resource::tex_toroid_normal, t)) {

				turrets.push_back(t);

				//タレット用弾作成
				BulletGeneratorPtr b;
				if ((i % 2) == 0) {
					b = std::make_shared<MultiWayShot>(*p, GameState::EntityGroupId_EnemyShot, nullptr, 15, 3);
				}
				else {
					b = std::make_shared<NormalShot>(*p, GameState::EntityGroupId_EnemyShot, playerEntity);
				}

				t->BulletGenerator(b);
				b->Color(glm::vec4(0, 0.2, 1, 1));
			}
		}

		glm::vec2 windowSize = game.WindowSize();

		//HPゲージ作成
		hpGuage = std::make_shared<HealthGuage>(Texture::LoadFromFile(Resource::tex_healthMeter));
		hpGuage->Program(Shader::Program::Create("Res/Shader/HealthGauge.vert", "Res/Shader/HealthGauge.frag"));
		glm::vec3 pos = hpGuage->Position();
		pos += glm::vec3(0, windowSize.y * 0.45, 0);
		hpGuage->Position(pos);
		hpGuage->Scale(glm::vec2(5, 3));
		hpGuage->Rectangle({ glm::vec2(0,128),glm::vec2(128,12) });	

		hp = maxHp;
	}

	void BossEnemy::Update(float deltaTime) {

		timer += deltaTime;

		//移動管理システムの更新
		if (moveController) {
			moveController->Update(*entity, deltaTime);
		}

		// 円盤を回転させる.
		float rot = glm::angle(entity->Rotation());
		rot += glm::radians(30.0f) * deltaTime;
		if (rot > glm::pi<float>() * 2.0f) {
			rot -= glm::pi<float>() * 2.0f;
		}
		entity->Rotation(glm::angleAxis(rot, glm::vec3(0, 1, 0)));

		//タレットの位置情報更新処理
		UpdateTurret();

		//HPゲージの表示
		GameEngine::Instance().AddSprite(*hpGuage);
	}

	void BossEnemy::Damage(float p) {

		GameEngine& game = GameEngine::Instance();

		if (--hp <= 0) {

			game.PlayAudio(1, CRI_CUESHEET_0_EXPLOSIVE2);

			GameEngine& game = GameEngine::Instance();

			//爆発エフェクト
			if (Entity::Entity* p = game.AddEntity(GameState::EntityGroupId_Others, entity->Position(),
				"Blast", Resource::fbx_blast, std::make_shared<Blast>())) {
				const std::uniform_real_distribution<float> rotRange(0.0f, glm::pi<float>() * 2);
				p->Scale(glm::vec3(10.0f));
				p->Rotation(glm::quat(glm::vec3(0, rotRange(game.Rand()), 0)));
				p->Color(glm::vec4(1.0f, 0.75f, 0.5f, 1.0f));
				game.UserVariable("score") += 100;

				game.PlayAudio(0, CRI_CUESHEET_0_EXPLOSIVE);

				for (auto turret : turrets) {
					turret->Entity()->Destroy();
				}
				dynamic_cast<MainGame*>(&SceneStack::Instance().Current())->StageClear(5);
			}

			entity->Destroy();
		}
		else {

			game.PlayAudio(1, CRI_CUESHEET_0_DAMAGE);

			//HPゲージの更新
			std::dynamic_pointer_cast<HealthGuage>(hpGuage)->Ratio(
				static_cast<float>(hp) / static_cast<float>(maxHp));

			//砲台の更新
			if ((static_cast<float>(hp) / maxHp) > 0.6) {
				for (auto turret : turrets) {

					turret->BulletGenerator()->BulletSpeed(20.0);
					turret->BulletGenerator()->ShotInterval(2.0f);
 					turret->BulletGenerator()->Color(glm::vec4(0, 0.2, 1, 1));
				}
			}
			else if(static_cast<float>(hp) / maxHp > 0.3){
				for (auto turret : turrets) {

					turret->BulletGenerator()->BulletSpeed(15.0);
					turret->BulletGenerator()->ShotInterval(1.0f);
					turret->BulletGenerator()->Color(glm::vec4(1, 1, 0, 1));
				}
			}
			else {
				for (auto turret : turrets) {
					std::shared_ptr<MultiWayShot> b = std::dynamic_pointer_cast<MultiWayShot>(turret->BulletGenerator());

					turret->BulletGenerator()->BulletSpeed(15.0);
					
					if (b) {
						b->MaxBulletNum(10);
						b->ShotInterval(0.5f);
						b->AngleInterval(15);
					}
					else {
						turret->BulletGenerator()->ShotInterval(0.7f);
					}
					turret->BulletGenerator()->Color(glm::vec4(1, 0, 0, 1));
				}
			}

		}
	}

	void BossEnemy::CollisionEnter(Entity::Entity &e) {

		e.EntityData()->Damage(1);
	}

	void BossEnemy::UpdateTurret() {

		glm::quat baseRot = entity->Rotation();
		glm::vec3 centerPos = entity->Position();
		glm::vec3 forward = glm::vec3(-1, 0, 0);

		for (int i = 0; i < 8; i++) {

			const float perRad = glm::pi<float>() * 2.0f * (1.0f / 8.0f);
			float rot = glm::pi<float>() * 2.0f * 0.125f * i;
			float offset = 0.15f;
			glm::quat tmp = glm::angleAxis(rot + offset, glm::vec3(0, 1, 0));

			tmp = tmp * baseRot;

			glm::vec3 vec = tmp * (forward * 17.0f);

			glm::vec3 offsetPos = glm::vec3(0, 0, 0);

			turrets[i]->Entity()->Position(centerPos + vec + offsetPos);
			turrets[i]->BulletGenerator()->Rotation(tmp);
		}
	}

	void BossEnemy::Target(Entity::Entity* t) {

		playerEntity = t;
		for (auto turret : turrets) {
			if (turret) {
				turret->Target(t);
			}
		}
	}

}
