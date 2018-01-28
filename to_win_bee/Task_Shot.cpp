//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Shot.h"
#include  "Task_Player.h"
#include  "Task_Cloud.h"
#include  "Task_Bell.h"
#include  "Task_Enemy_Itigo.h"
#include  "Task_Enemy_Diecon.h"
#include  "Task_DeathEnemy.h"

namespace  Shot
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		imageName = "Shot";
		DG::Image_Create(imageName, "./data/image/Shot.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(imageName);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		render2D_Priority[1] = 0.7f;

		beforePos = { 0, 0 };
		hitBase = { -8, -8, 16, 16 };
		shotUser = Player; //(この後別の値に変更されたりされなかったり)

		image.ImageCreate(0, 0, 2, 1, 8, 8);
		image.drawPos = { 4, 4 };

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		image.ImageErase();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		beforePos = pos;
		switch (shotUser)
		{
		case Player:
			HitEnemy();
			HitCloudOrBell();
			break;

		case Enemy:
			HitPlayer();
			break;
		}
		pos += speed;
		ScreenOutObj();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		image.ImageRender(pos, res->imageName);
	}

	//-------------------------------------------------------------------
	//敵との当たり判定
	void Object::HitEnemy()
	{
		{	//イティゴ
			auto enemy = ge->GetTask_Group_GN<Itigo::Object>("敵", "イティゴ");
			for (auto it = enemy->begin(); it != enemy->end(); ++it)
			{
				if (hitBase.OffsetCopy(pos).Hit((*it)->hitBase.OffsetCopy((*it)->pos)))
				{
					auto de = DeathEnemy::Object::Create(true);
					de->pos = (*it)->pos;
					(*it)->Kill();
					Kill();
					return;
				}
			}
		}
		{	//大根
			auto enemy = ge->GetTask_Group_GN<Diecon::Object>("敵", "大根");
			for (auto it = enemy->begin(); it != enemy->end(); ++it)
			{
				if (hitBase.OffsetCopy(pos).Hit((*it)->hitBase.OffsetCopy((*it)->pos)))
				{
					auto de = DeathEnemy::Object::Create(true);
					de->pos = (*it)->pos;
					(*it)->Kill();
					Kill();
					return;
				}
			}
		}
	}

	//-------------------------------------------------------------------
	//雲、ベルとの当たり判定
	void Object::HitCloudOrBell()
	{
		{	//雲
			auto cloud = ge->GetTask_Group_GN<Cloud::Object>("敵", "雲");
			for (auto it = cloud->begin(); it != cloud->end(); ++it)
			{
				if ((*it)->hp <= 0)
					continue;

				if (hitBase.OffsetCopy(pos).Hit((*it)->hitBase.OffsetCopy((*it)->pos)))
				{
					--(*it)->hp;
					return;
				}
			}
		}
		{	//ベル
			auto bell = ge->GetTask_Group_GN<Bell::Object>("敵", "ベル");
			for (auto it = bell->begin(); it != bell->end(); ++it)
			{
				if (hitBase.OffsetCopy(pos).Hit((*it)->hitBase.OffsetCopy((*it)->pos)))
				{
					(*it)->ChangeType();
					Kill();
					return;
				}
			}
		}
	}

	//-------------------------------------------------------------------
	//プレイヤーとの当たり判定
	void Object::HitPlayer()
	{
		auto player = ge->GetTask_One_GN<Player::Object>("本編", "プレイヤー");
		if (player == nullptr)
			return;

		if (hitBase.OffsetCopy(pos).Hit(player->hitBase.OffsetCopy(player->pos)))
		{
			player->Kill();
			Kill();
		}
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}