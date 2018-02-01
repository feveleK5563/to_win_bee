//-------------------------------------------------------------------
//�G�ėp�X�[�p�[�N���X
//-------------------------------------------------------------------
#include "EChara.h"
#include "MyPG.h"
#include "Task_Player.h"
#include "Task_Barrier.h"

//-------------------------------------------------------------------
//�v���C���[�Ƃ̓����蔻��
//�����F�v���C���[�����ʂ��ۂ�(bool)
//�߂�l�F�v���C���[�ɓ����������ۂ�
bool EChara::HitPlayer(bool killPlayer)
{
	auto player = ge->GetTask_One_GN<Player::Object>("�{��", "�v���C���[");
	if (player == nullptr)
		return false;

	ML::Box2D eh = hitBase.OffsetCopy(pos);
	ML::Box2D ph = player->hitBase.OffsetCopy(player->pos);

	if (eh.Hit(ph))
	{
		if (killPlayer)
		{
			if (auto barrier = ge->GetTask_One_GN<Barrier::Object>("�{��", "�o���A"))
				barrier->Kill();

			player->life.MinusLife(1);
			if (player->life.GetLife() <= 0)
			{
				player->Kill();
			}

			player->PlayerInitialize();
		}
		return true;
	}
	return false;
}