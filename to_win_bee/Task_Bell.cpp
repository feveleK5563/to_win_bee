//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Bell.h"
#include  "Task_Player.h"
#include  "Task_Barrier.h"

namespace  Bell
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[Xฬ๚ป
	bool  Resource::Initialize()
	{
		imageName = "Bell";
		DG::Image_Create(imageName, "./data/image/Bell.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[Xฬ๐๚
	bool  Resource::Finalize()
	{
		DG::Image_Erase(imageName);
		return true;
	}
	//-------------------------------------------------------------------
	//u๚ปv^XNถฌษP๑พฏsค
	bool  Object::Initialize()
	{
		//X[p[NX๚ป
		__super::Initialize(defGroupName, defName, true);
		//\[XNXถฌor\[XคL
		this->res = Resource::Create();

		//f[^๚ป
		render2D_Priority[1] = 0.75f;
		bellType = Yellow;
		damage = 0;
		hitBase = { -16, -16, 32, 32 };
		speed = { 0.f, -6.f };

		image.ImageCreate(0, 0, 3, 4);
		image.drawPos = { 16, 16 };

		//^XNฬถฌ

		return  true;
	}
	//-------------------------------------------------------------------
	//uIนv^XNมลษP๑พฏsค
	bool  Object::Finalize()
	{
		//f[^^XN๐๚
		image.ImageErase();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//๘ซpฌ^XNฬถฌ
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[ษsค
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
	//uQc`ๆvPt[ษsค
	void  Object::Render2D_AF()
	{
		image.ImageRender(pos, res->imageName);
	}

	//-------------------------------------------------------------------
	//xฬ^CvฯX(eqbg)
	void Object::ChangeType()
	{
		++damage;
		if (speed.y > 0.f)
			speed.y = -4.f;

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
	//vC[ษ๘สt^
	void Object::EffectGrant()
	{
		auto player = ge->GetTask_One_GN<Player::Object>("{า", "vC[");
		if (player == nullptr)
			return;

		switch (bellType)
		{
		case Yellow:	//XRA{[iX
			break;

		case Blue:		//Xs[hAbv
			player->baseSpeed += 0.5f;
			break;

		case White:		//eํ๐ฯX
			player->ChangeShot();
			break;

		case Red:		//oAถฌ
			if (auto barrier = ge->GetTask_One_GN<Barrier::Object>("{า", "oA"))
			{
				barrier->life.SetLife(10);
			}
			else
			{
				auto newBarrier = Barrier::Object::Create(true);
			}
			break;

		case Flash:		//ชg
			break;
		}
		Kill();
	}

	//
	//ศบอ๎{IษฯXsvศ\bh
	//
	//-------------------------------------------------------------------
	//^XNถฌ๛
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWษo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYษธsตฝ็Kill
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
	//\[XNXฬถฌ
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