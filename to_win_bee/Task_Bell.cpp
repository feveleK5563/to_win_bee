//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Bell.h"
#include  "Task_Player.h"

namespace  Bell
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		imageName = "Bell";
		DG::Image_Create(imageName, "./data/image/Bell.png");
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
		render2D_Priority[1] = 0.75f;
		bellType = Yellow;
		damage = 0;
		hitBase = { -16, -16, 32, 32 };
		speed = { 0.f, -7.f };

		image.ImageCreate(0, 0, 3, 4);
		image.drawPos = { 16, 16 };

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
		if (speed.y > 0.f)
			speed.y += acclation / 3.f;
		else
			speed.y += acclation;
		pos += speed;
		ScreenOutObj();

		++cntTime;
		if (bellType == Flash)
		{
			image.baseImageNum = flashAnim[(cntTime / 2) % 2];
		}
		image.animCnt = float((cntTime / 15) % 3);

		if (HitPlayer(false))
		{
			EffectGrant();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		image.ImageRender(pos, res->imageName);
	}

	//-------------------------------------------------------------------
	//ベルのタイプ変更(弾ヒット処理)
	void Object::ChangeType()
	{
		++damage;
		if (speed.y > 0.f)
			speed.y += -4.f;

		if (!(damage % 7))
		{
			if (bellType == Flash)
			{
				Kill();
			}
			else
			{
				++bellType;
				if (bellType != Flash)
					image.baseImageNum += 3;
			}
		}
	}

	//-------------------------------------------------------------------
	//プレイヤーに効果付与
	void Object::EffectGrant()
	{
		auto player = ge->GetTask_One_GN<Player::Object>("本編", "プレイヤー");
		if (player == nullptr)
			return;

		switch (bellType)
		{
		case Yellow:	//スコアボーナス
			break;

		case Blue:		//スピードアップ
			player->baseSpeed += 0.5f;
			break;

		case White:		//弾種を変更
			player->ChangeShot();
			break;

		case Red:		//バリア生成
			break;

		case Flash:		//分身
			break;
		}
		Kill();
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