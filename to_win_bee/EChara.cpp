//-------------------------------------------------------------------
//�G�ėp�X�[�p�[�N���X
//-------------------------------------------------------------------
#include "EChara.h"
#include "MyPG.h"
#include "Task_Player.h"

//-------------------------------------------------------------------
//�v���C���[�Ƃ̓����蔻��
void EChara::HitPlayer()
{
	auto player = ge->GetTask_One_GN<Player::Object>("�{��", "�v���C���[");
	if (player == nullptr)
		return;

	ML::Box2D eh = hitBase.OffsetCopy(pos);
	ML::Box2D ph = player->hitBase.OffsetCopy(player->pos);

	if (eh.Hit(ph))
	{
		//player->state = Death;
		//player->cntTime = 0;
		player->Kill(); //��
	}
}