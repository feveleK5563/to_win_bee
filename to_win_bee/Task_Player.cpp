//-------------------------------------------------------------------
//プレイヤー
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Shot.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		imageName = "Player";
		DG::Image_Create(imageName, "./data/image/Player.png");
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
		render2D_Priority[1] = 0.5f;

		state = State1;	//State1 = 通常のショット
						//State2 = 2発同時
						//State3 = 3発同時
		baseSpeed = 3.f;
		pos = { float(ge->screen2DWidth) / 2, float(ge->screen2DHeight) / 3 * 2 };
		hitBase = { -16, -16, 32, 32 };

		image.ImageCreate(0, 0, 1, 1);
		image.baseImageNum = 0;
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
		if (state != Death)
		{
			in = DI::GPad_GetState("P1");

			MovePlayer();
			CreateShot();

			//(仮)弾の種類変更
			if (in.B2.down)
			{
				if (state == State1)
					state = State2;
				else if (state == State2)
					state = State3;
				else if (state == State3)
					state = State1;
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		image.ImageRender(pos, res->imageName);
	}

	//-------------------------------------------------------------------
	//キー入力の処理
	float Object::ControllKeyTable()
	{
		float keyDirection[3][3] = {
			{ 225, 270, 315},
			{ 180,  -1,   0},
			{ 135,  90,  45},
		};
		int kdX = 1, kdY = 1;

		if (in.LStick.L.on) { --kdX; }
		if (in.LStick.R.on) { ++kdX; }
		if (in.LStick.U.on) { --kdY; }
		if (in.LStick.D.on) { ++kdY; }

		return keyDirection[kdY][kdX];
	}
	//-------------------------------------------------------------------
	//プレイヤーの動作処理
	void Object::MovePlayer()
	{
		speed = { 0, 0 };
		float angle = ControllKeyTable();
		if (angle != -1)
		{
			speed.x = cos(ML::ToRadian(angle)) * baseSpeed;
			speed.y = sin(ML::ToRadian(angle)) * baseSpeed;
		}
		pos += speed;

		if (pos.x < float(image.drawPos.x))
			pos.x = float(image.drawPos.x);
		if (pos.x > float(ge->screen2DWidth) - image.drawPos.x)
			pos.x = float(ge->screen2DWidth) - image.drawPos.x;
		if (pos.y < float(image.drawPos.y))
			pos.y = float(image.drawPos.y);
		if (pos.y > float(ge->screen2DHeight) - image.drawPos.y)
			pos.y = float(ge->screen2DHeight) - image.drawPos.y;
	}

	//-------------------------------------------------------------------
	//弾の生成
	void Object::CreateShot()
	{
		if (in.B1.on)
		{
			if (!((cntTime++) % 10))
			{
				switch (state)
				{
				case BChara::State1: //通常ショット
				{
					auto shot = Shot::Object::Create(true);
					shot->pos = pos;
					shot->pos.y -= 8.f;
					shot->speed = { 0.f, -15.f };
				}
					break;

				case BChara::State2: //2発同時ショット
				{
					for (int i = 0; i < 2; ++i)
					{
						auto shot = Shot::Object::Create(true);
						shot->pos = pos;
						shot->pos.y -= 8.f;
						shot->pos.x += doubleShot[i];
						shot->speed = { 0.f, -15.f };
						shot->image.baseImageNum = 1;
					}
				}
					break;

				case BChara::State3: //3発同時ショット
				{
					for (int i = 0; i < 3; ++i)
					{
						auto shot = Shot::Object::Create(true);
						shot->pos = pos;
						shot->pos.y -= 8.f;
						shot->speed = threeWayShot[i];
					}
				}
					break;
				}
			}
		}
		else
		{
			cntTime = 0;
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