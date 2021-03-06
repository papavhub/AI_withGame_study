#include "Goal_RunAway.h"
/**/
#include "Raven_Bot.h"
#include "navigation/Raven_PathPlanner.h"
#include "Raven_Game.h"
#include "Raven_Map.h"
#include "Messaging/Telegram.h"
#include "Raven_Messages.h"
#include "goals/Goal_MoveToPosition.h"

#include "goals/Goal_SeekToPosition.h"
#include "goals/Goal_FollowPath.h"


//------------------------------ Activate -------------------------------------
//-----------------------------------------------------------------------------
void Goal_RunAway::Activate()
{
	// 랜덤 위치로 이동
	// 단, 적에게서 안전한 곳으로...
	m_iStatus = active;

	RemoveAllSubgoals();

	Raven_Bot* target_enemy = m_pOwner->GetTargetSys()->GetTarget();
	Vector2D location;
	bool enemy_can_see = TRUE;

	while (enemy_can_see)
	{
		location = m_pOwner->GetWorld()->GetMap()->GetRandomNodeLocation();

		if (!target_enemy->canWalkTo(location))
			enemy_can_see = FALSE;
	}

	m_pOwner->GetPathPlanner()->RequestPathToPosition(location);
	AddSubgoal(new Goal_MoveToPosition(m_pOwner, location));


}

//------------------------------ Process -------------------------------------
//-----------------------------------------------------------------------------
int Goal_RunAway::Process()
{
	ActivateIfInactive();

	m_iStatus = ProcessSubgoals();

	return m_iStatus;
}


//------------------------------ Terminate -------------------------------------
//----------------------------------------------------------------------------- 

void Goal_RunAway::Terminate()
{
	m_iStatus = completed;
}