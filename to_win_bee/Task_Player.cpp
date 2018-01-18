//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Shot.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XÌú»
	bool  Resource::Initialize()
	{
		imageName = "Player";
		DG::Image_Create(imageName, "./data/image/Player.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XÌðú
	bool  Resource::Finalize()
	{
		DG::Image_Erase(imageName);
		return true;
	}
	//-------------------------------------------------------------------
	//uú»v^XN¶¬ÉPñ¾¯s¤
	bool  Object::Initialize()
	{
		//X[p[NXú»
		__super::Initialize(defGroupName, defName, true);
		//\[XNX¶¬or\[X¤L
		this->res = Resource::Create();

		//f[^ú»
		render2D_Priority[1] = 0.5f;

		baseSpeed = 2.f;
		pos = { float(ge->screen2DWidth) / 2, float(ge->screen2DHeight) / 2 };

		image.ImageCreate(0, 0, 1, 1);
		image.baseImageNum = 0;
		image.drawPos = { 16, 16 };
		
		//^XNÌ¶¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uI¹v^XNÁÅÉPñ¾¯s¤
	bool  Object::Finalize()
	{
		//f[^^XNðú
		image.ImageErase();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ø«p¬^XNÌ¶¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[És¤
	void  Object::UpDate()
	{
		in = DI::GPad_GetState("P1");
		MovePlayer();
		CreateShot();
	}
	//-------------------------------------------------------------------
	//uQc`ævPt[És¤
	void  Object::Render2D_AF()
	{
		image.ImageRender(pos, res->imageName);
	}

	//-------------------------------------------------------------------
	//L[üÍÌ
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
	//vC[Ì®ì
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
	//eÌ¶¬
	void Object::CreateShot()
	{
		if (in.B1.on)
		{
			if (!((cntTime++) % 9))
			{
				auto shot = Shot::Object::Create(true);
				shot->pos = pos;
				shot->pos.y -= 16.f;
				shot->speed = { 0.f, -12.f };
			}
		}
		else
		{
			cntTime = 0;
		}
	}

	//
	//ÈºÍî{IÉÏXsvÈ\bh
	//
	//-------------------------------------------------------------------
	//^XN¶¬û
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWÉo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYÉ¸sµ½çKill
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
	//\[XNXÌ¶¬
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