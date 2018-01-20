//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Shot.h"
#include  "Task_Player.h"

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
		render2D_Priority[1] = 0.8f;

		beforePos = { 0, 0 };
		shotUser = Player; //暫定

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
			break;

		case Enemy:
			HitPlayer();
			break;
		}
		pos += speed;
		ScreenOutShot();
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

	}

	//-------------------------------------------------------------------
	//プレイヤーとの当たり判定
	void Object::HitPlayer()
	{
		auto player = ge->GetTask_One_GN<Player::Object>("本編", "プレイヤー");
		if (player == nullptr)
			return;

		if (BoxAndLineHit(player->hitBase.OffsetCopy(player->pos)))
			player->Kill();
	}

	//-------------------------------------------------------------------
	//線分と矩形の当たり判定
	//弾の前座標と現座標との線分と矩形の辺4つとの通過判定を行う
	bool Object::BoxAndLineHit(const ML::Box2D& plyHb)
	{
		POINT posA, posB;
		for (int i = 0; i < 4; ++i)
		{
			switch (i)
			{
			case 0:	//左辺
				posA = { plyHb.x, plyHb.y };
				posB = { plyHb.x, plyHb.y + plyHb.h };
				break;

			case 1:	//右辺
				posA = { plyHb.x + plyHb.w, plyHb.y };
				posB = { plyHb.x + plyHb.w, plyHb.y + plyHb.h };
				break;

			case 2:	//上辺
				posA = { plyHb.x, plyHb.y };
				posB = { plyHb.x + plyHb.w, plyHb.y };
				break;

			case 3:	//下辺
				posA = { plyHb.x, plyHb.y + plyHb.h };
				posB = { plyHb.x + plyHb.w, plyHb.y + plyHb.h };
				break;
			}
			int tA = (posA.x - posB.x) * (pos.y - posA.y) + (posA.x - posB.x) * (posA.y - pos.y);
			int tB = (posB.x - posA.x) * (beforePos.y - posA.y) - (beforePos.x - posA.x) * (posB.y - posA.y);
			int tC = (pos.x - beforePos.x) * (posA.y - pos.y) + (pos.x - beforePos.x) * (pos.y - posA.y);
			int tD = (pos.x - beforePos.x) * (posB.y - pos.y) + (pos.x - beforePos.x) * (pos.y - posB.y);
			if (tA * tB <= 0 && tC * tD <= 0)
				return true;
		}
		return false;
	}

	//-------------------------------------------------------------------
	//画面外に出たら消す処理
	void Object::ScreenOutShot()
	{
		if (pos.x < -16.f || float(ge->screen2DWidth) + 16.f < pos.x ||
			pos.y < -16.f || float(ge->screen2DHeight) + 16.f < pos.y)
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