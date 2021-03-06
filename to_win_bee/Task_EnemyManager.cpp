//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Cloud.h"
#include  "Task_EnemyManager.h"
#include  "Task_Enemy_Itigo.h"
#include  "Task_Enemy_Diecon.h"

namespace  EnemyManager
{
	std::istream& operator >>(
		std::istream& i,
		EnemyManager::Object::AppEnemyData& data)
	{
		i >> data.appPoint >>
			data.pos.x >>
			data.pos.y >>
			data.type;
		return i;
	}

	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
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
		monsterNum = 0;
		cntTime = 0;
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("敵");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		++cntTime;
		if (!(cntTime % 180))
		{
			AppMonster();
		}
		if (!(cntTime % 210))
		{
			AppCloud();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}

	//-------------------------------------------------------------------
	//敵の読み込み
	void Object::LoadEnemyTable(int stageNum)
	{

	}

	//-------------------------------------------------------------------
	//読み込んだ敵データの解放
	void Object::EraseEnemyTable()
	{
		int size = aed.size();
		for (int i = 0; i < size; ++i)
			delete aed[i];
		aed.clear();
		aed.shrink_to_fit();
	}

	//-------------------------------------------------------------------
	//雲の出現
	void Object::AppCloud()
	{
		auto cloud = Cloud::Object::Create(true);
		cloud->pos = { float(32 + rnd() % int(ge->screen2DWidth - 32)), -32.f};
		cloud->speed = { 0.f, 1.f };
	}

	//-------------------------------------------------------------------
	//モンスターの出現
	void Object::AppMonster()
	{
		switch (rnd() % 2)
		{
		case 0: //イティゴ
			AppMonster_Itigo();
			break;

		case 1:	//大根
			AppMonster_Diecon();
			break;
		}
	}

	//-------------------------------------------------------------------
	//イティゴ
	void Object::AppMonster_Itigo()
	{
		ML::Vec2 basePos = { float(16 + rnd() % int(ge->screen2DWidth - 16)), -32.f };
		ML::Vec2 baseSpd = { 1.f, 2.f };
		if (basePos.x > float(ge->screen2DWidth / 2))
			baseSpd.x *= -1.f;

		for (int i = 4; i >= 0; --i)
		{
			auto en = Itigo::Object::Create(true);
			en->pos = { basePos.x - (baseSpd.x * 26.f) * i,
						basePos.y - (baseSpd.y * 14.f) * i };
			en->speed = { baseSpd.x, baseSpd.y };
		}
	}

	//-------------------------------------------------------------------
	//大根
	void Object::AppMonster_Diecon()
	{
		ML::Vec2 basePos = { float(16 + rnd() % int(ge->screen2DWidth - 16)), -32.f };
		ML::Vec2 baseSpd = { 2.f, 2.f };
		if (basePos.x > float(ge->screen2DWidth / 2))
			baseSpd.x *= -1.f;

		int num = rnd() % 2 + 2;
		for (int i = num; i >= 0; --i)
		{
			auto en = Diecon::Object::Create(true);
			en->pos = { basePos.x - (baseSpd.x * 16.f) * i,
						basePos.y - (baseSpd.y * 14.f) * i };
			en->speed = { baseSpd.x, baseSpd.y };
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