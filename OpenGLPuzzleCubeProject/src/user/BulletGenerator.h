/**
*	@file BulletGenerator.h
*	@brief	敵の弾生成システムの制御用
*	@author	Takuya Yokoyama
*/
#pragma once
#include "../Engine/Entity.h"


namespace Application {

	///弾の生成システムのベースクラス
	class BulletGenerator {
	public:

		BulletGenerator() = default;

		/**
		*	コンストラクタ
		*
		*	@param parent	取り付けるエンティティ
		*	@param groupId	適用するグループID
		*	@param target	ターゲットのエンティティ
		*/
		BulletGenerator(Entity::Entity& parent,int groupId, Entity::Entity* target = nullptr);	

		virtual ~BulletGenerator() = default;
		const BulletGenerator& operator=(const BulletGenerator&) = delete;

		/**
		*	更新処理
		*
		*	@param deltaTime	経過時間
		*/
		virtual void Update(float deltaTime) = 0;

		/**
		*	弾の発射処理
		*/
		virtual void ShotBullet();

		/**
		*	移動量の計算処理
		*/
		virtual void CalcVelocity();

		/**
		*	ターゲットの設定
		*
		*	@param t	ターゲットとして設定するエンティティ
		*/
		void Target(Entity::Entity* t) { target = t; CalcVelocity(); }

		/**
		*	グループIDの設定
		*/
		void GroupId(int i) { groupId = i; }

		/**
		*	発射間隔の設定
		*
		*	@param i	適用する発射間隔
		*/
		void ShotInterval(float i) { shotInterval = i; }

		/**
		*	発射間隔情報の取得
		*
		*	@return 発射間隔
		*/
		float ShotInterval() const { return shotInterval; }

		/**
		*	弾速の設定
		*	
		*	@param s	適用する弾速
		*/
		void BulletSpeed(float s) { initBulletSpeed = s; }

		/**
		*	弾速の取得
		*	
		*	@return 弾速
		*/
		float BulletSpeed() const { return initBulletSpeed; }

		/**
		*	弾の色の設定
		*
		*	@param c	適用する色
		*/
		void Color(glm::vec4 c) { color = c; }

		/**
		*	弾の回転情報を設定する
		*
		*	@param r	回転情報
		*/
		void Rotation(glm::quat r) { direction = r * glm::vec3(-1, 0, 0); }

	protected:

		float timer;					///< 発射間隔制御用タイマー
		float shotInterval = 2;			///< 弾の発射間隔
		float initBulletSpeed = 1;		///< 弾の速度
		int groupId = -1;				///< 弾のID
		glm::vec4 color = glm::vec4(0.3, 0.3, 1, 1);	///< 発射する弾の色
		glm::vec3 direction;			///< 弾を発射させる向き
		glm::quat rotation;				///< 弾を発射させる角度

		Entity::Entity& parent;		/// 弾の発射元
		Entity::Entity* target;		/// 弾の追尾対象
	};

	using BulletGeneratorPtr = std::shared_ptr<BulletGenerator>;

	///通常の弾発射
	class NormalShot : public BulletGenerator {
	public:

		NormalShot() = default;

		/**
		*	コンストラクタ
		*
		*	@param parent	取り付けるエンティティ
		*	@param interval	発射間隔
		*	@param target	ターゲットのエンティティ
		*/
		NormalShot(Entity::Entity& p, int i, Entity::Entity* t = nullptr);
		NormalShot(const NormalShot&) = delete;
		void operator=(const NormalShot&) = delete;

		/**
		*	更新処理
		*
		*	@param deltaTime	経過時間
		*/
		void Update(float deltaTime) override;
	};

	///複数の方向に弾を発射する
	class MultiWayShot : public BulletGenerator {
	public:

		MultiWayShot() = default;

		/**
		*	コンストラクタ
		*	
		*	@param p	取り付けるエンティティ
		*	@param i	発射間隔
		*	@param t	ターゲットのエンティティ
		*	@param a	弾同士の間隔
		*	@param m	同時発射弾数
		*/
		MultiWayShot(Entity::Entity& parent,int i, Entity::Entity* t = nullptr, float a = 15.f,int m = 3);
		MultiWayShot(const MultiWayShot&) = delete;
		void operator=(const MultiWayShot&) = delete;

		/**
		*	更新処理
		*
		*	@param deltaTime	更新処理
		*/
		void Update(float deltaTime) override;

		/**
		*	中心位置の設定処理
		*
		*	@param v	中心の方向ベクトル
		*/
		void CenterDirectiton(glm::vec3 v);

		/**
		*	最大弾数の設定
		*
		*	@param m	適用する最大弾数
		*/
		void MaxBulletNum(int m) { maxBulletNum = m; }

		/**
		*	角度間隔の設定
		*
		*	@param a	適用する角度間隔 
		*/
		void AngleInterval(float r) { angleInterval = r; }

	private:

		float angleInterval;		/// 弾と弾の間隔()
		int maxBulletNum;			/// 同時発射できる最大弾数
		bool isUseParentRotation;	/// 親の回転に影響を受けるか
	};

	///回転しながら連続で弾を発射する
	class CircleShot : public BulletGenerator {
	public:

		CircleShot() = default;


		CircleShot(Entity::Entity& parent , int interval, float a = 15.f, float shotAngle = 0.f);
		~CircleShot() = default;
		const CircleShot& operator=(const CircleShot&) = delete;;

		/**
		*	更新処理
		*
		*	@param deltaTime	経過時間
		*/
		void Update(float delta) override;

	private:

		float angleInterval;	/// 弾同士の角度の間隔
		float shotAngle;		

	};

}
