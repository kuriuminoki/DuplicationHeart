#ifndef WORLD_DRAWER_H_INCLUDED
#define WORLD_DRAWER_H_INCLUDED

class World;
class Camera;
class CharacterDrawer;
class ObjectDrawer;
class AnimationDrawer;
class ConversationDrawer;


class TargetDrawer {
private:
	const double TARGET_EX = 0.3;
	double m_ex;
	int m_targetHandle1;
	int m_targetHandle2;
	int m_targetHandle3;
	const double PI = 3.14;
	double m_targetAngle1;
	double m_targetAngle2;
	double m_targetAngle3;
public:
	TargetDrawer();
	~TargetDrawer();

	void setEx(double ex) { m_ex = ex * TARGET_EX; }

	void draw();

private:
	void anime();
};


// ���E��`�悷��
class WorldDrawer {
private:

	// �`�悷�鐢�E
	const World* m_world;

	// �w�i�摜
	int m_noonHaikei;
	int m_eveningHaikei;
	int m_nightHaikei;

	// �L�����`��p
	CharacterDrawer* m_characterDrawer;

	// �I�u�W�F�N�g�`��p
	ObjectDrawer* m_objectDrawer;

	// �A�j���[�V�����`��p
	AnimationDrawer* m_animationDrawer;

	// �}�E�X�J�[�\������
	TargetDrawer m_targetDrawer;

	// HP�o�[
	int m_hpBarGraph;

	// skill�o�[
	int m_skillBarGraph;

	// �{�X��HP�o�[
	int m_bossHpBarGraph;

	// ��b�C�x���g
	ConversationDrawer* m_conversationDrawer;

	int m_enemyNotice;

	double m_exX, m_exY;

	int m_font;

public:
	WorldDrawer(const World* world);

	~WorldDrawer();

	// �Z�b�^
	void setWorld(World* world) { m_world = world; }

	// �`�悷��
	void draw(bool drawSkillBar);

private:

	void drawBattleField(const Camera* camera, int bright, bool drawSkillBar);
};

#endif