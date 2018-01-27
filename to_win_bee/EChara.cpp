//-------------------------------------------------------------------
//敵汎用スーパークラス
//-------------------------------------------------------------------
#include "EChara.h"
#include "MyPG.h"
#include "Task_Player.h"

//-------------------------------------------------------------------
//プレイヤーとの当たり判定
//引数：プレイヤーが死ぬか否か(bool)
//戻り値：プレイヤーに当たったか否か
bool EChara::HitPlayer(bool killPlayer)
{
	auto player = ge->GetTask_One_GN<Player::Object>("本編", "プレイヤー");
	if (player == nullptr)
		return false;

	ML::Box2D eh = hitBase.OffsetCopy(pos);
	ML::Box2D ph = player->hitBase.OffsetCopy(player->pos);

	if (eh.Hit(ph))
	{
		if (killPlayer)
		{
			//player->state = Death;
			//player->cntTime = 0;
			player->Kill(); //仮
		}
		return true;
	}
	return false;
}