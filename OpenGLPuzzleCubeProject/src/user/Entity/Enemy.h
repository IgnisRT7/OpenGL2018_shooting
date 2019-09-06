/**
*	@file Enemy.h
*	@brief	敵の制御用
*	@author	Takuya Yokoyama
*/

#pragma once

#include "../../Engine/Entity.h"
#include "../BulletGenerator.h"
#include "../EnemyMoveController.h"
#include "../../Engine/Sprite.h"

namespace Application {

	/// 敵
	class Toroid : public Entity::EntityDataBase {
	public:

		Toroid(int typeID = 0, int health = 2, bool itemDrop = false) :
			enemyType(typeID), isItemDrop(itemDrop), hp(health) {} 

		virtual ~Toroid() = default;
		Toroid(const Toroid&) = delete;
		const Toroid& operator=(const Toroid&) = delete;
	
		/**
		*	敵の初期化処理
		*/
		void Initialize() override;

		/**
		*	敵の更新処理
		*
		*	@param deltaTime	経過時間
		*/
		void Update(float deltaTime) override;

		/**
		*	ダメージ処理
		*
		*	@param p	ダメージ量
		*/
		void Damage(float p) override;

		/**
		*	衝突判定処理
		*
		*	@param e	衝突してきたエンティtティ
		*/
		void CollisionEnter(Entity::Entity& e) override;

		/**
		*	移動用コントローラの設定
		*
		*	@param m	移動用コントローラ
		*/
		void MoveController(MoveControllerPtr m) { moveController = m; }

		/**
		*	移動用コントローラーの取得
		*	
		*	@return 移動用コントローラー
		*/
		MoveControllerPtr MoveController() const { return moveController; }

		/**
		*	弾の生成器を設定
		*
		*	@param b	弾の生成器
		*/
		void BulletGenerator(BulletGeneratorPtr b) { bulletGenerator = b; }

		/**
		*	弾の生成器を取得
		*
		*	@return 弾の生成器
		*/
		BulletGeneratorPtr BulletGenerator() const { return bulletGenerator; }
		void DestroyByScreenOut(bool v) { isDestroyByScreenOut = v; }

		/**
		*	ターゲットの設定
		*
		*	@param t	ターゲットとなるエンティティ
		*/
		void Target(Entity::Entity* t);

		/**
		*	弾の色を設定する
		*/
		void BulletColor(glm::vec4 c) { bulletColor = c; }

		/**
		*	設定しているエンティティの取得
		*
		*	@return 試用しているエンティティ
		*/
		Entity::Entity* Entity() { return entity; }

	private:

		float timer = 0;	/// 
		int enemyType = 0;	/// 移動タイプ
		int hp = 2;			/// 体力

		bool isItemDrop = false;
		bool isDestroyByScreenOut = true;
		glm::vec4 bulletColor = glm::vec4(1);

		BulletGeneratorPtr bulletGenerator;	///< 弾の生成管理クラス
		MoveControllerPtr moveController;	///< 移動管理クラス
		
		Entity::Entity* playerEntity;
	};
	
	//ボス用敵
	class BossEnemy : public Entity::EntityDataBase {
	public:

		/**
		*	初期化処理
		*/
		void Initialize() override;

		/**
		*	更新処理
		*
		*	@param deltaTime	経過時間
		*/
		void Update(float deltaTime) override;

		/**
		*	ダメージ処理
		*
		*	@param p	ダメージ量
		*/
		void Damage(float) override;

		/**
		*	衝突判定処理
		*
		*	@param e	衝突したエンティティ
		*/
		void CollisionEnter(Entity::Entity& e) override;

		/**
		*	タレットの更新処理
		*/
		void UpdateTurret();

		/**
		*	移動用コントローラの設定
		*
		*	@param m	設定するコントローラー
		*/
		void MoveController(MoveControllerPtr m) { moveController = m; }

		/**
		*	ターゲットの設定
		*
		*	@param t	ターゲットのエンティティ
		*/
		void Target(Entity::Entity* t);

	private:

		float timer = 0;	/// 
		int hp = 0;		/// 体力
		const int maxHp = 1500;
		SpritePtr hpGuage;

		MoveControllerPtr moveController;
		std::vector<std::shared_ptr<Toroid> > turrets;

		Entity::Entity* playerEntity;
	};

}