//-------------------------------------------------------------------
// タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_Game.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		imageName[0] = "TitleImg";
		DG::Image_Create(imageName[0], "./data/image/Title.png");
		imageName[1] = "CompanyImg";
		DG::Image_Create(imageName[1], "./data/image/Konmai.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		for (int i = 0; i < 2; ++i)
			DG::Image_Erase(imageName[i]);
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
		cntTime = 0;
		diflectionWidth = 100.f;
		moveOk = false;

		image.ImageCreate(0, 0, 1, 60, 220, 1);
		image.drawPos = { 110, 0 };
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		image.ImageErase();
		ge->KillAll_G("タイトル");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");

		if (diflectionWidth > 0)
		{
			diflectionWidth -= 0.8f;
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
	//「２Ｄ描画」１フレーム毎に行う処理
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