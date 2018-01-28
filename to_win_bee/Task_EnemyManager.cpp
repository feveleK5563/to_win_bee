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
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		this->res = Resource::Create();

		//f[^ϊ»
		rnd.seed(0);

		monsterNum = 0;
		cntTime = 0;
		
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ
		ge->KillAll_G("G");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
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
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
	}

	//-------------------------------------------------------------------
	//_Μo»
	void Object::AppCloud()
	{
		if (rnd() % 2)
			return;

		auto cloud = Cloud::Object::Create(true);
		cloud->pos = { float(32 + rnd() % int(ge->screen2DWidth - 32)), -32.f};
		cloud->speed = { 0.f, 1.f };
	}

	//-------------------------------------------------------------------
	//X^[Μo»
	void Object::AppMonster()
	{
		switch (rnd() % 2)
		{
		case 0: //CeBS
			AppMonster_Itigo();
			break;

		case 1:	//εͺ
			AppMonster_Diecon();
			break;
		}
	}

	//-------------------------------------------------------------------
	//CeBS
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
	//εͺ
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

	//
	//ΘΊΝξ{IΙΟXsvΘ\bh
	//
	//-------------------------------------------------------------------
	//^XNΆ¬ϋ
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWΙo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYΙΈs΅½ηKill
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
	//\[XNXΜΆ¬
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