#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Player
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("本編");	//グループ名
	const  string  defName("プレイヤー");		//タスク名
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//共有する変数はここに追加する
		string imageName;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:

		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//追加したい変数・メソッドはここに追加する
		//BCharaに含まれないモノのみここに追加する

		DI::VGamePad in;	//コントローラ
		const float doubleShot[2] = { -4.f, 4.f };
		const ML::Vec2 threeWayShot[3] = {
			{ -7.f, -15.f },
			{  0.f, -15.f },
			{  7.f, -15.f },
		};
		float baseSpeed;	//基本スピード
		bool  noStart;		//画面下からブワーッと出てき終わったらfalseになる

		void  PlayerStart();			//画面下からブワーッと出てくる処理
		float ControllKeyTable();	//移動系キー入力の処理
		void  MovePlayer();			//プレイヤーの動作処理
		void  ChangeShot();			//弾種を変える
		void  CreateShot();			//弾の出現
	};
}