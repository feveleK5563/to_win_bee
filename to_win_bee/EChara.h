#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//敵汎用クラス
//-------------------------------------------------------------------
#include "BChara.h"

class EChara : public BChara
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	EChara(){};
	virtual ~EChara(){};

	void HitPlayer();
};