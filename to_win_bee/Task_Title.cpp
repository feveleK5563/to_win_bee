//-------------------------------------------------------------------
// ^CgζΚ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_Game.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		imageName[0] = "TitleImg";
		DG::Image_Create(imageName[0], "./data/image/Title.png");
		imageName[1] = "CompanyImg";
		DG::Image_Create(imageName[1], "./data/image/Konmai.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		for (int i = 0; i < 2; ++i)
			DG::Image_Erase(imageName[i]);
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
		cntTime = 0;
		diflectionWidth = 100.f;
		moveOk = false;

		image.ImageCreate(0, 0, 1, 60, 220, 1);
		image.drawPos = { 110, 0 };
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ
		image.ImageErase();
		ge->KillAll_G("^Cg");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
			auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");

		if (diflectionWidth > 0)
		{
			diflectionWidth -= 1.f;
			cntTime += 8;

			if (in.ST.down)
			{
				diflectionWidth = 0.f;
			}
		}
		else
		{
			moveOk = true;
			diflectionWidth = 0.f;

			if (in.ST.down)
			{
				Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{	
		int rNum = image.RectangleSize();
		for (int i = 0; i < rNum; ++i, ++image.baseImageNum)
		{
			ML::Vec2 pos = { float(ge->screen2DWidth) / 2.f, (float(ge->screen2DHeight - 60) / 3.f) + i };
			if (i % 2)
				pos.x += cos(ML::ToRadian(float(cntTime + (i * 5)))) * diflectionWidth;
			else
				pos.x -= cos(ML::ToRadian(float(cntTime + (i * 5)))) * diflectionWidth;
			image.ImageRender(pos, res->imageName[0]);
		}
		if (moveOk)
		{
			ML::Vec2 pos = { float(ge->screen2DWidth) / 2.f, 180.f};
			ML::Box2D draw(-53, 0, 106, 10);
			draw.Offset(pos);
			ML::Box2D src(0, 0, 106, 10);
			DG::Image_Draw(res->imageName[1], draw, src);
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