//-------------------------------------------------------------------
//敵汎用スーパークラス
//-------------------------------------------------------------------
#include "EChara.h"
#include "MyPG.h"
#include "Task_Player.h"

//-------------------------------------------------------------------
//プレイヤーとの当たり判定
void EChara::HitPlayer()
{
	auto player = ge->GetTask_One_GN<Player::Object>("本編", "プレイヤー");
	if (player == nullptr)
		return;

	ML::Box2D eh = hitBase.OffsetCopy(pos);
	ML::Box2D ph = player->hitBase.OffsetCopy(player->pos);

	if (eh.Hit(ph))
	{
		//player->state = Death;
		//player->cntTime = 0;
		player->Kill(); //仮
	}
}