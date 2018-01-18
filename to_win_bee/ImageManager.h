#pragma once
#include "MyPG.h"

//画像の分割管理とアニメーション
class ImageManager
{
public:

	vector<ML::Box2D*>	charaChip;		//キャラクタの素材
	int					defImageNum;	//差分用画像番号
	int					baseImageNum;	//基準画像番号
	POINT				drawPos;		//描画の基準座標
	int					imageWidth;		//画像の横幅
	int					imageHeight;	//画像の縦幅
	float				animCnt;		//アニメーションカウンタ
	bool				animTurn;		//アニメーションが反転しているか否か

	ImageManager() :
		defImageNum(0),
		baseImageNum(0),
		imageWidth(0),
		imageHeight(0),
		animCnt(0),
		animTurn(false)
	{
		drawPos.x = 0;
		drawPos.y = 0;
	};

	~ImageManager() {};

	//--------------------------------------------------------
	//以下使用メソッド

	//------------------------------------
	//画像の分割
	//引数：開始位置XY(int, int), 画像の分割数WH(int, int), 画像サイズ(int, int デフォルトで32x32)
	void ImageCreate(int startX, int startY, int width, int height, int imageWidth = 32, int imageHeight = 32);

	//------------------------------------
	//画像の分割(1つだけ)
	//引数：開始位置XY(int, int), 終了位置(int, int)
	void ImageOneCreate(int startX, int startY, int endX, int endY);

	//------------------------------------
	//画像の分割解除
	void ImageErase();

	//------------------------------------
	//画像の描画
	//引数：表示座標(ML::Vec2), 画像名(string), 表示色(ML::Color デフォルトで(1, 1, 1, 1))
	void ImageRender(const ML::Vec2& pos, const string& imageName, const ML::Color& color = { 1, 1, 1, 1 });
};